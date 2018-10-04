# File Formats

* [JSON](#json)
* [JAXN](#jaxn)
* [CBOR](#cbor)
* [MsgPack](#msgpack)
* [UBJSON](#ubjson)

## JSON

* Decoding a number that does not fit into `std::int64_t`, `std::uint64_t` or `double` is an error.
* When encoding, binary data must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.
* When encoding, non-finite floating-point values must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.

## JAXN

* This is work-in-progress?
* Decoding a number that does not fit into `std::int64_t`, `std::uint64_t` or `double`.

## CBOR

CBOR is supported within the data model this library.
Only strings are supported as keys in objects/maps.

Semantic tags are currently not supported.

### Major Type 7

The following table lists the supported Minors for Major type 7.

| Minor | Note |
|-------|------|
| 20 | Boolean `false`. |
| 21 | Boolean `true`. |
| 22 | Null value. |
| 25 | IEEE 754 half-precision float in following 16 bits. |
| 26 | IEEE 754 single-precision float in following 32 bits. |
| 27 | IEEE 754 double-precision float in following 64 bits. |
| 31 | Break code for indefinite-length containers. |

Note that 16 and 32-bit floating point numbers are correctly decoded, but, given the internal representation that exclusively uses `double`, are never encoded.

## MsgPack

MsgPack does not support open-ended Arrays or Objects, i.e. in MsgPack every Array and Object has to contain the number of elements, or members, respectively, at the beginning.
The Events Consumers that encode MsgPack will throw an exception when the `begin_array()` or `begin_object()` functions are called without a size.

Extension types are currently not supported.

## UBJSON

* The No-Op value `N` is only supported as top-level padding.
* Does not currently encode strongly typed Arrays or Objects, except:
* Supports encoding and decoding of strongly typed Arrays of type `U` for binary data.
* Supports decoding of all Arrays and Objects, including with size and strongly typed.
* Decoding a high-precision number that does not fit into `std::int64_t`, `std::uint64_t` or `double` is an error.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
