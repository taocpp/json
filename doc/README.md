# Preliminary Documentation

This document gives a first introduction to design and usage of the taocpp/json library pending final changes for version 1.0.0.



## Concepts

The taocpp/json library is built around two main concepts, the *Value class* and the *Events interface*.

The Value class is an actual C++ class (or more precisely a class template with a traits class as template parameter) that implements the taocpp/json data model.

The Events interface is a calling convention, a kind of abstract (in the conceptual, not object-oriented sense) interface that allows different parts of the library to communicate with each other.

The Events interface is modelled after the SAX-interface for XML and has functions like `begin_object()` and `boolean( const bool )`.

We distinguish betwen *event producers* that usually take some input and generate calls to the Events interface from it, and *event consumers* which implement the functions of the Events interface to usually produce some output.
*Event filters* occupy both roles, accepting calls to the Events interface and transforming them into (other) calls to another Events interface implementation.

For example the `tao::json::events::from_value()` function is an Events producer that takes a Value object of type `tao::json::basic_value< Traits, Base >` for any traits class `Traits` and annotation class `Base` as input, and generates Events depending on the contents.
It could be used together with the `tao::json::events::to_string()` class, an Events consumer that generates the JSON string representation corresponding to the Events it receives.
In order to transform binary data in the Value object into something compatible with standard JSON it might be necessary to use an Events filter like `tao::json::events::binary_to_hex` between the producer and consumer.

Producers and consumers can be freely coupled, it is not necessary for the beginning or end of the chain to be a value object.
For example an Events producer that parses CBOR could be used with an Events consumer that writes UBJSON in order to transform between these two formats on-the-fly.



## Data Model

TODO: Reference JAXN data model when ready and remove this section?

The taocpp/json data model is mostly a superset of the data model implied by the JSON standard, with some important extensions.

(Note that JSON is a data interchange format does not explicitly define a data model; for example JSON numbers have no minimum or maximum precision.)

### Value

The Value-type can be considered the sum-type, or discriminated union, of all of the below types.

On the C++ level `Value` is a template class obtained by instantiating the class template `template< template< typename... > class Traits, typename Base > tao::json::basic_value< Traits, Base >` with the chosen traits class template, and annotation base class.

### Null

Models a type with a single value, `null`.

Null exists only on the type/meta-level; there is no C++ type corresponding to `null` values.

### Boolean

Models a type with two values, `true` and `false`.

Stored and passed around as C++ `bool`.

### Numbers

Numbers are restricted to the three types used for them, `double`, `std::int64_t` and `std::uint64_t`.

### Strings

Strings model sequences of Unicode code points.

Strings are stored as UTF-8 in `std::string` and `tao::string_view` objects, the latter of which is an alias for `std::string_view` when possible.

Checking for valid UTF-8, and (un)escaping according to the representation format like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

### Binary

Binary data models arbitrary byte sequences.

Binary data is stored in `std::vector< tao::byte >` and `tao::byte_view`, where `tao::byte` is an alias for `std::byte` when possible.

### Date-time

TODO: Document when implemented.

### Arrays

Arrays model ordered sequences of Values.

Arrays are stored as `std::vector< Value >`.

### Objects

Objects model ordered sequence of key-value pairs where the keys are strings.

Objects are stored as `std::map< std::string, Value >`.



## Value Class

The class `tao::json::value` implements the above data model, allowing for single Values to be stored in single C++ objects.

More precisely, `tao::json::value` is a specialisation of the class template `tao::json::basic_value` with the default `tao::json::traits`, and an empty base class.

The member function `tao::json::value::type()` can be used to determine the type of Value stored in the object.
The return value is an enumerated value from `tao::json::type`.

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

Please not that unlike some other JSON libraries, a default-initialised Value object will be of type `UNINITIALIZED` rather than `NULL_`!

The `explicit operator bool () const noexcept` that is part of the Value class returns whether the Value contains *anything*, including a `NULL_`.
In other words, it only returns `false` when its `type()` returns `type::UNITIALIZED`.

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

### Base Annotations

