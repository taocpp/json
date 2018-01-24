# Preliminary Documentation

This document gives a first introduction to design and usage of the taocpp/json library pending final changes for version 1.0.0.

## Concepts

The taocpp/json library is built around two main concepts, the *value class* and the *Events interface*.

The value class is an actual C++ class (more precisely a class template with a traits class as template parameter) that implements the taocpp/json data model.

The Events interface is a calling convention, a kind of abstract (in the conceptual, not object-oriented sense) interface that allows different parts of the library to communicate with each other.

The Events interface is modelled after the SAX-interface for XML and has functions like `begin_object()` and `boolean( const bool )`.
We distinguish betwen *event producers* that usually take some input and generate calls to the Events interface from it, and *event consumers* which implement the functions of the Events interface to usually produce some output.
*Event filters* occupy both roles, accepting calls to the Events interface and transforming them into (other) calls to another Events interface implementation.

For example the `tao::json::events::from_value()` function is an events producer that takes a `tao::json::basic_value< traits >` as input and generates events depending on the contents of the value.
It could be used together with the `tao::json::events::to_string()` class, an events consumer that generates the JSON string representation corresponding to the events it receives.
In order to transform the binary data in the value object into something compatible with standard JSON it might be necessary to use an events filter like `tao::json::events::binary_to_hex` between the producer and consumer.

Producers and consumers can be freely coupled, it is not necessary for the beginning or end of the chain to be a value object.
For example an events producer that parses CBOR could be used with an events consumer that writes UBJSON in order to transform between these two formats on-the-fly.

## Data Model

The taocpp/json data model is mostly a superset of the data model implied by the JSON standard, with some important extensions.
(Note that JSON is a data interchange format does not explicitly define a data model; JSON numbers have no maximum precision.)

### Null

### Boolean

### Numbers

Numbers are restricted to the three types used for them, `double`, `int64_t` and `uint64_t`.

### Strings

Strings are UTF-8 encoded sequences of Unicode code points.

Strings are stored as `std::string` and `tao::string_view`, the latter of which is an alias for `std::string_view` when possible.
Checking for valid UTF-8, and (un)escaping according to the encoded representation like JSON, is performed "at the edges" by the respective parsers and encoders.

### Binary

Binary data can represent any sequence of bytes.

Binary data is stored in `std::vector< tao::byte >` and `tao::byte_view`, where `tao::byte` is an alias for `std::byte` when possible.

### Arrays



### Objects



## Value Class

## Events Interface

