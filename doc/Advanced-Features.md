# Advanced Features

* [Custom Types and Traits](#custom-types-and-traits)
* [Raw and Opaque Pointers](#raw-and-opaque-pointers)

## Custom Types and Traits

**Please note that while everything documented here will continue to work in version 1.0, we are currently developing additional, simplified ways to write traits for custom types.**

The `Traits` template parameter used most prominently with `tao::json::basic_value<>` is used to control many aspects of the interaction between the JSON library and other C++ types.

It is both possible to change the traits for types like `int` for which the library already contains a default traits implementation, and to extend the traits to work with other (custom) types.

1. How to create a JSON Value from a (custom) type.
2. How to convert a JSON Value into a (custom) type.
3. How to compare a JSON Value to a (custom) type.
4. How to produce [Events](Events-Interface.md) from another type.

(Strictly speaking we should have written "How to create a JSON Value from *an instance of* a (custom) type", etc.
As is common we favour brevity and drop the distinction between a type and an instance of said type when the intended meaning is clear from the context.)

The usual way to add support for a custom data type, or change the behaviour of the existing traits, is to define a custom traits class that derives from the supplied traits `tao::json::traits<>` by default, and (partially) specialise it for some types as needed.

```c++
template< typename T >
struct my_traits
   : public tao::json::traits< T >
{
};

struct my_type
{
   std::string title;
   std::vector< int > ints;
};

template<>
struct my_traits< my_type >
{
   // See below...
};

using my_value = tao::json::basic_value< my_traits >;
```

Depending on the use cases that need to be changed or supported, one or more of the following functions need to be defined in a traits' specialisation.
Note that all traits functions are `static` member functions.

### From Type to JSON Value

The conversion from a type to a JSON Value is performed by the type's traits' `assign()` function.
The assign function receives a mutable reference to a default-initialised Value object, and (usually) a constant reference to the type in question.

For small types (`sizeof()` at most 1 or 2 pointers) that are cheap to copy the `assign()` function can take the second argument by value.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( basic_value< Traits, Base >& v, const my_type& d )
   {
      v = {
         { "title", d.title },
         { "ints", d.ints }
      };
   }
};
```

It is not necessary to provide (additional) traits for `std::string`, `int` and `std::vector< int >` because the supplied traits `tao::json::traits` already know how to handle these types.

It would however be possible to change e.g. the behaviour for `std::vector< int >` in case a different representation than the default JSON Array is desired.

In some cases it can be an optimisation to (also) provide a moving version of the `assign()` function even when (usually) the only data types that can be moved are strings and binary data.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( basic_value< Traits, Base >& v, my_type&& d )
   {
      v = {
         { "title", std::move( d.title ) },
         { "ints", d.ints }
      };
   }
};
```

In the example, `title` can be moved into the JSON Value since the JSON Value internally uses a `std::string` to store JSON strings.

For the integer vector it makes no difference because the internal representation is different, a vector of JSON Values.

### From JSON Value to Type

The conversion from a JSON Value back to a type is performed by the type's traits' `as()` function.

There are two possible signatures for the `as()` function of which *only one needs to be implemented*.
If not particularly awkward or slow it is recommended to implement the version that returns the data.

The user-facing functions `tao::json::as()` and `tao::json::basic_value<>::as()` are always available in both versions, regardless of which version the traits implement, subject to the following limitation:

* In order to provide the one-argument version when the traits implement only the two-argument form the type has to be default-constructible.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void as( const basic_value< Traits, Base >& v, my_type& d )
   {
      const auto& object = v.get_object();
      d.title = v.at( "title" ).as< std::string >();
      d.ints = v.at( "ints" ).as< std::vector< int > >();
   }

   template< template< typename... > class Traits, typename Base >
   static my_type as( const basic_value< Traits, Base >& v )
   {
      my_type result;
      const auto& object = v.get_object();
      result.title = v.at( "title" ).as< std::string >();
      result.ints = v.at( "ints" ).as< std::vector< int > >();
      return result;
   }
};
```

The used `get_object()`, `as<>()` and `at()` functions will all throw exceptions when something goes wrong, i.e. the types don't match or the accessed key doesn't exist.

Here no error is thrown when the top-level JSON Object contains additional key-value pairs beyond the `"title"` and `"ints"`.

### Compare JSON Value to Type

The JSON library includes comparison operators that can compare a JSON Value with any type provided that

1. the type can be converted into a JSON Value via the types' traits' `assign()` function, or
2. the types' traits' specialisation implements the following comparison functions.

Relying on the first possibility incurs the performance overhead of creating a temporary JSON Value for every comparison operation.

When implementing both possibilities, care should be taken to keep them consistent.

TODO: equal

TODO: less_than

TODO: greater_than

### Produce JSON Events from Type

The generation of JSON Events from a type is performed by the types' traits' `produce()` function.

Implementing the `produce()` function is only required to make use of some optimisation techniques.

1. Directly serialise any type to JSON or another supported file format.
2. Embed a plain pointer to the type in a JSON Value as [explained below](#raw-and-opaque-pointers).

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Consumer >
   static void produce( Consumer& c, const my_data& d )
   {
      c.begin_object( 2 );
      c.key( "title" );
      tao::json::produce< Traits >( c, d.title );
      c.member();
      c.key( "ints" );
      tao::json::produce< Traits >( c, d.ints );
      c.member();
      c.end_object( 2 );
   }
};
```

For the first use case, it is now trivial to directly and efficiently generate the JSON representation of a `my_type` instance without creating any JSON Value instance along the way.

```c++
const my_data d;
const std::string json = tao::json::produce::to_string< my_traits >( d );
```

As with the `assign()` functions, it might be beneficial to provide a moving-version for strings and binary data (although only few Events consumers will be able to optimise for the move case).

## Raw and Opaque Pointers

TODO: Explain lifetime issues.

TODO: Explain traits requirements.

TOOD: Give short example.

## Custom Annotations

TODO: Explain base-class mechanism and its behaviour.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
