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

Objects model ordered sequence of key-value pairs where the keys are strings.

Objects are stored as `std::map< std::string, Value >`.

## Value Class

The class `tao::json::value` implements the above data model, allowing for single Values to be stored in single C++ objects.

The member function `tao::json::value::type()` can be used to determine the type of Value stored in the object.
The return value is an enumerated value from the enum `tao::json::type`.

```c++
enum class type : std::uint8_t
{
   UNINITIALIZED,
   DISCARDED,
   DESTROYED,
   NULL_,
   BOOLEAN,
   SIGNED,
   UNSIGNED,
   DOUBLE,
   STRING,
   STRING_VIEW,
   BINARY,
   BINARY_VIEW,
   ARRAY,
   OBJECT,
   RAW_PTR
};
```

The set of types is larger than that defined by the data model, with the additional types allowing for greater efficiency and better error detection in some cases.

...

The type `RAW_PTR` indicates that the stored Value is a plain-and-simple C-pointer to another `tao::json::value`.
It is typically used when creating Arrays or Objects.
When adding a sub-value that is already available as instance of `tao::json::value` it is cheaper to add a `RAW_PTR` to the existing instance, rather than creating a copy, provided there are no issues of life-time.

```c++
   tao::json::value a = build_complex_value();
   tao::json::value b = {
      { "complex_value", & a },
      { "interesting_int", 42 }
   };
```

Here `b` will contain a `RAW_PTR` to `a` as value for the object key `"complex_value"`, and bad things will happen if `b` is used (in the "wrong" way) after `a` goes out of scope.

### Constructors

### Numbers

### Value Comparison

### The Traits Class

### Unsafe Functions

Some of the member functions of class `tao::json::basic_value<>` are also available in "unsafe" versions that, in the name of efficiency, make certain assumptions, or omit/make impossible certain checks.

All of the accessors like `get_boolean()` etc. have an unsafe version `unsafe_get_boolean()` that does not check whether the result of `type()` is the correct one.

## Events Interface

As noted, the Events interface is an interface in the colloquial sense, a calling convention, rather than an actual interface in the object-oriented sense.

*It should also be noted that nothing prevents the creation of an Events interface class in the object-oriented sense, one that has `virtual` methods corresponding to the Events functions.*

```c++
struct complete_consumer
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

### Arrays and Objects

For arrays and objects there are two versions of the begin and end methods, one that receives the size of the array (what `std::vector< Value >::size()` would return), or the size of the object (what `std::map< std::string, Value >::size()` would return), respectively, and one that doesn't.

An Events producer will usually only either always call the begin/end-array/object functions with a size, or those without a size, depending on whether the size is readily available.

Within an Array, i.e. after a call to `begin_array()` but before a call to the corresponding `end_array()`, the `element()` function is called after the function(s) for each array sub-value.

Similarly within an Object the sequence for every object entry is `key()`, then the function(s) for the value, then `member()`.

If the Value within an Array or Object is simple, like a string or number, it is defined by a single corresponding Events call.

However the data model allows for Arrays and Objects to be nested, wherefore the same holds for corresponding matching calls to an Event consumer's begin/end-array/object methods.
If a Value in an Array or Object is itself an Array or Object, then it will lead to at least the corresponding begin and end calls, with the possibility of arbitrary many sub-values, including arbitrarily nested other Arrays and Objects.

Calls to `element()` or `member()` are naturally understood to belong to the inner-most Array or Object, i.e. to the last begin-method without end.

An Events producer MUST NOT generate inconsistent sequences of Event calls.
For example pairing `begin_array()` with `end_object()`, using `member()` when the inner-most container is not an Object, or similar, must all be avoided.

### Reducing the interface

It will not always be necessary, desirable, or even possible, to implement all of these functions.

For example consider a consumer that writes some representation to a stream.
Such a consumer will not benefit from implementing the functions with movable arguments since it has no place to move the arguments to.
If the format it writes requires the encoding of objects and arrays to start with the number of array elements or object members, respectively, then the consumer might not implement the begin/end-array/object methods without size.

A consumer that does not need the sizes of arrays and objects in the begin/end-calls, and that does not benefit from receiving movable arguments, can implement a reduced interface that - thanks to default arguments and the view-classes' implicit constructors - can still accept all function calls that the full interface can.

```c++
struct reduced_consumer
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
