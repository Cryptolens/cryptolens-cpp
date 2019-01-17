# Static linking

This directory contains an example of of to statically link the library against OpenSSL
and libcurl.

The main part of the example is available in the Dockerfile. The Dockefile first
downloads and builds OpenSSL and libcurl. Then it compiles the unix example programs
with static linking against OpenSSL and libcurl.

The Dockerfile both shows how to compile the library by calling g++ and by using CMake.
Note that the CMake solution is a temporary solution, but in order to support static
linking with the ordinary CMakeLists.txt the structure may have to be changed somewhat.