The Value class template `tao::json::basic_value< Traits, Base >` privately derives from its second template argument, allowing the addition of custom members to Value instances, including all sub-values.

The base class is always default-constructed, but fully preserved during copy/move construct and assignment operations from other (compatible) Value instances.
Access to the base class is provided by two getter-style functions, `Base& tao::json::basic_value< Traits, Base >::base() noexcept` and `const Base& bao::json::basic_value< Traits, Base >::base() const noexcept`.

The file `tao/include/json/contrib/position.hpp` shows how to parse a JSON file into a Value object where the Value, and all sub-values, are annotated with the filename and position in the file their respective definitions started at.

### The Traits Class

TODO: List everything customised via traits...



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

### Included Producers

Producers are (usually) functions that calls Event functions on a provided consumer.

* `tao::json::events::from_stream` reads JSON from a `std::istream` and generates corresponding Events.
* `tao::json::events::from_string` reads JSON from a `std::string` and generates corresponding Events.
* `tao::json::events::from_value` calls Events corresponding to a `tao::json::basic_value<>` on the provided consumer.
* `tao::json::events::parse_file` reads JSON from the file provided as filename and generates corresponding Events.

* `tao::json::events::jaxn::from_stream` reads JAXN from a `std::istream` and generates corresponding Events.
* `tao::json::events::jaxn::from_string` reads JAXN from a `std::string` and generates corresponding Events.
* `tao::json::events::jaxn::parse_file` reads JAXN from the file provided as filename and generates corresponding Events.

* `tao::json::events::cbor::from_string` reads CBOR from a `std::string` and generates corresponding Events.
* `tao::json::events::cbor::parse_file` reads CBOR from the file provided as filename and generates corresponding Events.

* `tao::json::events::msgpack::from_string` reads MSGPACK from a `std::string` and generates corresponding Events.
* `tao::json::events::msgpack::parse_file` reads MSGPACK from the file provided as filename and generates corresponding Events.

* `tao::json::events::ubjson::from_string` reads UBJSON from a `std::string` and generates corresponding Events.
* `tao::json::events::ubjson::parse_file` reads UBJSON from the file provided as filename and generates corresponding Events.

### Convenience Wrappers

Functions that combine the similarly named Events producer with a consumer that "writes" a Value.

* `tao::json::from_stream` reads JSON from a `std::istream` and returns a Value.
* `tao::json::from_string` reads JSON from a `std::string` and returns a Value.
* `tao::json::parse_file` reads JSON from the file provided as filename and returns a Value.

* `tao::json::jaxn::from_stream` reads JAXN from a `std::istream` and returns a Value.
* `tao::json::jaxn::from_string` reads JAXN from a `std::string` and returns a Value.
* `tao::json::jaxn::parse_file` reads JAXN from the file provided as filename and returns a Value.

* `tao::json::cbor::from_string` reads CBOR from a `std::string` and returns a Value.
* `tao::json::cbor::parse_file` reads CBOR from the file provided as filename and returns a Value.

* `tao::json::msgpack::from_string` reads MSGPACK from a `std::string` and returns a Value.
* `tao::json::msgpack::parse_file` reads MSGPACK from the file provided as filename and returns a Value.

* `tao::json::ubjson::from_string` reads UBJSON from a `std::string` and returns a Value.
* `tao::json::ubjson::parse_file` reads UBJSON from the file provided as filename and returns a Value.

### Included Consumers

Consumers are structs or classes that implement the Events functions.

* `tao::json::events::debug` writes all Events to a `std::ostream` in human-readable form.
* `tao::json::events::discard` does nothing with the Events it receives.
* `tao::json::events::hash` calculates a SHA-256 hash for the received Events.
* `tao::json::events::to_pretty_stream` writes nicely formatted JSON to a `std::ostream`.
* `tao::json::events::to_stream` writes compact JSON to a `std::ostream`.
* `tao::json::events::to_string` writes compact JSON to a `std::string`.
* `tao::json::events::to_value` builds up a Value with the received Events.
* `tao::json::events::validate_event_order` checks for a consistent order of received Events (e.g. no `end_array()` after `begin_object()`).

