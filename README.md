# Cryptolens Client API for C++

On this page, we have outlined several examples of how to get started with the [Cryptolens Client API](/web-api/skm-client-api) for C++.

> **Note**, Cryptolens Client API for C++ currently supports **activation** and **deactivation** methods. Support for more methods is coming soon.

You can find the API documentation here: [https://api.serialkeymanager.com/cpp/](https://api.serialkeymanager.com/cpp/).

If you are already familiar with the .NET version of the library, we have summarized key differences in an [announcement](https://cryptolens.io/2017/08/new-client-api-for-c/) on our blog.


## Table of contents

* [Example projects](#example-projects)
  - [CMake](#cmake) (for Linux)
  - [Visual Studio](#visual-studio) (for Windows)
* [Library overview](#library-overview)
* [Error handling](#error-handling)
* [Offline activation](#offline-activation)
* [HTTPS requests outside the library](#https-requests-outside-the-library)


## Example projects

[This repository](https://github.com/Cryptolens/cryptolens-cpp) contains some example projects using the library in the examples/ directory.
The cmake example project is set up to be compiled against OpenSSL and libcurl, while the
VisualStudio project builds against the CryptoAPI and WinHTTP libraries available on Windows.
The rest of this section contains instructions for how to build the example projects.

### CMake

First we need to install libcurl and OpenSSL, including the header files, unless that has
already been done.

```
Debian/Ubuntu:
$ apt-get install libcurl4-openssl-dev libssl-dev
Fedora/Red Hat:
$ yum install libcurl-devel openssl-devel
```

Next, clone the repository and build the examples

```
$ git clone https://github.com/Cryptolens/cryptolens-cpp
$ cd cryptolens-cpp/examples/cmake
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
$ ./example1
```

### Visual Studio

Getting started with the example project for Visual Studio requires two steps. First we
build the library file the example project will statically link against, then we build
the example project.

The following steps build the library:

 * Open the solution file *vsprojects/Cryptolens.sln* in Visual Studio.
 * Set platform and configuration as appropriate, e.g. *x64* and *Debug*
 * Build the project in Visual Studio
 * Now the folder *vsprojects/Output/$Platform/$Configuration/* contains the *Cryptolens.lib* file. With platform and configuration as above, the file is *vsprojects/Output/x64/Debug/Cryptolens.lib*

Now we can build the example project:

 * Open *examples/VisualStudio/VisualStudio.sln*
 * Set configuration and platform in the same way as when building the library
 * Build and run the project.

> For setting up your own Visual Studio project to use the library, we have a step-by-step guide with pre-compiled binaries [here](https://help.cryptolens.io/web-api/cpp/windows).


## Library overview

This section contains an overview of the standard way to implement the library in an
application. The first step is to include the appropriate headers:

```cpp
#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/RequestHandler_XXX.hpp>
#include <cryptolens/SignatureVerifier_YYY.hpp>
```

We currently support the following RequestHandlers and SignatureVerifiers

| RequestHandler                | Description                                           |
| ----------------------------- | ----------------------------------------------------- |
| `RequestHandler_curl`         | Uses libcurl                                          |
| `RequestHandler_WinHTTP`      | Uses the WinHTTP library available as part of Windows |

| SignatureVerifier             | Description                                     |
| ----------------------------- | ----------------------------------------------- |
| `SignatureVerifier_OpenSSL`   | Uses OpenSSL or LibreSSL                        |
| `SignatureVerifier_CryptoAPI` | Uses the CryptoAPI available as part of Windows |

The next step is to create and set up a handle class responsible for making requests
to the Cryptolens Web API.

```cpp
using Cryptolens = cryptolens::basic_Cryptolens
                     <cryptolens::RequestHandler_curl, cryptolens::SignatureVerifier_OpenSSL>;

Cryptolens cryptolens_handle;
cryptolens::Error e;
cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");
```

Here the strings `"ABCDEFGHI1234"` and `"ABCD"` needs to be replaced by your public keys. These
can be found when logged in on Cryptolens.io from the menu in the top-right corner
("Hello <username>!") and then *Security Settings*. The example above corresponds to
the following value on the website

```
<RSAKeyValue><Modulus>ABCDEFGHI1234</Modulus><Exponent>ABCD</Exponent></RSAKeyValue>
```

Now that the handle class has been set up, we can attempt to activate a license key

```cpp
cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.activate
    ( // Object used for reporting if an error occured
      e
    , // Cryptolens Access Token
      "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0="
    , // Product id
      "3646"
    , // License Key
      "MPDWY-PQAOW-FKSCH-SGAAU"
    , // Machine Code
      "289jf2afs3"
    );

if (e) {
  // An error occured. Handle it.
  handle_error(e);
  return 1;
}
```

The `activate` method takes several arguments:

 1. The first argument is used to indicate if an error occured, and if so can provide additional
    information. For more details see the section *Error handling* below.
 1. We need an access token with the `Activate` scope. Access tokens can be created at
    https://app.cryptolens.io/User/AccessToken/.
 1. The third argument is the product id, this can be found at the page for the corresponding product at
    https://app.cryptolens.io/Product.
 1. The fourth argument is a string containing the license key string, in most cases this will be
    input by the user of the application in some application dependent fashion.
 1. The last argument is an optional identifier for the device the application is running on, or
    something similar.

After the `activate` call we check if an error occurred by converting `e` to bool. If an error
occured this returns true. If an error occurs, the optional containing the `LicenseKey` object
will be empty, and dereferencing it will result in undefined behaviour.

If no error occurs we can inspect the `LicenseKey` object for information as follows

```cpp
std::cout << "License key for product with id: " << license_key->get_product_id() << std::endl;

if (license_key->check()->has_expired(1234567)) {
  std::cout << "Your subscription has expired." << std::endl;
  return 1;
}

if (license_key->check()-has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
else                                     { std::cout << "Welcome!" << std::endl; }
```


## Error handling

This section describes in more detail how the library reports when something went wrong.
The library uses an exceptionless design and return values use optionals where they can be missing.
All functions accept as the first argument a reference to `cryptolens::basic_Error` which
is used to indicate when an error has occured as well as for reporting more detailed error information.

Furthermore, if an error occurs, subsequent calls to the library with that error object as first
argument are turned into noops. consider the following code:

```cpp
cryptolens::Error e;
f1(e);
f2(e);
```

If an error occurs during the call to `f1()`, `f2()` will not be performed. This allows writing
several function calls after each other without having to check for errors after
every single call.


## Offline activation

One way to support activation while offline is to initially make one activation request
while connected to the internet and then saving this response. By then reading the saved
response and performing the cryptographic checks it is not necessary to make another
request to the Web API in the future. Thus we can proceed as during online activation
but save the response as a string:

```cpp
Cryptolens cryptolens_handle;
cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");
...
cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.activate(
    ( // Object used for reporting if an error occured
      e
    , // Cryptolens Access Token
      "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0="
    , // Product id
      "3646"
    , // License Key
      "MPDWY-PQAOW-FKSCH-SGAAU"
    , // Machine Code
      "289jf2afs3"
    );
if (e) { handle_error(e); return 1; }

std::string s = license_key->to_string();
```

The string *s* can now be saved to a file or similar, in an application dependent manner. In order
to check the license later when offline, load the string *s* and recover the license key as follows:

```cpp
cryptolens::Error e;
Cryptolens cryptolens_handle;
cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");

cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.make_license_key(e, s);
if (e) { handle_error(e); return 1; }
```

A full working example can be found as *example4.cpp* in the examples directory.

## HTTPS requests outside the library

In some cases it may be needlessly complex to have the Cryptolens library be responsible
for initiating the HTTPS request to the Web API, instead it might be easier to have some
other part of the application that does the HTTPS request, and then only use this library
for making sure that the cryptographic signatures are valid. This can be accomplished
by using the `handle_activate()` function as follows

```cpp
std::string web_api_response; // Some other part of the code creates and populates this object

SignatureVerifier_XXX signature_verifier;
cryptolens::Error e;
signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
signature_verifier.set_exponent_base64(e, "ABCD");

cryptolens::optional<cryptolens::LicenseKey> key =
  cryptolens::handle_activate(e, signature_verifier, web_api_resonse);
if (e) { handle_error(e); return 1; }
```

This code is also applicable to the case where one wants a completly air-gapped offline activation.
In this case the `web_api_response` string would be prepared and delivered one for example an USB
thumb drive, and the application then reads this response from the device and stores it in
the `web_api_response` string.
