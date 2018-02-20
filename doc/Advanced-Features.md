# Advanced Features

* [Custom Types and Traits](#custom-types-and-traits)
* [Raw and Opaque Pointers](#raw-and-opaque-pointers)

## Custom Types and Traits

*Please note that while everything documented here will continue to work in version 1.0, we are currently developing additional, simplified ways to write traits for custom types.*

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
const my_data d;  // ...assumed to contain important data...
const std::string json = tao::json::produce::to_string< my_traits >( d );
```

As with the `assign()` functions, it might be beneficial to provide a moving-version for strings and binary data (although only few Events consumers will be able to optimise for the move case).

### Positive Side Effects

Remember that the defaults traits implementation knows how to handle the most common smart pointers and containers.

As a consequence, given the traits specialisation for `my_data`, the following will automagically work, too, (as long as the default case of `my_traits< T >` derives from `tao::json::traits< T >`).

```c++
my_value v;  // ...assumed to contain the right data...
const auto p = v.as< std::unique_ptr< std::list< std::shared_ptr< my_data > > > >();
```

## Raw and Opaque Pointers

When creating a JSON Value instance with other JSON Values or other (custom) types as sub-values, the other JSON Values are "deep copied" into the target value, and the other types generate their corresponding JSON Value structure as per the type's traits' `assign()` method.

As a performance optimisation it is sometimes possible to replace these deep copied and generated structures of sub-values with pointers, and have the outside value behave "as if" it contained the deep copied or generated object hierarchy.

Both raw and opaque pointers use non-owning plain pointers for efficient object sharing.
It is up to the user to track object lifetimes and make sure that the objects pointed to be JSON Values with raw and opaque pointer sub-values are valid as long as they might be needed!

Both raw and opaque pointers MUST NOT be `nullptr`.

The difference between raw and opaque pointers is that raw pointers are used with other JSON Value instances (with the same traits and base class) while opaque pointers can be used with any data type for which the values's traits contain a `produce()` method.

Raw pointers are automatically dereferenced for comparison operations, however comparison operators MUST NOT be used with JSON Value instances that contain opaque pointers.

The main use case of opaque pointers is when creating a JSON Value structure in preparation of serialising to one of the supported representation formats.
Creating a value with a sub-value with a raw or opaque pointer, and then using one of the `to_string()` etc. functions, will yield the same result as creating a value with the full object hierarchy and serialising that, with the optmisation of not actually creating the full object hierarchy.

The function `tao::json::is_self_contained()` can be used to check whether a JSON Value is fully self contained, i.e. does not rely on any outside storage.
It recursively check the entire JSON Value and returns `false` if it finds any sub-value of type `STRING_VIEW`, `BINARY_VIEW`, `RAW_PTR` or `OPAQUE_PTR`.

The function `tao::json::make_self_contained()` can be used to make a JSON Value fully self contained.
Sub-values of type `STRING_VIEW` and `BINARY_VIEW` are transformed into `STRING` and `BINARY`, respectively.
Occurrences of `RAW_PTR` are replaced with a deep copy of the pointee.

Occurrences of `OPAQUE_PTR` are replaced with a JSON Value created by hooking up the `produce()` method of the pointee type's traits with an Events consumer that builds a Value from the Events.

We continue the `my_data` example from above, assuming that we are generating JSON text for a toy version of a JSON-formatted log file.

```c++
const my_data d;  // ...assumed to contain important data...
my_value v = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "data", d }
};
const auto json = tao::json::to_string( v );
```

The suggested way of creating JSON Values of type `OPAQUE_PTR` is to specialise the traits class for pointers.

```c++
template<>
struct my_traits< const my_data* >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( basic_value< Traits, Base >& v, const my_data* const d )
   {
      v.unsafe_assign_opaque_ptr( d );
   }
};
```

Remember that the `unsafe` in `unsafe_assign_opaque_ptr()` refers to it blindly assuming that `v` is not currently owning any memory, a safe assumption because traits' `assign()` functions are always called with default-initialised value instances as first argument.

Given this traits specialisation for the pointer to `my_data`, and the `produce()` function in the traits for `my_data` itself, we can replace the code from above with a more efficient version.

```c++
const my_data d;  // ...assumed to contain important data...
my_value v = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "data", &d }
};
const auto json = tao::json::to_string( v );
```

Note that only one detail has changed, we are passing `&d` instead of `d`, and that the resulting JSON text will be the same as before.

We do NOT recommend the following traits specialisation as it removes the `&` from all places that an opaque pointer is created, thereby removing the visual reminder that object lifetimes need to be tracked manually.

```c++
template<>
struct my_traits< my_data >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( basic_value< Traits, Base >& v, my_data& d )
   {
      v.unsafe_assign_opaque_ptr( &d );  // NOT recommended!
   }
};
```

## Custom Annotations

TODO: Explain base-class mechanism and its behaviour.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