* `tao::json::events::jaxn::to_pretty_stream` writes nicely formatted JAXN to a `std::ostream`.
* `tao::json::events::jaxn::to_stream` writes compact JAXN to a `std::ostream`.
* `tao::json::events::jaxn::to_string` writes compact JAXN to a `std::string`.

* `tao::json::events::cbor::to_stream` writes CBOR to a `std::ostream`.
* `tao::json::events::cbor::to_string` writes CBOR to a `std::string`.

* `tao::json::events::msgpack::to_stream` writes MSGPACK to a `std::ostream`.
* `tao::json::events::msgpack::to_string` writes MSGPACK to a `std::string`.

* `tao::json::events::ubjson::to_stream` writes UBJSON to a `std::ostream`.
* `tao::json::events::ubjson::to_string` writes UBJSON to a `std::string`.

### Convenience Wrappers

Functions that combine the similarly named Events consumers with a producer that "reads" from a Value (optionally applying an arbitrary list of Filters given as additional template parameters).

* `tao::json::to_stream` writes a Value as compact or nicely formatted JSON to a `std::ostream`.
* `tao::json::to_string` writes a Value as compact or nicely formatted JSON to a `std::string`.

* `tao::json::jaxn::to_stream` writes a Value as compact or nicely formatted JAXN to a `std::ostream`.
* `tao::json::jaxn::to_string` writes a Value as compact or nicely formatted JAXN to a `std::string`.

* `tao::json::jaxn::to_stream` writes a Value as CBOR to a `std::ostream`.
* `tao::json::jaxn::to_string` writes a Value as CBOR to a `std::string`.

* `tao::json::msgpack::to_stream` writes a Value as MSGPACK to a `std::ostream`.
* `tao::json::msgpack::to_string` writes a Value as MSGPACK to a `std::string`.

* `tao::json::ubjson::to_stream` writes a Value as UBJSON to a `std::ostream`.
* `tao::json::ubjson::to_string` writes a Value as UBJSON to a `std::string`.

### Included Filters

Filters are structs or classes that implement the Events functions as they are consumers that then produce events for another consumer.

* `tao::json::events::binary_to_base64` passes through all Events except for binary data which is converted to base64-encoded strings.
* `tao::json::events::binary_to_base64url` passes through all Events except for binary data which is converted to URL-safe base64-encoded strings.
* `tao::json::events::binary_to_exception` passes through all Events except for binary data which provokes an exception.
* `tao::json::events::binary_to_hex` passes through all Events except for binary data which is is converted to hex-dumped strings.
* `tao::json::events::key_camel_case_to_snake_case` passes through all Events except for keys in Objects which are converted from "CamelCaseStyle" to "snake_case_style".
* `tao::json::events::key_snake_case_to_camel_case` passes through all Events except for keys in Objects which are converted from "snake_case_style" to "CamelCaseStyle".
* `tao::json::events::non_finite_to_exception` passes through all Events except for numbers of type `double` which contain non-finite values which provoke an exception.
* `tao::json::events::non_finite_to_null` passes through all Events except for numbers of type `double` which contain non-finite values which are passed on as `null()`.
* `tao::json::events::non_finite_to_string` passes through all Events except for numbers of type `double` which contain non-finite values which are passed on as appropriate `string()`.
* `tao::json::events::prefer_signed` passes through all Events except for numbers of type `std::uint64_t` which fit into a `std::int64_t` and are passed on as such.
* `tao::json::events::prefer_unsigned` passes through all Events except for numbers of type `std::int64_t` which fit into a `std::uint64_t` and are passed on as such.
* `tao::json::events::ref` passes all Events to another consumer or filter to which it holds a C++ reference.
* `tao::json::events::tee` passes all Events to an arbitrary number of other consumers or filters which it holds in a `std::tuple<>`.
* `tao::json::events::validate_keys` passes through all Events except for keys which are first validated against a provided PEGTL grammar rule.

### Need Description

* `tao::json::events::basic_compare`
* `tao::json::events::compare`
* `tao::json::events::transformer`
* `tao::json::nlohmann::from_value`
* `tao::json::nlohmann::to_value`
