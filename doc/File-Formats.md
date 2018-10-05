# File Formats

The following properties and limitations are a consequence of the internal data model and apply to all data formats.

Numbers are stored and handled as `std::int64_t`, `std::uint64_t` or `double`, and most (all?) parsers will throw an error when encountering a number that will fit any of these types.

For binary formats, floating-point values are always written as 64-bit `double`.
Decoding 16- and 32bit floating-point values, where defined, is of course supported.

All string values, and, for text-based formats, the encoded representation, have to be valid UTF-8 (however the parsers for the binary formats have an option to trust that strings are valid UTF-8 which bypasses UTF-8 validation).

The Value class uses a `std::map` for Objects, wherefore multiple Object members with the same key are not supported.
The Events Interface processes keys individually and does not have this limitation.
Consequently parsing any data format into a Value will throw an exception when the same key is encountered the second time for the same Object, whereas duplicate keys are not an issue when directly using one data format's parser as Events Producer with another format's Events Consumer.

* [JSON](#json)
* [JAXN](#jaxn)
* [CBOR](#cbor)
* [MsgPack](#msgpack)
* [UBJSON](#ubjson)

## JSON

When serialising to JSON, Values that go beyond the JSON data model, i.e. binary data and non-finite floating point values, must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.

## JAXN

* Work-in-progress...

## CBOR

CBOR allows arbitrary Values as keys in Objects, the data model of this library only supports strings.
The CBOR parsers throw an exception whey they encounter an Object key that is not a string.

Semantic tags are currently not supported, the CBOR parsers ignore and skip them.

## MsgPack

MsgPack only has Arrays and Objects that contain a count of the number of sub-values at the beginning.
The Events Consumers that encode MsgPack will throw an exception when either of the `begin_array()` or `begin_object()` functions is called without a size.

Extension types are currently not supported, the MsgPack parsers throw an exception when they encounter an extension type.

## UBJSON

* The No-Op value `N` is only supported as top-level padding.
* Does not currently encode strongly typed Arrays or Objects, except:
* Supports encoding and decoding of strongly typed Arrays of type `U` for binary data, and:
* Supports decoding of all Arrays and Objects, including with size and strongly typed.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
