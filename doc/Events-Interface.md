# Events Interface

* [Overview](#overview)
* [Consumer Interface](#consumer-interface)
* [Arrays and Objects](#arrays-and-objects)
* [Reduced Interface](#reduced-interface)
* [Batteries Included](#batteries-included)

## Overview

The Events Interface is a calling convention, a kind of abstract (in the conceptual, not object-oriented sense) interface that allows different parts of the library to communicate with each other.

The Events Interface is modelled after the SAX-interface for XML and has functions like `begin_object()` and `boolean( const bool )`.

We distinguish between *Event Producers* that usually take some input and generate calls to the Events Interface from it, and *Event Consumers* which implement the functions of the Events Interface to usually produce some output.
*Event Transformers* occupy both roles, accepting calls to the Events Interface and transforming them into (other) calls to another Events Consumer.

For example the `tao::json::events::from_value()` function is an Events Producer that takes a Value object of type `tao::json::basic_value< Traits, Base >` for any Type Traits class `Traits` and Value base class `Base` as input, and generates Events depending on the contents.
It could be used together with the `tao::json::events::to_string()` class, an Events Consumer that generates the JSON string representation corresponding to the Events it receives.
In order to transform binary data in the Value object into something compatible with standard JSON it might be necessary to use an Events Transformer like `tao::json::events::binary_to_hex` between the Producer and Consumer.

Producers and Consumers can be freely coupled, it is not necessary for the beginning or the end of the chain to be a JSON Value object.
For example an Events Producer that parses CBOR can be directly used with an Events Consumer that creates UBJSON.

```c++
#include <tao/json.hpp>

std::string cbor_to_ubjson( const std::string& input )
{
   tao::json::ubjson::events::to_string consumer;
   tao::json::cbor::events::from_string( consumer, input );
   return consumer.value();
}
```

A more common use case is parsing a JSON text representation into a JSON Value...

```c++
tao::json::value from_string( const std::string& input )
{
   tao::json::events::to_value consumer;
   tao::json::events::from_string( consumer, input );
   return std::move( consumer.value );
}
```

...which is in fact so common that a (more general) convenience function `tao::json::from_string()` is included with the library.

The tables below list all included Events Producers, Consumers, and Transformers, as well as the convenience functions where the source or destination is a Value.

## Consumer Interface

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
   void binary( const tao::binary_view & );
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

## Arrays and Objects

For arrays and objects there are two versions of the begin and end methods, one that receives the size of the array (what `std::vector< Value >::size()` would return), or the size of the object (what `std::map< std::string, Value >::size()` would return), respectively, and one that doesn't.

An Events producer will usually only either always call the begin/end-array/object functions with a size, or those without a size, depending on whether the size is readily available.

Within an Array, i.e. after a call to `begin_array()` but before a call to the corresponding `end_array()`, the `element()` function is called after the function(s) for each array sub-value.

Similarly within an Object the sequence for every object entry is `key()`, then the function(s) for the value, then `member()`.

If the Value within an Array or Object is simple, like a string or number, it is defined by a single corresponding Events call.

However the data model allows for Arrays and Objects to be nested, wherefore the same holds for corresponding matching calls to an Event consumer's begin/end-array/object methods.
If a Value in an Array or Object is itself an Array or Object, then it will lead to at least the corresponding begin and end calls, with the possibility of arbitrary many sub-values, including arbitrarily nested other Arrays and Objects.

Calls to `element()` or `member()` are naturally understood to belong to the inner-most Array or Object, i.e. to the last begin-method without end.

An Events Producer MUST NOT generate inconsistent sequences of Event calls.
For example pairing `begin_array()` with `end_object()`, using `member()` when the inner-most container is not an Object, must all be avoided.

## Reduced Interface

It will not always be necessary, desirable, or even possible, to implement all of these functions.

For example consider a consumer that writes some representation to a stream.
Such a consumer will not benefit from implementing the functions with movable arguments since it has no place to move the arguments to.
If the format it writes requires the encoding of objects and arrays to start with the number of array elements or object members, respectively, then the consumer might not implement the begin/end-array/object methods without size.

An Events Consumer that does not need the sizes of arrays and objects in the begin/end-calls, and that does not benefit from receiving movable arguments, can implement a reduced interface that - thanks to default arguments and the view-classes' implicit constructors - can still accept all function calls that the full interface can.

```c++
struct reduced_consumer
{
   void null();
   void boolean( const bool );
   void number( const double );
   void number( const std::int64_t );
   void number( const std::uint64_t );
   void string( const tao::string_view );
   void binary( const tao::binary_view );
   void begin_array( const std::size_t = 0 );
   void element();
   void end_array( const std::size_t = 0 );
   void begin_object( const std::size_t = 0 );
   void key( const tao::string_view );
   void member();
   void end_object( const std::size_t = 0 );
};
```

## Batteries Included

* [Events Producers](Batteries-Included.md#events-producers)
* [Events Consumers](Batteries-Included.md#events-consumers)
* [Events Transformers](Batteries-Included.md#events-transformers)
* [Convenience Functions](Batteries-Included.md#convenience-functions)

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
