Serial Key Manager C++ API
==========================

This is the C++ API for interacting with the Serial Key Manager Web API.
Among the various methods available in the Web API the only ones currently
supported in the C++ API are Activate and Deactivate.

The key class in the C++ API is the basic_SKM class, which is templated
with two policy classes, SignatureVerifier and RequestHandler, responsible
for handling verification of signatures and making requests to the Web API,
respectivly.

Included in the C++ API are SignatureVerifier_OpenSSL and RequestHandler_curl
which are build around the OpenSSL (or LibreSSL) and curl libraries, respectivly.

Two interfaces for working with requests are availble, either one instantiates
a basic_SKM class, and then use the methods provided by the class, which will
then make a request to the Web API through the RequestHandler class. Or in
case it is more convenient, it is also possible to make a request to the
Web API through some other means, and use the handle_* functions provided in
basic_SKM.hpp with the raw reply from the Web API.

Two examples are provided in the examples/ directory. The first, example1.cpp,
shows how to use the basic_SKM class. The second, example2.cpp, shows how
to use the handle_* functions.
