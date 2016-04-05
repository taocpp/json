# The Art of C++ / JSON

[![Release](https://img.shields.io/github/release/taocpp/json.svg)](https://github.com/taocpp/json/releases/latest)
[![License](https://img.shields.io/github/license/taocpp/json.svg)](#license)
[![TravisCI](https://travis-ci.org/taocpp/json.svg)](https://travis-ci.org/taocpp/json)
[![Coverage](https://img.shields.io/coveralls/taocpp/json.svg)](https://coveralls.io/github/taocpp/json)
[![Issues](https://img.shields.io/github/issues/taocpp/json.svg)](https://github.com/taocpp/json/issues)

The Art of C++ / JSON is a zero-dependency C++11 header-only library that provides a generic JSON value object as well as conversions from and to JSON string representation. It also serves as a fully functional real-world example application for the [Parsing Expression Grammar Template Library (PEGTL)](https://github.com/ColinH/PEGTL).

## Design

This library is designed for correctness and simplicity, and strives for speed through minimalism rather than complicated optimisations.

It uses `std::vector` for arrays, `std::map` for objects, `std::string` for strings, `bool` for booleans, and both `int64_t` and `double` for numbers.

The JSON value class contains a `union` of all of these types and does *not itself* perform any heap allocations (the wrapped `std::map`, `std::vector` and `std::string` *do* perform allocations normally).

## Status

The core JSON value class is implemented and working.

The conversions from and to JSON string representation are finished and achieve a 100% score in the [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark) conformance tests.

Some parts of the API are still under discussion and development and might change and/or be extended before the first release.

## Documentation

The documentation will be finished once the functionality and interfaces are finalised.

Until then, here are a few short indications on how to use this library:

* Requires Clang or GCC with `-std=c++11` (or other compiler with sufficient C++11 support).
* The library is header-only, to install and use simply copy the directory `include/tao` to a convenient place and include the file `include/tao/json.hh`.
* The generic JSON value class, the main part of this library, is in [`include/tao/json/value.hh`](include/tao/json/value.hh).
* To parse a JSON string representation, use one of the functions in `include/tao/json/from_string.hh` or `include/tao/json/parse_file.hh`.
* To produce a JSON string representation, use one of the appropriate functions in `include/tao/json/to_string.hh` or `include/tao/json/stream.hh`.

For questions and suggestions please contact the authors at **jsonl(at)colin-hirsch.net**.

#### Thank You

* Niels Lohmann and his [JSON library](https://github.com/nlohmann/json) for the inspiration.
* Milo Yip and his [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark) for a reference to measure progress against.

## License

<a href="http://www.opensource.org/"><img height="100" align="right" src="http://wiki.opensource.org/bin/download/OSI+Operations/Marketing+%26+Promotional+Collateral/OSI_certified_logo_vector.svg"></a>

The Art of C++ is certified [Open Source](http://www.opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license](http://www.opensource.org/licenses/mit-license.html) reproduced here.

> Copyright (c) 2015-2016 Dr. Colin Hirsch
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This site and software is not affiliated with or endorsed by the Open Source Initiative. For questions and suggestions about the Art of C++ / JSON please contact the authors at `jsonl(at)colin-hirsch.net`.

### PEGTL License

The Art of C++ / JSON library contains [the PEGTL library](https://github.com/ColinH/PEGTL) which is licensed as follows:

> Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Double-Conversion License

The Art of C++ / JSON library contains slightly modified portions of the [double-conversion library](https://github.com/google/double-conversion) which is licensed as follows:

> Copyright 2006-2011, the V8 project authors. All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
>
> * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
> * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
> * Neither the name of Google Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
