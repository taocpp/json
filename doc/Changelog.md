# Changelog

## 1.0.0

**Not yet released*

## Milestones

Pre-1.0.0 milestones in rough reverse chronological order.

* Optional base class for value annotations.
* Support for opaque pointers in value objects.
* JAXN data model with binary data and non-finite doubles.
* JSON Reference and JSON Schema support.
* JSON Pointer and JSON Patch support.
* Support for CBOR, UBJSON and MsgPack.
* Support for value pointers in value objects.
* Traits class for custom type integration.
* Events interface for modularity and filters.
* 100% conformance score in [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark).
* Inclusion of double-conversion library.
* Value class with parsing and (pretty) printing.

## History

Development of TAOCPP / JSON started in October 2015 as an experiment to combine the [PEGTL](https://github.com/taocpp/PEGTL) JSON parser with a JSON value class based on a `union` that directly embeds standard containers for arrays, objects and strings.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
