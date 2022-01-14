# Changelog

## 1.0.0

**Not yet released**

## Milestones

Pre-1.0.0 milestones in rough reverse chronological order.

* Change value from `union` to `std::variant`.
* Change library to require and use C++17.
* Direct parsing to arbitrary types.
* Binding facilities and polymorphic factory.
* Optional custom Base class for Values.
* Support for Opaque Pointers in Values.
* JAXN data model with binary data and non-finite doubles.
* JSON Pointer and JSON Patch support.
* Support for CBOR, UBJSON and MsgPack.
* Support for Value Pointer in Values.
* Events Interface for modularity and Transformers.
* Type Traits for interoperation with C++ types.
* 100% conformance score in [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark).
* Inclusion of double-conversion library.
* Value Class with parsing and (pretty) printing.

## History

Development of taoJSON started in October 2015 as an experiment to combine the [PEGTL](https://github.com/taocpp/PEGTL) JSON parser with a JSON value class based on a `union` that directly embeds standard containers for arrays, objects and strings.

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
