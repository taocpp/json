# Preliminary Documentation

This document gives a first introduction to design and usage of the taocpp/json library pending final changes for version 1.0.0.

## Concepts

The taocpp/json library is built around two main concepts, the *Value class* and the *Events interface*.

The Value class is an actual C++ class (or more precisely a class template with a traits class as template parameter) that implements the taocpp/json data model.

The Events interface is a calling convention, a kind of abstract (in the conceptual, not object-oriented sense) interface that allows different parts of the library to communicate with each other.

The Events interface is modelled after the SAX-interface for XML and has functions like `begin_object()` and `boolean( const bool )`.

We distinguish betwen *event producers* that usually take some input and generate calls to the Events interface from it, and *event consumers* which implement the functions of the Events interface to usually produce some output.
*Event filters* occupy both roles, accepting calls to the Events interface and transforming them into (other) calls to another Events interface implementation.

For example the `tao::json::events::from_value()` function is an Events producer that takes a Value object of type `tao::json::basic_value< traits >` as input, and generates Events depending on the contents.
It could be used together with the `tao::json::events::to_string()` class, an Events consumer that generates the JSON string representation corresponding to the Events it receives.
In order to transform binary data in the Value object into something compatible with standard JSON it might be necessary to use an Events filter like `tao::json::events::binary_to_hex` between the producer and consumer.

Producers and consumers can be freely coupled, it is not necessary for the beginning or end of the chain to be a value object.
For example an Events producer that parses CBOR could be used with an Events consumer that writes UBJSON in order to transform between these two formats on-the-fly.

## Data Model

The taocpp/json data model is mostly a superset of the data model implied by the JSON standard, with some important extensions.

(Note that JSON is a data interchange format does not explicitly define a data model; for example JSON numbers have no minimum or maximum precision.)

### Value

The Value-type can be considered the sum-type, or discriminated union, of all of the below types.

On the C++ level `Value` is a template class obtained by instantiating the class template `tao::json::basic_value<>` with the chosen traits class.

### Null

Models a type with a single value, `null`.

Null exists only on the type/meta-level; there is no C++ type corresponding to `null` values.

### Boolean

Models a type with two values, `true` and `false`.

Stored and passed around as C++ `bool`.

### Numbers

Numbers are restricted to the three types used for them, `double`, `int64_t` and `uint64_t`.

### Strings

Strings model sequences of Unicode code points.

Strings are stored as UTF-8 in `std::string` and `tao::string_view` objects, the latter of which is an alias for `std::string_view` when possible.

Checking for valid UTF-8, and (un)escaping according to the encoded representation like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

### Binary

Binary data models arbitrary byte sequences.

Binary data is stored in `std::vector< tao::byte >` and `tao::byte_view`, where `tao::byte` is an alias for `std::byte` when possible.

### Date-time

*coming soon*

### Arrays

Arrays model ordered sequences of any other value.

Arrays are stored as `std::vector< Value >`.

### Objects

Objects model ordered sequence of key-value pairs where they keys are strings.

Objects are stored as `std::map< std::string, Value >`.

## Value Class

## Events Interface

```c++
struct consumer
{
   void null();
   void boolean( const bool );
   void number( const double );
   void number( const std::int64_t );
   void number( const std::uint64_t );
   void string( std::string && );
   void string( const std::string & );
   void string( const tao::string_view & );
   void binary( std::vector< tao::byte > && );
   void binary( const std::vector< tao::byte > & );
   void binary( const tao::byte_view & );
   void begin_array();
   void begin_array( const std::size_t );
   void element();
   void end_array();
   void end_array( const std::size_t );
   void begin_object();
   void begin_object( const std::size_t );
   void key( std::string && );
   void key( const std::string & );
   void key( const tao::string_view & );
   void member();
   void end_object();
   void end_object( const std::size_t );
};
```

It will not always be necessary, desirable, or even possible, to implement all of these functions.

For example consider a consumer that writes some representation to a stream.
Such a consumer will not benefit from implementing the functions with movable arguments since it has no place to move the arguments to.
If the format it writes requires the encoding of objects and arrays to start with the number of array elements or object members, respectively, then the consumer might not implement the begin/end-array/object methods without size.

Similarly an Events producer will usually only either always call the begin/end-array-object functions with a size, or those without a size, depending on what is available in its input.

```c++
struct minimal_complete_consumer
{
   void null();
   void boolean( const bool );
   void number( const double );
   void number( const std::int64_t );
   void number( const std::uint64_t );
   void string( const tao::string_view & );
   void binary( const tao::byte_view & );
   void begin_array( const std::size_t = 0 );
   void element();
   void end_array( const std::size_t = 0 );
   void begin_object( const std::size_t = 0 );
   void key( const tao::string_view & );
   void member();
   void end_object( const std::size_t = 0 );
};
```
