# Cryptolens Licensing for C++ CLI

This repository contains an example of how the [Cryptolens C++ library](https://github.com/cryptolens/cryptolens-cpp) can be used with [C++ CLI](https://en.wikipedia.org/wiki/C%2B%2B/CLI).

The example contains two parts, first a wrapper written in ordinary C++ available in the `src/` and `include/` directories. This wrapper is intended to be modified to suit the target application and should contain most of the licensing logic. Then what information needs to be passed to the rest of the application can be added to the `CryptolensResult` struct.

The second part of this example consists of a simple console application written in C++ CLI, which is available in the `examples/` directory. This application calls methods exposed from the wrapper.

When adding the wrapper to the example project we made the following changes to project properties:

1. **Include files:** Under *Configuration Properties -> C\C++ -> General -> Additional Include Directories* we added the path to the directory `cryptolens-cpp-cli/include` from this repository.
1. **Library directories:** Under *Configuration Properties -> Linker -> General -> Additional Library Directories* we added the path to both `cryptolens-cpp\vsprojects\Output\$(Platform)\$(Configuration)` and `cryptolens-cpp-cli\$(Platform)\$(Configuration)`.
1. **Library dependencies:** Under *Configuration Properties -> Linker -> Input -> Additional Dependencies* we added the following dependencies `CryptolensCLI.lib`, `Cryptolens.lib`, `winhttp.lib`, and `Advapi32.lib`. Here `CryptolensCLI.lib` is the library for the wrapper and `Cryptolens.lib` is the library for the main Cryptolens C++ library. The dependencies `winhttp.lib` and `Advapi32.lib` are needed by the main Cryptolens C++ library, and provides support for `WinHTTP` and `CryptoAPI`, respectivly.
