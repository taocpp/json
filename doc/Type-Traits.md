# Type Traits

* [Overview](#overview)
* [Binding Traits Facilities](#binding-traits-facilities)
* [Create JSON Value from (custom) type](#create-value-from-type)
* [Convert JSON Value into (custom) type](#convert-value-into-type)
* [Compare JSON Value to (custom) type](#compare-value-with-type)
* [Produce JSON Events from (custom) type](#produce-events-from-type)
* [Consume a (custom) type from a parser](#consume-type-from-parser)
* [Default Traits Specialisations](#default-traits-specialisations)
* [Contrib Traits Specialisations](#contrib-traits-specialisations)
* [Default Key for Objects](#default-key-for-objects)

For brevity we will often write "the traits" instead of "the (corresponding/appropriate/whatever) specialisation of the traits class template".

## Overview

The class template passed as `Traits` template parameter, most prominently to `tao::json::basic_value<>`, controls the interaction between the JSON library and other C++ types.

The library includes the Type Traits class template `tao::json::traits<>` with [many specialisations](#default-traits) that is used throughout the library as default.
A custom Type Traits class can be used to change the behaviour of the default traits, and/or to add support for new types.

* In the first case it is necessary to create a new traits class template.
* In the second case it is also possible to (partially) specialise `tao::json::traits<>` for the new types.

As is common, for any type `T`, the Type Traits class template instantiated with `T` as template argument is used as Type Traits class for `T`.

The default Type Traits use a second template parameter that defaults to `void` to selective enable or disable a particular (partial) specialisation via SFINAE.
For this reason, any template that takes a Type Traits class template as template parameter should be templated over `template< typename... > class Traits`, rather than the more obvious `template< typename > class Traits`.

We will use the following class in the example implementations of the Type Traits functions.

```c++
struct my_type
{
   std::string title;
   std::vector< int > values;
};
```

While we could simply specialise `tao::json::traits<>` for `my_type`, we will prefer to define a new Type Traits class, an approach that also works when redefining traits for types that the default Type Traits already cover.

```c++
template< typename T >
struct my_traits
   : public tao::json::traits< T >
{
   // Inherit the default traits by default,
   // otherwise we would have to implement
   // specialisations for ALL types (that we
   // want to use).
};
```

For convenience, we might add a `using` for the JSON Value class with the new Type Traits.

```c++
using my_value = tao::json::basic_value< my_traits >;
```

Or, if we are planning on using [custom Base class for Values](Advanced-Use-Cases.md#custom-base-class-for-values), we might be more elaborate.

```c++
template< typename Base >
using my_basic_value = tao::json::basic_value< my_traits, Base >;
using my_value = tao::json::basic_value< my_traits >;
```

Note that all Type Traits functions are `static` member functions, and that, depending on the use cases, it is not necessary for a traits specialisation to implement *all* traits functions.

For the common use case of implementing Type Traits for a custom struct or class that uses an Array or an Object for its data members the [Binding Facilities](#binding-traits-facilities) can be used to *automatically* create the Type Traits functions without writing any actual code.

## Binding Traits Facilities

The Binding Facilities allow the automatic generation of Type Traits functions for a type from a list of the type's data members.

It requires that Type Traits specialisations for all of the data member types exist, too, either manually implemented, inherited from the [default Type Traits](#default-traits), or again generated with the Binding Facilities.

There are two kinds of Binding, one that uses an Array for the class or struct members, and one that uses an Object.

The Array is more efficient, the Object is more flexible as it allows for optional members, and possible forward and/or backward compatibility with future extensions.
Given that members are named rather than accessed by index, Objects are also easier to comprehend and debug.

To use an Object it is necessary to derive `my_traits< my_type >` from `tao::json::binding::object`, and to supply a list of member variable pointers with their name.
It is not technically necessary for the name to match the name of the data member, any string can be used for the key in the Object.

Two macros are used to simplify the binding of the individual member variables.
When directly parsing an external representation like JSON into a `my_type` with `tao::json::consume()`, or converting a Value into a `my_type` with `tao::json::value::as()` or `tao::json::value::to()`, an exception will be thrown when a required member is missing.

```c++
template<>
struct my_traits< my_type >
   : tao::json::binding::object< TAO_JSON_BIND_REQUIRED( "title", &my_type::title ),
                                 TAO_JSON_BIND_OPTIONAL( "values", &my_type::values ) >
{
};
```

To use an Array it is necessary to derive `my_traits< my_type >` from `tao::json::binding::array`, and again supply a list of pointers to member variables.
Here neither a name can be given, nor is there a choice between optional and required values - everything is required, converting from an Array expects exactly the correct number of Array elements.


```c++
template<>
struct my_traits< my_type >
   : tao::json::binding::array< TAO_JSON_BIND_ELEMENT( &my_type::title ),
                                TAO_JSON_BIND_ELEMENT( &my_type::values ) >
{
};
```

As usual it is possible to override some of the functions inherited from `tao::json::binding::object` or `tao::json::binding::array` by defining them in `my_traits< my_type >`.

Regardless of the choice of Array or Object, all of the Type Traits functions (current limitation: except for `less_than()` and `greater_than()`) are automatically generated for the Type Traits of `my_type` :smile:

The examples given below of how to use the individual Type Traits functions all work without manually implementing the underlying functions of the Type Traits specialisation.

The binding facilities also support [some more advanced options](Advanced-Use-Cases.md#advanced-binding-options), a [polymorphic object factory](Advanced-Use-Cases.md#polymorphic-object-factory), and [multiple bindings for the same data type](Advanced-Use-Cases.md#disjunction-of-single-type-traits) for multi-version support.

## Create Value from Type

The traits' `assign()` functions are used to create Value instances from any type `T`.
The first argument is always a default-initialised `tao::json::basic_value<>` that reports `tao::json::UNINITIALIZED` as type, wherefore it is safe to call the `unsafe_assign_foo()` methods on it.

We will use the implementation of `assign()` in the specialisation of `my_traits` for `my_type` as example.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( tao::json::basic_value< Traits, Base >& v, const my_type& t ) noexcept
   {
      v = {
         { "title", t.title },
         { "values", t.values }
      };
   }
};
```

The first thing to note is that `assign()` is "templated over" the traits class template.
If it weren't templated over the traits, the `assign()` function would only work with `my_traits`, but not when using a third traits class template that inherits from `my_traits` by default without changing the specialisation for `my_type`.

For greater flexibility and future compatibility, we recommend to *always* template over the traits, regardless of whether creating other traits class templates based on the traits in question is planned or not.

The second point of interest is that we have chosen to encode `my_type` as JSON Object with two sub-values, one for the title and one for the values, using the [initialiser-list syntax for Objects as explained in the documentation of the Value class](Value-Class.md).

Nothing more needs to be done for `t.title` and `t.values` since the default traits already know how to create Values from `std::string`, `std::vector<>`, and `int`, and therefore also `std::vector< int >`.
The default traits use JSON Arrays for vectors, lists and sets.

Given the above traits specialisation for `my_type` it is now possible to write all of the following.

```c++
const my_type a = make_my_type();
const my_value v1 = a;
const my_value v2 = {
   { "it's my type", a }
};

const std::list< my_type > l = make_list_of_my_type();
const my_value v3 = l;
```

As a performance optimisation it is possible to provide an additional overload of `assign()` for the case that the source `my_type` instance is `move()`-able.
The moving `assign()` is of course only beneficial when there is data that can actually be moved, like `title` in the example.
The vector `values` can not be moved to the JSON Value since it is a `std::vector< int >`, not one of the types used to represent data in a JSON Value.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void assign( tao::json::basic_value< Traits, Base >& v, my_type&& t ) noexcept
   {
      v = {
         { "title", std::move( t.title ) },
         { "values", t.values }
      };
   }
};
```

There are of course other possibilities for the JSON structure for `my_type`, for example it would be possible to create a flat JSON Array with the title as first element followed by the integers from `values`.
In the following we will assume the implementation from above.

## Convert Value into Type

The traits' `as()` and/or `to()` functions are used to convert Values into any type `T`.

If not particularly awkward or slow it is recommended to implement `as()`, which returns the `T`, rather than `to()`.

The user-facing functions `tao::json::basic_value<>::as()/to()` are all available, regardless of which function(s) the traits implement, subject to the following limitation:

* `T` needs to be copy-assignable to use the front-end `to()` when the traits only implement `as()`.
* `T` needs to be default-constructible to use the front-end `as()` when the traits only implement `to()`.

Here both functions are shown even though a real-world traits class will typically only implement either one of them.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static void to( const tao::json::basic_value< Traits, Base >& v, my_type& d )
   {
      const auto& object = v.get_object();
      d.title = v.at( "title" ).template as< std::string >();
      d.values = v.at( "values" ).template as< std::vector< int > >();
   }

   template< template< typename... > class Traits, typename Base >
   static my_type as( const tao::json::basic_value< Traits, Base >& v )
   {
      my_type result;
      const auto& object = v.get_object();
      result.title = v.at( "title" ).template as< std::string >();
      result.values = v.at( "values" ).template as< std::vector< int > >();
      return result;
   }
};
```

The `as<>()` and `to()` functions again template over the traits class, just like `assign()`, and for the same reasons.

The employed `get_object()`, `as<>()`, `to<>()` and `at()` functions will all throw an exception when something goes wrong, i.e. when the accessed JSON Value is not of the correct type, or when the indexed key does not exist.

Instead of manually calling the Traits' `as()`- and `to()`-functions it is recommended to use one of the following to create a `T` from a Value.

```c++
const tao::json::value v = ...;
const my_type mt = v.as< my_type >();
```

Or alternatively:

```c++
const tao::json::value v = ...;
my_type mt;
v.to( mt );
```

In this example no error is thrown when the top-level JSON Object contains additional keys beyond `"title"` and `"values"`.

## Compare Value with Type

The comparison operators `==`, `!=`, `<`, `<=`, `>` and `>=` in namespace `tao::json` that compare instances of `basic_value<>` with other types *ideally* use the traits' `equal()`, `greater_than()` and `less_than()` functions.

That is "ideally", because as long as the traits for the type in question have an `assign()` function, the comparison operators will still work -- by creating a temporary JSON Value and then compare the two Values.
In other words, adding `equal()`, `greater_than()` and `less_than()` functions to traits is "only" a performance optimisation, it prevents the creation of the temporary JSON Value from the non-Value argument.

The `equal()` function has to check whether an instance `d` of the type for which the traits are specialised is equal to a Value.
This check should be consistent with the other traits functions, i.e. the check should return `true` if and only if comparing the Value to a Value created with the traits' `assign()` function (or the traits' `produce()` function together with `tao::json::events::to_value`) would also return `true`.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Base >
   static bool equal( const tao::json::basic_value< Traits, Base >& v, const my_type& d ) noexcept
   {
      if( !v.is_object() ) {
         return false;
      }
      const auto& o = v.unsafe_get_object();
      const auto i = o.find( "title" );
      const auto j = o.find( "values" );
      return ( o.size() == 2 )
          && ( i != o.end() )
          && ( i->second == d.title )
          && ( j != o.end() )
          && ( j->second == d.values );
   }
};
```

The other two functions, `less_than()` and `greater_than()`, have the same signature, and need to return whether the first argument is less than, or greater than the second, respectively.
The same consistency conditions as for `equal()` should be applied.
When all traits functions are consistent which each other then the following assertions will never fail, regardless of the values of `m` and `d`.

```c++
const my_value m = some_value();

const my_type d = make_my_type();
const my_value v = d;

assert( ( d == m ) == ( v == m ) );
assert( ( d != m ) == ( v != m ) );
assert( ( d < m ) == ( v < m ) );
assert( ( d <= m ) == ( v <= m ) );
assert( ( d > m ) == ( v > m ) );
assert( ( d >= m ) == ( v >= m ) );

// When the traits have a produce() function:

const auto e = tao::json::produce::to_value< my_traits >( d );

assert( ( d == m ) == ( e == m ) );
assert( ( d != m ) == ( e != m ) );
assert( ( d < m ) == ( e < m ) );
assert( ( d <= m ) == ( e <= m ) );
assert( ( d > m ) == ( e > m ) );
assert( ( d >= m ) == ( e >= m ) );
```

## Produce Events from Type

Producing [JSON Events](Events-Interface.md) from a type is performed by the type's Traits' `produce()` function.

Implementing the `produce()` function is only required for some optimisation techniques, namely:

1. To directly serialise any type to JSON or another supported external representation format.
2. To create an [Opaque pointer JSON Value](Advanced-Use-Cases.md#instance-sharing-with-opaque-pointers) instead of using building the JSON data structure for a type.

The `produce()` function receives an arbitrary [Events Consumer](Events-Interface.md) as first argument, and can call any [Events Functions](Events-Interface.md) on it.

It should/must again template over their traits, and, since there is no `basic_value<>` instance from which the traits can be derived, the traits `produce()` functions and the functions in namespace `tao::json::produce` need to be called with the traits class template as explicit template parameter.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Consumer >
   static void produce( Consumer& c, const my_type& d )
   {
      c.begin_object( 2 );
      c.key( "title" );
      tao::json::produce< Traits >( c, d.title );
      c.member();
      c.key( "values" );
      tao::json::produce< Traits >( c, d.values );
      c.member();
      c.end_object( 2 );
   }
};
```

For the first use case mentioned above, the following code snippet directly and efficiently generates the JSON representation of a `my_type` instance without creating any JSON Values along the way.

```c++
const my_type d = make_my_type();
const std::string json = tao::json::produce::to_string< my_traits >( d );
```

As with the `assign()` functions, and depending on which Events Consumers are used, it might be beneficial to provide a moving-version of `produce()` that moves strings and binary data.

## Consume Type from Parser

Directly creating an instance of a type from a parser is performed by the type's Traits' `consume()` function.

Unlike all other similar parts of the library the `consume()` interface is *not* based on the Events Interface!

Instead ... TODO: Explain pull-interface of the parts parsers.

Similar to the Traits' `as()`-function there are two possibilities on how to implement this function, one that returns the value, and one that takes a mutable reference as second argument.
Again it is not *necessary* to implement both functions, either one is sufficient, and when all is equal preference should be given to the version that returns the value.

```c++
template<>
struct my_traits< my_type >
{
   template< template< typename... > class Traits, typename Producer >
   static void consume( Producer& p, my_type& mt )
   {
      // Easier to use the Binding Facilities to generate this function...
   }

   template< template< typename... > class Traits, typename Producer >
   static my_type consume( Producer& p )
   {
      // ...and not implement this one.
   }
};
```

Creating instances of `my_type` can then be achieved by manually calling the Traits' `consume()` function, or more conveniently by using one of the global `consume()` functions.
Both the one- and two-argument version of the global `consume()` function can be called regardless of which version the Traits implement.

```c++
const std::string json = ...;
tao::json::parts_parser pp( json, "source" );
const my_type mt = tao::json::consume< my_type, Traits >( pp );
```

Or alternatively:

```c++
const std::string json = ...;
tao::json::parts_parser pp( json, "source" );
my_type mt;
tao::json::consume< Traits>( pp, mt );
```

## Default Traits Specialisations

The included Type Traits contain (partial) specialisations for the following types.

| Specialised for | Remarks |
| -------------- | -------- |
| `null_t` | |
| `bool` | |
| *signed integers* | |
| *unsigned integers* | |
| `double`, `float` | |
| `empty_binary_t` | |
| `empty_array_t` | |
| `empty_object_t` | |
| `std::string` | |
| `tao::string_view` | |
| `const char*` | |
| `const std::string&` | |
| `std::vector< tao::byte >` | |
| `tao::binary_view` | |
| `const std::vector< tao::byte >&` | |
| `std::vector< basic_value< Traits, Base > >` | Corresponds to JSON Array. |
| `basic_value< Traits, Base >*` | Creates Value Pointer; must not be `nullptr`. |
| `const basic_value< Traits, Base >*` | Creates Value Pointer; must not be `nullptr`. |
| `std::map< std::string, basic_value< Traits, Base > >` | Corresponds to JSON Object. |
| `tao::optional< T >` | Empty optional corresponds to JSON Null. |

## Contrib Traits Specialisations

The following additional specialisations are part of various additional traits classes.
These classes are defined in various headers in `tao/json/contrib`.
It is also possible to include `tao/json/contrib/traits.hpp` which adds appropriate specialisations to the default traits `tao::json::traits<>`.

| Implementation for | Header |
| -------------- | -------- |
| `std::pair< U, V >` | `tao/json/contrib/pair_traits.hpp` |
| `std::tuple< Ts... >` | `tao/json/contrib/tuple_traits.hpp` |
| `std::array< T, N >` | `tao/json/contrib/array_traits.hpp` |
| `std::deque< T >` | `tao/json/contrib/deque_traits.hpp` |
| `std::list< T >` | `tao/json/contrib/list_traits.hpp` |
| `std::set< T >` | `tao/json/contrib/set_traits.hpp` |
| `std::unordered_set< T >` | `tao/json/contrib/unordered_set_traits.hpp` |
| `std::vector< T >` | `tao/json/contrib/vector_traits.hpp` |
| `std::map< std::string, T >` | `tao/json/contrib/map_traits.hpp` |
| `std::unordered_map< std::string, T >` | `tao/json/contrib/unordered_map_traits.hpp` |
| `std::shared_ptr< T >` | `tao/json/contrib/shared_ptr_traits.hpp` |
| `std::unique_ptr< T >` | `tao/json/contrib/unique_ptr_traits.hpp` |

The Type Traits correctly work with nested types.
Given that `std::string`, `int`, `std::tuple`, `std::vector`, `std::shared_ptr`, and `std::map` with `std::string` as `key_type` are supported (when including the contrib traits), so is for example the following type:

```c++
std::map< std::string, std::shared_ptr< std::vector< std::tuple< int, int, int > > >
```

## Default Key for Objects

The use of default keys is [shown in the section on creating Values](Value-Class.md#creating-values).

The default key for a type is a compile-time string that needs to be declared in the Traits specialisation.

```c++
template<>
struct my_traits< my_type >
{
   TAO_JSON_DEFAULT_KEY( "fraggle" );
};
```

Further uses of the default key are for object bindings and the polymorphic object factory (TODO: Links).

The default Traits supplied with the library do not define a default key for any type.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
