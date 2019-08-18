The Cryptolens Client API for C++ is licensed under the 2-Clause BSD License
given below. The client API consists of the files in the `examples/`, `include/`,
`src/` and `vsprojects/` directories.

```
Copyright (c) 2019, Cryptolens AB
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

## Dependencies

The client API depends on other libraries for it to function correctly.

Currently, all versions and configurations are set up to use the
base64 implementation found in `third_party/base64_OpenBSD/`. This
implementation is licensed under a ISC license as well as something
similar to a 3-Clause BSD license, for more details, see the files in
the directory.

In the `master` branch on Github we have for conveniance included several
other dependencies in the `third_party/` directory. Which of these are used
depends on how the `basic_Cryptolens` or `basic_SKM` class is set up, as well
as how you set up your build environment.

The following sections cover which dependencies from `third_party/` are
included by default for the different api versions.

### Core dependencies

This section details the dependencies which are used independently of which
template arguments are provided to the `basic_Cryptolens` or `basic_SKM`
class. 

More precisly, the following dependencies are included when using the
`cryptolens_io::v20180502` namespace, or when using the
`cryptolens_io::v20190401` namespace with `Configuration_Unix`,
`Configuration_Unix_IgnoreExpires`, `Configuration_Windows` or
`Configuration_Windows_IgnoreExpires` as the `Configuration` template
argument to `basic_Cryptolens` class.

 * [ArduinoJson 5](https://github.com/bblanchon/ArduinoJson) from the
   `third_party/ArduinoJson` directory.
 * [optional lite](https://github.com/martinmoene/optional-lite) from
   the `third_party/optional-lite` is used when compiling against
   C++-14 or below.

### Configuration dependent dependencies

When using `Configuration_Windows`, `Configuration_Windows_IgnoreExpires`
or `RequestHandler_WinHTTP` the default build configurations link against
the code in `third_party/curl`. This code consists of a single, very simple
function from the [cURL](https://curl.haxx.se/) library. It is unclear
to us if this function is complex enough to be covered by copyright.
