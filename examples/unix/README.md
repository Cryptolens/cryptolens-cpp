# Cryptolens C++ Examples

This directory contains several source code examples of how to use the Cryptolens Client
API for C++. The examples are mainly targeting unix-like platforms, but most of them could
be built on e.g. Windows as well, assuming curl/OpenSSL is available.

Subdirectories of this folder show how the examples can be built with different build
systems.

## Example descriptions

|  Filename            | Description |
|----------------------|-------------|
| example_activate.cpp | The most basic example. Makes a single activate call checking that a license key is valid. |
| example_floating.cpp | Showing how floating licencing could be used. |
| example_messaging_api.cpp | Uses the messaging api to fetch a message of the day |
| example_offline.cpp  | Showing how the response from the Web API can be saved, and later restored and checked. This could be used to check the license even when the user is offline. |
| example_external.cpp | An example showing how another part of the application can make the HTTP(S) request, and the response is provided to the Cryptolens C++ API. |

## Subdirectory descriptions


|  Subdirectory        | Description |
|----------------------|-------------|
| cmake                | Shows how the use the CMakeLists.txt from the root of the repository in another project |
| static_docker        | Contains a Dockerfile showing how to statically link libcurl and OpenSSL. Shows this using CMake as well as using basic gcc/clang invocations |
