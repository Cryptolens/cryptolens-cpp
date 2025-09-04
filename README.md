# Cryptolens Client API for C++

On this page, we have outlined several examples of how to get started with the Cryptolens Client API for C++.

> **Note**, Cryptolens Client API for C++ currently supports a subset of the full Cryptolens Web API. Please contact us if you need something in particular.

You can find the API documentation here: [https://help.cryptolens.io/api/cpp/](https://help.cryptolens.io/api/cpp/).

If you are already familiar with the .NET version of the library, we have summarized key differences in an [announcement](https://cryptolens.io/2017/08/new-client-api-for-c/) on our blog.

## Table of contents

- [Example projects](#example-projects)
  - [CMake](#cmake) (for Linux)
  - [Visual Studio](#visual-studio) (for Windows)
- [Library overview](#library-overview)
- [Error handling](#error-handling)
- [Offline activation](#offline-activation)
- [HTTPS requests outside the library](#https-requests-outside-the-library)

## Example projects

[This repository](https://github.com/Cryptolens/cryptolens-cpp) contains some example projects using the library in the `examples/` directory. The directory contains examples for Unix and Windows. The Unix example projects use CMake as the build system and depend on OpenSSL and libcurl. The Windows example projects are built using Visual Studio and depend on WinHTTP and CryptoAPI. The rest of this section contains instructions for how to build the example projects.

### CMake

First we need to install libcurl and OpenSSL, including the header files, unless that has
already been done.

```shellscript
Debian/Ubuntu:
$ apt-get install libcurl4-openssl-dev libssl-dev
Fedora/Red Hat:
$ yum install libcurl-devel openssl-devel
```

Next, clone the repository and build the examples

```shellscript
$ git clone https://github.com/Cryptolens/cryptolens-cpp.git
$ cd cryptolens-cpp/examples/unix/cmake
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
$ ./example_activate
```

### Visual Studio

Getting started with the example project for Visual Studio requires two steps. First we
build the library file the example project will statically link against, then we build
the example project.

The following steps build the library:

- Open the solution file _vsprojects/Cryptolens.sln_ in Visual Studio.
- Set platform and configuration as appropriate, e.g. _x64_ and _Debug_
- Build the project in Visual Studio
- Now the folder _vsprojects/Output/Platform/Configuration/_ contains the _Cryptolens.lib_ file. With platform set to "x64" and configuration set to "Debug", the file is _vsprojects/Output/x64/Debug/Cryptolens.lib_

Now we can build the example project:

- Open _examples/VisualStudio/VisualStudio.sln_
- Set configuration and platform in the same way as when building the library
- Build and run the project.

Instructions for how to add the library to your own Visual Studio project can be found [here](/tutorials/add-to-project-windows.md).

## Library overview

This section contains an overview of the standard way to implement the library in an
application. The first step is to include the appropriate headers:

```cpp
#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_XXX.hpp>
#include <cryptolens/MachineCodeComputer_YYY.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;
```

Besides including headers the above code sets up a namespace alias for the api version of the C++
library we are using.

The `Configuration` class allows for using different libraries for parsing JSON, making HTTPS
requests, performing cryptographic operations as well as minor changes in the behaviour of the
library. We currently support the following `Configurations` and `MachineCodeComputers`:

| Configuration                         | Description                                           |
| -----------------------------------   | ----------------------------------------------------- |
| `Configuration_Unix_IgnoreExpires`    | Suggested default configuration for Unix-like systems. Use s ArduinoJson 5, libcurl and OpenSSL. Does not check if the license key has expired against the system time. |
| `Configuration_Unix_CheckExpires`     | Same as `Configuration_Unix`, but additionally checks if the license key has expired against the system time. |
| `Configuration_Windows_IgnoreExpires` | Suggested default configuration for Windows based systems. Uses ArduinoJson 5, WinHTTP and CryptoAPI. Does not check if the license key has expired against the system time. |
| `Configuration_Windows_CheckExpires`  | Same as `Configuration_Windows`, but additionally checks if the license key has expired against the system time. |

| MachineCodeComputer                            | Description                                     |
| ---------------------------------------------- | ----------------------------------------------- |
| `MachineCodeComputer_COM`                      | Works on Windows and computes a machine code using functionallity provided through COM. |
| `MachineCodeComputer_static`                   | Does not automatically compute a machine code, instead the machine code is set by calling `set_machine_code()`. |
| `MachineCodeComputer_SystemdDBusInodes_SHA256` | Works on Linux systems and computes a machine code based on information provided by Systemd, DBus and the filesystem. |

The next step is to create and set up a handle class responsible for making requests
to the Cryptolens Web API.

```cpp
using Cryptolens = cryptolens::basic_Cryptolens
                     <cryptolens::Configuration_XXX<cryptolens::MachineCodeComputer_static>>;

cryptolens::Error e;
Cryptolens cryptolens_handle(e);

cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");

// This line is only for MachineCodeComputer_static and sets the machine code to a static value
cryptolens_handle.machine_code_computer.set_machine_code(e, "289jf2afs3");
```

Here the strings `"ABCDEFGHI1234"` and `"ABCD"` needs to be replaced by your public keys. These
can be found when logged in on Cryptolens.io from the menu in the top-right corner
("Hello username\!") and then _Security Settings_. The example above corresponds to
the following value on the website

```
<RSAKeyValue><Modulus>ABCDEFGHI1234</Modulus><Exponent>ABCD</Exponent></RSAKeyValue>
```

In this example we set the machine code used to `"289jf2afs3"`.

Now that the handle class has been set up, we can attempt to activate a license key

```cpp
cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.activate
    ( e, // Object used for reporting if an error occured
      "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0=", // Cryptolens Access Token
      3646, // Product id
      "MPDWY-PQAOW-FKSCH-SGAAU" // License Key
    );

if (e) {
  // An error occured. Handle it.
  handle_error(e);
  return 1;
}
```

The `activate` method takes several arguments:

1. The first argument is used to indicate if an error occured, and if so can provide additional
   information. For more details see the section _Error handling_ below.
2. We need an access token with the `Activate` scope. Access tokens can be created at
   https://app.cryptolens.io/User/AccessToken/.
3. The third argument is the product id, this can be found at the page for the corresponding product at
   https://app.cryptolens.io/Product.
4. The fourth argument is a string containing the license key string, in most cases this will be
   input by the user of the application in some application dependent fashion.

After the `activate` call we check if an error occurred by converting `e` to bool. If an error
occured this returns true. If an error occurs, the optional containing the `LicenseKey` object
will be empty, and dereferencing it will result in undefined behaviour.

If no error occurs we can inspect the `LicenseKey` object for information as follows

```cpp
if (license_key->check()->has_expired(1234567)) {
  // Above, the value 1234567 represents the current time as a unix timestamp in seconds

  std::cout << "Your subscription has expired." << std::endl;
  return 1;
}

if (license_key->check()->has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
else                                      { std::cout << "Welcome!" << std::endl; }
```

## Error Handling

This section explains how the Cryptolens C++ library handles errors. The library adopts an exceptionless design, using return values with optionals to handle cases where a value might be absent. Many functions accept a reference to a `cryptolens::basic_Error` object as their first argument, which is used to report errors and provide detailed error information.

### Error Classes

The library defines two key classes for error handling:

- `cryptolens::basic_Error`: An abstract base class that defines the interface for error handling. It specifies the required methods for error reporting.
- `cryptolens::Error`: A concrete implementation of the `cryptolens::basic_Error` interface. This is the error class used in our examples and by most users of the library.

This design allows users to create custom error classes by subclassing `cryptolens::basic_Error` instead of using the provided `cryptolens::Error`. Details on subclassing `cryptolens::basic_Error` are provided in the section [Custom error class](#custom-error-class) below.

### Interface Overview

The `cryptolens::basic_Error` interface provides detailed information about errors through several methods. Errors are categorized by subsystem (e.g., API, networking, or cryptographic operations), allowing precise identification of the error’s origin.

The error information consists of three components:

- **Subsystem**: Identifies the subsystem (e.g., API, networking, cryptography) where the error occurred, accessible via the `get_subsystem()` method.
- **Reason**: Describes the specific cause of the error within the subsystem, accessible via the `get_reason()` method.
- **Extra**: Provides additional details, such as error codes from underlying libraries (e.g., OpenSSL, WinHTTP), accessible via the `get_extra()` method.

> Note: Dividing errors into subsystems ensures that error codes from different libraries (e.g., HTTP or cryptographic libraries) do not conflict, enabling clear and unambiguous error reporting.

To check if an error has occured, the error class allows an implicit conversion to a boolean. This allows for checking if an error has occured using a standard if-statement. For example:

```cpp
cryptolens::Error e;

f(e);

if (e) {
  // Error has occured. Handle it!
}
```

Additionally, the `get_call()` method indicates which method call triggered the error. How this method can be used is described in more detail below in section [Behaviour When Error Has Occured](#behaviour-when-error-has-occured).

The following table summarizes the key methods of the `cryptolens::basic_Error` interface:

| **Method**        | **Description**                                                   |
| ----------------- | ----------------------------------------------------------------- |
| `get_subsystem()` | Returns the subsystem where the error occurred.                   |
| `get_reason()`    | Returns the specific reason for the error.                        |
| `get_extra()`     | Returns additional details, such as library-specific error codes. |
| `get_call()`      | Returns the method call that triggered the error.                 |

### Behavior When Error Has Occured

If an error occurs during a method call, the `cryptolens::basic_Error` object is updated to reflect the error. In this case, subsequent calls to methods that accept a reference to a `cryptolens::basic_Error` object then become no-ops and return default values. This design simplifies error handling by allowing users to defer error checks until after multiple method calls, rather than checking after each method call.

For example:

```cpp
cryptolens::Error e;

f1(e);
f2(e);

if (e) {
  // Handle error from f1 or f2
}
```

To identify which method caused the error, the `get_call()` method can be used.

### Error codes

#### Subsystem overview

Errors from the library are divided into multiple subsystems to allow identifying the origin of an error. As described above, this information is available through the `get_subsystem()` method on the error class.

Constants indicating in which subsystem an error occured are available in the namespace `::cryptolens_io::v20190401::errors::Subsystem` and are included by the file `cryptolens/core.hpp`.

The possible values are as follows:

| **Subsystem**                  | **Numeric value** | **Description**                                                                                |
| ------------------------------ | ----------------- | ---------------------------------------------------------------------------------------------- |
| `Subsystem::Ok`                | 0                 | Indicates that no error has occured.                                                           |
| `Subsystem::Main`              | 1                 | Indicates that a request was made to the server and the server returned an error.              |
| `Subsystem::Json`              | 2                 | Indicates that an error occured when dealing with JSON.                                        |
| `Subssytem::Base64`            | 3                 | Indicates that an error occured when dealing with Base64 encoded data.                         |
| `Subsystem:: RequestHandler`   | 4                 | Indicates that an error occured when connecting to the server.                                 |
| `Subsystem::SignatureVerifier` | 5                 | Indicates that an error occured when checking cryptographic signatures in the server response. |

#### Subsystems

**Main subssystem**

The main subsystem consists of errors where the request was made successfully to the server and the server returned an error.

The main subsystem uses the reason field for communicating more information about what went wrong, but does not use the extra field.

Constants for the possible values for the reason field are available in the namespace `::cryptolens_io::v20190401::errors::Main` and are included by the file `cryptolens/core.hpp`.

The possible values for the reason field are as follows:

| **Reason**                                              | **Numeric value** | **Description**                                                                                                                                                                                                                 |
| ------------------------------------------------------- | ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Main::UNKNOWN_SERVER_REPLY`                            | 1                 | General error that indicates that the reply from the server was not in format expected by the library. This error should not happen in general but can occur if there is e.g. a proxy that alters the response from the server. |
| `Main::INVALID_ACCESS_TOKEN`                            | 2                 | Indicates that the access token used is not a valid access token for this user.                                                                                                                                                 |
| `Main::ACCESS_DENIED`                                   | 3                 | Indicates that the the access token used does not have permission to perform the requested operation.                                                                                                                           |
| `Main::INCORRECT_INPUT_PARAMETER`                       | 4                 | Indicates that a parameter was incorrect.                                                                                                                                                                                       |
| `Main::PRODUCT_NOT_FOUND`                               | 5                 | Indicates that the product parameter was incorrect.                                                                                                                                                                             |
| `Main::KEY_NOT_FOUND`                                   | 6                 | Indicates that the key could not be found.                                                                                                                                                                                      |
| `Main::KEY_BLOCKED`                                     | 7                 | Indicates that the operation could not be performed because the key was blocked.                                                                                                                                                |
| `Main::DEVICE_LIMIT_REACHED`                            | 8                 | Indicates that the operation could not be performed because the maximum number of activated machines was reached.                                                                                                               |
| `Main::KEY_EXPIRED`                                     | 9                 | Indicates that the operation could not be performed because the key has expired.                                                                                                                                                |
| `Main::MACHINE_CODE_NOT_ACTIVATED_OR_NO_KEY_ACTIVATION` | 10                | Indicates that XXX                                                                                                                                                                                                              |

**Json subsystem**

Indicates that an error occured when processing a Json value. This subsystem does currently not provide more information about the exact reason for why the error occured.

**Base64 subsystem**

Indicates that an error occured when decodring a Base64 value. This subsystem does currently not provide more information about the exact reason for why the error occured.

**RequestHandler subsystem**

Indicates that en error occured when making a network connection to the server.

This subsystem uses the reason value to provide a more detailed location in the code where the error occured, to aid debugging, and uses the extra field to propagate the error from the underlying library used for making network connections.

_Curl_

The value of the extra field is a CURLcode error code. The possible values, what causes them and what can be done to resolve the problem is available in [Curls documentation](https://curl.se/libcurl/c/libcurl-errors.html) and also mirrored in [our documentation](/libraries/cpp/libcurl-errors).

_WinHTTP_

The value of the extra field is a debug system error code, and the full list of possible values can be found in Microsoft's documentation about [Debug system error codes](https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes).

In most cases the code will be from WinHTTP itself, and those values are described in Microsoft's documentation about [Error Messages (Winhttp.h)](https://learn.microsoft.com/en-us/windows/win32/winhttp/error-messages).

**SignatureVerifier subsystem**

This subsystem uses the reason value to indicate where the error occured. Sometimes additional information is provided in the extra field.

Note we recommend using these values only for debugging purpuses, and do not recommend giving access to them in an automated way where e.g. another computer program can read the value. The values may reveal information about precisely where a cryptographic operation failed, which may be useful for circumventing the cryptographic protections.

## Offline activation

One way to support activation while offline is to initially make one activation request
while connected to the internet and then saving this response. By then reading the saved
response and performing the cryptographic checks it is not necessary to make another
request to the Web API in the future. Thus we can proceed as during online activation
but save the response as a string:

```cpp
cryptolens::Error e;
Cryptolens cryptolens_handle(e);

cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");

cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.activate
    ( e, // Object used for reporting if an error occured
      "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0=", // Cryptolens Access Token
      3646, // Product id
      "MPDWY-PQAOW-FKSCH-SGAAU" // License Key
    );
if (e) { handle_error(e); return 1; }

std::string s = license_key->to_string();
```

The string `s` can now be saved to a file or similar, in an application dependent manner. In order
to check the license later when offline, load the string `s` and recover the license key as follows:

```cpp
cryptolens::Error e;
Cryptolens cryptolens_handle(e);

cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");

cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.make_license_key(e, s);
if (e) { handle_error(e); return 1; }
```

A full working version of the code above can be found as _example_offline.cpp_ among the examples.

## HTTPS requests outside the library

In some cases it may be needlessly complex to have the Cryptolens library be responsible
for initiating the HTTPS request to the Web API, instead it might be easier to have some
other part of the application that does the HTTPS request, and then only use this library
for making sure that the cryptographic signatures are valid. This can be accomplished
by using the `handle_activate()` function as follows

```cpp
std::string web_api_response; // Some other part of the code creates and populates this object

cryptolens::Error e;
Cryptolens cryptolens_handle(e);

cryptolens_handle.signature_verifier.set_modulus_base64(e, "ABCDEFGHI1234");
cryptolens_handle.signature_verifier.set_exponent_base64(e, "ABCD");

cryptolens::optional<cryptolens::LicenseKey> license_key =
  cryptolens_handle.make_license_key(e, web_api_response);
if (e) { handle_error(e); return 1; }
```

This code is also applicable to the case where one wants a completly air-gapped offline activation.
In this case the `web_api_response` string would be prepared and delivered one for example an USB
thumb drive, and the application then reads this response from the device and stores it in
the `web_api_response` string.
