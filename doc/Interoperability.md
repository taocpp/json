# Interoperability

* [General Notes](#general-notes)
* [Specific Notes](#specific-notes)
  * [JSON](#json)
  * [JAXN](#jaxn)
  * [CBOR](#cbor)
  * [MsgPack](#msgpack)
  * [UBJSON](#ubjson)
* [Unsupported Formats](#unsupported-formats)

## General Notes

The following properties and limitations are a consequence of the internal data model and apply to the internal representation as well as all data formats.

Numbers are stored and handled as `std::int64_t`, `std::uint64_t` or `double`, and most (all?) parsers will throw an exception when encountering a number that exceeds the combined range of these types.

For binary formats, floating-point values are always written as 64-bit `double`.
Decoding 16- and 32bit floating-point values, where defined, is of course supported.

All string values, and, for text-based formats, the entire Value representation, have to be valid UTF-8.
The binary format parsers can optionally trust the input which disables UTF-8 validation of strings.

The Value Class uses a `std::map` for Objects and does not support multiple occurrences of the same key.
The Events Interface processes keys individually and does not have this limitation.

## Specific Notes

The following properties and limitations are specific to the interaction with particular file formats.

### JSON

When serialising to JSON, Values that go beyond the JSON data model, i.e. binary data and non-finite floating point values, must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.

### JAXN

* Work-in-progress...

### CBOR

CBOR allows arbitrary Values as keys in Objects, the data model of this library only supports strings.
The CBOR parsers throw an exception whey they encounter an Object key that is not a string.

Semantic tags are currently not supported, the CBOR parsers ignore and skip them.

The CBOR Parts Parser returns strings as `std::string`, rather than `tao::string_view`, in order to support multi-part indefinite length strings.
An alternative parser function `string_view()` only works for strings with a single top-level part and returns a `tao::string_view`.

### MsgPack

MsgPack only has Arrays and Objects that contain a count of the number of sub-values at the beginning.
The Events Consumers that encode MsgPack will throw an exception when either of the `begin_array()` or `begin_object()` functions is called without a size.

Extension types are currently not supported, the MsgPack parsers throw an exception when they encounter an extension type.

### UBJSON

* The No-Op value `N` is only supported as top-level padding.
* Does not currently encode strongly typed Arrays or Objects, except:
* Supports encoding and decoding of strongly typed Arrays of type `U` for binary data, and:
* Supports decoding of all Arrays and Objects, including with size and strongly typed.

## Unsupported Formats

The following JSON-esque formats are currently unsupported.

* BSON
* BJSON
* jsonb
* JSONx
* TOML
* YAML
* VelocyPack

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
