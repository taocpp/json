# Overview

* [Classification](#classification)
* [Value Generic Parts](#value-generic-parts)
* [Custom Type Traits](#custom-type-traits)
* [Value Specific Parts](#value-specific-parts)
* [Hybrid Value Model](#hybrid-value-model)

TODO: Add links to relevant sections...

## Classification

JSON libraries for C++ are frequently of one of the following kinds.

1. Libraries with a value-generic in-memory representation based on standard containers that can easily read, write and modify any JSON value.
2. Libraries with a value-generic library-specific optimised in-memory representation that can easily and very quickly read and write any JSON value, but compromise on easy manipulation of in-memory values.
3. Libraries that directly use normal C++ data types as value-specific targets to very quickly read and write corresponding JSON values in a type-safe manner.

This library is two-in-one and is of both the first *and* the third kind.

It also supports conversions between arbitrary (custom) types and the generic JSON Value, and a hybrid model where a JSON Value can contain pointers to arbitrary (custom) types as sub-values.

## Value Generic Parts

At the heart of the value generic parts of this library are the [JSON Value class](Value-Class.md) that can store and manipulate arbitrary (extended) JSON data, and the [JSON Events interface](Events-Interface.md) that is the universal adapter that connects all value generic parts within the library (and can even bridge to other libraries).

The [Events interface](Events-Interface.md) is mainly used to convert the JSON Value class from and to all supported (or custom) JSON representation formats.
It can also be used to convert between formats on-the-fly, i.e. without creating a JSON Value to intermediately store the represented data, and it can modify, verify and filter arbitrary JSON data on-the-fly.

## Custom Type Traits

The [type traits](Type-Traits.md) mechanism allows the library to seamlessly interoperate with arbitrary (custom) C++ types, and the "binding" parts allow the creation of type traits for custom C++ classes with very little code.
Once the library know which data members a class has it can automatically synthesise all required traits functions at compile time.

A wide range of standard data types is supported out-of-the-box, including tuples, shared pointers, and the most frequently used standard library containers.
When greater control over the conversion between custom type and JSON Value or JSON Events is required, or when the behaviour for existing types needs to be changed, the traits mechanism allows for full manual control over all details.

## Value Specific Parts

The value specific parts of the library can be used to directly and efficiently convert all supported (or custom) JSON representation formats from and to instances of arbitrary (custom) types.
C++ classes can be easily encoded as Arrays or Objects, with control over whether empty data members are omitted, whether to throw an exception when decoding unknown Object fields, and which fields are required and which are optional.

The traits specialisations created by the "binding" parts fully support the value specific parts of this library.
The value specific parts also support advanced use cases like polymorphic object factories, inheritance of data members, and multiple encoding schemes for the same data type.

## Hybrid Value Model

The [hybrid value model](Advanced-Use-Cases.md#instance-sharing-with-opaque-pointers) allows for pointers to (custom) types to be embedded in JSON Value objects and is also supported by the automatically generated "binding" traits functions.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
