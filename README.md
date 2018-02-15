# The Art of C++ / JSON

[![Release](https://img.shields.io/github/release/taocpp/json.svg)](https://github.com/taocpp/json/releases/latest)
[![TravisCI](https://travis-ci.org/taocpp/json.svg)](https://travis-ci.org/taocpp/json)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/taocpp/json?svg=true)](https://ci.appveyor.com/project/taocpp/json)
[![Coverage](https://img.shields.io/coveralls/taocpp/json.svg)](https://coveralls.io/github/taocpp/json)

The Art of C++ / JSON is a zero-dependency C++11 header-only library that provides a generic JSON Value object, conversions from and to JSON string representation, and supports JSON Pointer and JSON Patch. It is highly extensible with a traits mechanism for custom conversion from and to (custom) C++ types, and an Events interface for (custom) conversion from and to multiple other file formats or even JSON value representations. The library is designed for correctness and simplicity, is based on C++ standard containers, and strives for speed through minimalism (rather than complicated optimisations).

## Documentation

* [Work-in-progress Documentation](doc/README.md)

## Features

* Serious standard conformance!

  * [RFC7159], [ECMA-404]: The JavaScript Object Notation (JSON) Data Interchange Format.
  * Achieves a **100% score** in the [Native JSON Benchmark] conformance tests.
  * [RFC6901]: JavaScript Object Notation (JSON) Pointer.
  * [RFC6902]: JavaScript Object Notation (JSON) Patch.
  * [RFC7049]: Concise Binary Object Representation ([CBOR]).
  * [JAXN]: a standard for relaxed JSON.
  * [MsgPack]: MessagePack binary format (without extensions).
  * [UBJSON]: Universal Binary JSON.
  * Experimental support for [JSON Reference] (local URI fragments only).
  * Experimental support for [JSON Schema] (draft v4).

* General

  * Provides JSON Value Class (DOM-style).
  * Provides JSON Events Interface (SAX-style).
  * Numeric values are stored as `std::int64_t`, `std::uint64_t` or `double`.
  * Allows storing and handling of non-finite floating point values `NaN`, `Infinity` and `-Infinity`.
  * Allows storing and handling of binary data.
  * Supports string views and binary views to avoid copying data.
  * Currently only supports UTF-8 as input and output encoding.

* Value Class

  * Construction of objects and arrays via `std::initializer_list`.
  * Allows construction of JSON value objects from arbitrary *user-defined types* with specialised traits class template.
  * Built around standard STL containers
    * `std::string` for JSON strings,
    * `std::vector< tao::json::value >` for JSON arrays,
    * `std::map< std::string, tao::json::value >` for JSON objects,
    * `std::vector< tao::json::byte >` for JAXN binary data. (`tao::json::byte` is an alias for `std::byte` when available).
  * Does *not* support storing of duplicate keys in JSON objects.
  * No memory allocations by the JSON value class itself (the wrapped standard containers perform their memory allocations normally).
  * Indirect JSON values and *user-defined types* via non-owning C++ raw pointers for efficient object sharing and other optimisations.
  * C++11 literal operator for JSON values, including binary data.
  * Optional user-defined annotations for every (sub-)value (e.g. [the filename and position the (sub-)value was parsed from](include/tao/json/contrib/position.hpp).

* Events Interface

  * Simple, straightforward and flexible JSON Events API.
  * JSON Events-to-Value and Value-to-Events interfaces.
  * Parse JSON string representation to JSON Events.
  * Parse JAXN string representation to JSON Events.
  * Stream JSON Events to (prettified) JSON string representation.
  * Stream JSON Events to (prettified) JAXN string representation.
  * Supports conversion from and to binary formats, e.g. [CBOR], [UBJSON], [MsgPack].
  * Supports conversion from and to *other* JSON value objects (e.g. [`contrib/nlohmann.cpp`](contrib/nlohmann.cpp)).
  * JSON Events comparison (against an existing JSON Value).
  * JSON Events hash algorithm (SHA-256 based).
  * JSON Events schema validator.

## Status

The library is mostly stable as in "works correctly and does not crash" over months of multiple real-world applications.
It is less stable in the sense that some parts of the API are still under discussion and development and might change and/or be extended before the first release.

This library also serves as a fully functional real-world example for the [Parsing Expression Grammar Template Library (PEGTL)], which is used for parsing JSON and JAXN string representations.

## Design

JSON libraries for C++ can be classified according to certain properties and characteristics.

1. Libraries with a value-generic in-memory representation based on standard containers that can easily read, write and modify any JSON value.
2. Libraries with a value-generic library-specific optimised in-memory representation that can easily and very quickly read and write any JSON value, but compromise on easy manipulation of in-memory values.
3. Libraries that directly use normal C++ data types as value-specific targets to very quickly read and write corresponding JSON values in a type-safe manner.

This library is of both the first *and* the third kind, and supports a hybrid model where the C++ JSON value contains pointers to arbitrary C++ data types as sub-values.

It employs the [Events interface](https://github.com/taocpp/json/blob/master/doc/Events-Interface.md) as universal adapter within the library (and as bridge to other libraries), and the traits mechanism to allow for seamless integration of, and conversion from and to, user-defined C++ data types.

## License

The Art of C++ / JSON is certified [Open Source] software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license] reproduced here.

> Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Contact

For questions and suggestions about the Art of C++ / JSON please contact the authors at `taocpp(at)icemx.net`.

## Thank You

* Niels Lohmann and his [JSON library](https://github.com/nlohmann/json) for the inspiration.
* Milo Yip and his [Native JSON Benchmark] for a reference to measure progress against.

## Additional Licenses

The Art of C++ / JSON contains slightly modified portions of the [double-conversion library](https://github.com/google/double-conversion) which is licensed as follows:

> Copyright 2006-2011, the V8 project authors. All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
>
> * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
> * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
> * Neither the name of Google Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

[CBOR]: http://cbor.io
[ECMA-404]: http://www.ecma-international.org/publications/standards/Ecma-404.htm
[JAXN]: https://github.com/stand-art/jaxn
[JSON Reference]: https://tools.ietf.org/html/draft-pbryan-zyp-json-ref-03
[JSON Schema]: http://json-schema.org/documentation.html
[MsgPack]: http://msgpack.org
[MIT license]: http://www.opensource.org/licenses/mit-license.html
[Native JSON Benchmark]: https://github.com/miloyip/nativejson-benchmark
[Open Source]: http://www.opensource.org/docs/definition.html
[Parsing Expression Grammar Template Library (PEGTL)]: https://github.com/taocpp/PEGTL
[RFC6901]: https://tools.ietf.org/html/rfc6901
[RFC6902]: https://tools.ietf.org/html/rfc6902
[RFC7049]: https://tools.ietf.org/html/rfc7049
[RFC7159]: https://tools.ietf.org/html/rfc7159
[UBJSON]: http://ubjson.org
