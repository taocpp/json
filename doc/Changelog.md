# Changelog

## 1.0.0

**Not yet released**

## Milestones

Pre-1.0.0 milestones in rough reverse chronological order.

* Direct parsing to arbitrary types.
* Binding facilities and polymorphic factory.
* Optional base class for Value annotations.
* Support for Opaque Pointers in Values.
* JAXN data model with binary data and non-finite doubles.
* JSON Pointer and JSON Patch support.
* Support for CBOR, UBJSON and MsgPack.
* Support for Value Pointer in Values.
* Type Traits for interoperation with C++ types.
* Events Interface for modularity and Transformers.
* 100% conformance score in [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark).
* Inclusion of double-conversion library.
* Value Class with parsing and (pretty) printing.

## History

Development of TAOCPP / JSON started in October 2015 as an experiment to combine the [PEGTL](https://github.com/taocpp/PEGTL) JSON parser with a JSON value class based on a `union` that directly embeds standard containers for arrays, objects and strings.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
