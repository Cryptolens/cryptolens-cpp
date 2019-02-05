# Cryptolens C++ Examples

This directory contains several source code examples of how to use the Cryptolens Client
API for C++ with Visual Studio. The examples use windows specific libraries such as
WinHTTP and CryptoAPI.

Note that in order to build the examples, the library needs to be build using the
solution found in the */vsprojects* folder in this repository.

## Example descriptions

|  Filename            | Description |
|----------------------|-------------|
| Example_Activate | The most basic example. Makes a single activate call checking that a license key is valid. |
| Example_External | An example showing how another part of the application can make the HTTP(S) request, and the response is provided to the Cryptolens C++ API. |
| Example_Floating | Showing how floating licencing could be used. |
| Example_Offline  | Showing how the response from the Web API can be saved, and later restored and checked. This could be used to check the license even when the user is offline. |
