# File Formats

* [JSON](#JSON)
* [JAXN](#JAXN)
* [CBOR](#CBOR)
* [MsgPack](#MsgPack)
* [UBJSON](#UBJSON)

## JSON

* Decoding a number that does not fit into `std::int64_t`, `std::uint64_t` or `double` is an error.
* When encoding, binary data must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.
* When encoding, date-time values must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.
* When encoding, non-finite floating-point values must be [transformed](Events-Interface.md#included-transformers) to something compatible with JSON.

## JAXN

* This is work-in-progress?
* Decoding a number that does not fit into `std::int64_t`, `std::uint64_t` or `double`.

## CBOR

* Date-time support is coming soon...
* Most semantic tags are simply ignored.
* Only strings are supported as keys in objects/maps.

## MsgPack

* Timestamp support is coming soon...
* Most extension types are simply ignored.
* MsgPack does not support open-ended Arrays or Objects, i.e. without a leading count of elements or members, respectively, therefore the Events consumers that encode MsgPack will throw an exception when `begin_array()` or `begin_object()` is called without a size.

## UBJSON

* Does not currently encode strongly typed Arrays or Objects, except:
* Supports encoding and decoding of strongly typed Arrays of type `U` for binary data.
* Supports decoding of all Arrays and Objects, including with size or strongly typed.
* Decoding a high-precision number that does not fit into `std::int64_t`, `std::uint64_t` or `double` is an error.
* The No-Op value `N` is only supported as top-level padding.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
