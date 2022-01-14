# Binding Traits

The Binding Traits facilities are used to "bind" existing [Type Traits](Type-Traits.md) together to create [Type Traits](Type-Traits.md) for new types or with new features.

They thereby offer rather convenient ways of automatically creating Traits functions for many common use cases.

* [Types as Arrays](#types-as-arrays)
* [Types as Objects](#types-as-objects)
* [Polymorphic Factory](#polymorphic-factory)
* [Multiple Versions](#multiple-versions)

## Types as Arrays

The class template `binding::array` can be used to easily create Traits for types that are to be represented by Arrays.

It takes arbitrary many template arguments that are usually created by invoking the macro `TAO_JSON_BIND_ELEMENT` with a member object pointer, i.e. a pointer to a member object of the type for which the Traits are to be created.

For example consider the following hypothetical `my_traits` specialisation for `std::pair`, similar to the Traits defined in `tao/json/contrib/pair_traits.hpp`.

```c++
#include <utility>
#include <tao/json.hpp>

template< typename T >
struct my_traits;

template< typename U, typename V >
struct my_traits< std::pair< U, V > >
   : public tao::json::binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                                       TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >
{
};
```

Note that variadic macros are used to correctly pass the member pointer even though the type `std::pair< U, V >` contains a comma.

The `assign()` and `produce()` functions of these Traits for `std::pair` will always create Arrays with two elements, and conversely the `to()` and `consume()` functions will expect Arrays with two elements.

Handling of the two member objects `first` and `second` is delegated to the Traits' specialisations for their respective types.

The order in the serialisation will always correspond to the order of the template parameter to `binding::array` (which need not coincide with the order in the `class` or `struct`).

### Constants

It is also possible to add constant elements that are purely defined in the Traits.

These constants are currently limited to boolean values, signed and unsigned integers, and strings.

When generating an encoding, the constants behave "as if" there was a binding to a variable of the corresponding type with the predefined value.

When parsing an encoding, the constants verify that the value read from the input is equal to their predefined value.

Constants are added to an Array binding with the macros `TAO_JSON_BIND_ELEMENT_BOOL()`, `TAO_JSON_BIND_ELEMENT_SIGNED()`, `TAO_JSON_BIND_ELEMENT_UNSIGNED()` and `TAO_JSON_BIND_ELEMENT_STRING()`.

```c++
template< typename U, typename V >
struct my_traits< std::pair< U, V > >
   : public tao::json::binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                                       TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ),
                                       TAO_JSON_BIND_ELEMENT_UNSIGNED( 42 ), >
                                       TAO_JSON_BIND_ELEMENT_STRING( "hallo" ) >
{
};

const auto p = std::pair( 1, 2 );
const tao::json::basic_value< my_traits > v = p;
// v is [ 1, 2, 42, "hallo" ]
```

### Inheritance

When a derived class' Array-based binding is supposed to extend the base class' binding it is possible to indicate the base class to an Array binding in order to not repeat the bindings for the inherited member objects.

For example consider the following class that extends a pair to a triple (instead of using a `std::tuple`).

```c++
template< typename U, typename V, typename W >
struct triple
   : std::pair< U, V >
{
   W third;
};
```

The corresponding specialisation of the hypothetical `my_traits` can then be implemented as follows.

```c++
template< typename U, typename V, typename W >
struct my_traits< triple< U, V, W > >
   : public tao::json::binding::array< tao::json::binding::inherits< my_traits< std::pair< U, V > > >,
                                       TAO_JSON_BIND_ELEMENT( &triple< U, V, W >::third ) >
{
};
```

Note that while the example shows what is probably the most common use case with the `binding::inherits` as first template argumen to `binding::array`, it can be used anywhere, i.e. before, in the middle of, or after any occurrences of `TAO_JSON_BIND_ELEMENT`.

### Indirect

TODO: `element` with getter, and `element2` with getter and setter.

## Types as Objects

The class template `binding::object` can be used to easily create Traits for types that are to be represented by Objects.

Compared to the Array binding explained above the Object binding has the advantages of being more expressive and flexible, and the disadvantage of being less efficient, both in terms of time and space.

Here is an alternative implementation of Type Traits for `std::pair`, this time using `binding::object`, wherefore the `std::pair` will encode from and to Objects with two members, one called `first` and one called `second`.

```c++
template< typename U, typename V >
struct my_traits< std::pair< U, V > >
   : public tao::json::binding::object< TAO_JSON_BIND_REQUIRED( "first", &std::pair< U, V >::first ),
                                        TAO_JSON_BIND_REQUIRED( "second", &std::pair< U, V >::second ) >
{
};
```

### Optional Members

Unlike with `binding::array`, it is possible to make individual members optional by using the macro `TAO_JSON_BIND_OPTIONAL` instead of `TAO_JSON_BIND_REQUIRED`.
This works particularly well when the bound member is a `std::optional< T >`, but it can also be used when the previous or default-constructed value of the member is acceptable in the absence of said member from the input.

### Default Keys

The following alternative forms of the binding macros use the [default key](Type-Traits.md#default-key-for-objects) instead of a string supplied to the macro.

```c++
TAO_JSON_BIND_REQUIRED1( &type::member1 );
TAO_JSON_BIND_OPTIONAL1( &type::member2 );
```

Of course the macros with key as first argument can also be used for members whose type defines a default key.
This is even necessary to prevent collisions when there are two occurrences of the same member type.

### Constants

Constants works for Objects just like for Arrays, though of course a string with the key needs to be supplied in addition to the constant value.

Constants are added to an Object binding with the macros `TAO_JSON_BIND_REQUIRED_BOOL()`, `TAO_JSON_BIND_REQUIRED_SIGNED()`, `TAO_JSON_BIND_REQUIRED_UNSIGNED()` and `TAO_JSON_BIND_REQUIRED_STRING()`.
There are again variants with `optional` in place of `required` that are always encoded, but are not flagged as error when missing.

```c++
template< typename U, typename V >
struct my_traits< std::pair< U, V > >
   : public tao::json::binding::object< TAO_JSON_BIND_REQUIRED( "first", &std::pair< U, V >::first ),
                                        TAO_JSON_BIND_REQUIRED( "second", &std::pair< U, V >::second ),
                                        TAO_JSON_BIND_REQUIRED_SIGNED( "foo", -5 ) >
{
};
```

### Inheritance

Inheritance works for Objects just like for Arrays, though "mixing and matching" is not supported:
Array bindings can "inherit" a base class' Array bindings, and Object bindings can "inherit" a base class' Object bindings.

### Unknown Keys

By default, the Object traits' `to()` and `consume()` functions will throw an exception when they encounter an unknown Object key while converting a Value, or parsing some input, respectively.

This can be changed by using `tao::json::binding::basic_object` instead of `tao::json::binding::object`, and passing `tao::json::binding::for_unknown_key::skip` as template parameter in the appropriate place (instead of `tao::json::binding::for_unknown_key::fail`).

With this change, unknown keys will be ignored.

### Nothing Values

By default, the Object traits' `assign()` and `produce()` functions will encode all variables that it knows about, including those that might be considered "nothing".

This can be changed by again using `tao::json::binding::basic_object` instead of `tao::json::binding::object`, and passing `tao::json::binding::for_nothing_value::suppress` as template parameter in the appropriate place (instead of `tao::json::binding::for_nothing_value::ENCODE`).

To determine whether something is to be considered "nothing", the Traits' optional `is_nothing()` function is used.

```c++
template<>
class my_traits< some_type >
{
   template< template< typename... > class Traits >
   static bool is_nothing( const some_type& c ) noexcept
   {
      return whether c is to be considered "nothing";
   }
};
```

If the Traits specialisation for the bound member variable does not implement an `is_nothing()` function then the assumption is that there is something (to encode).

## Polymorphic Factory

The class template `binding::factory` combines Traits for a set of derived classes to form Traits for a common base class.

The `assign()` and `produce()` methods use RTTI to determine the dynamic type of the pointee and delegate to the approriate derived class' Traits.

The `as()` and `consume()` methods expect an encoding as Object with a single member whose key must correspond to one of the types registered with the factory.

Factory Traits must be used as specialisations for a pointer class like `std::shared_ptr` or `std::unique_ptr`.

```c++
struct base
{
   base( const base& ) = delete;
   void operator=( const base& ) = delete;

   virtual void foo() = 0;  // Need one virtual function for RTTI, and for a base class to really make sense.

protected:
   base() = default;
   ~base() = default;  // Has to be public and virtual when using std::unique_ptr instead of std::shared_ptr.
};

struct fobble
   : public base
{
   // Constructors etc.

private:
   void foo() override
   {
   }
};

struct fraggle
   : public base
{
   // Constructors etc.

private:
   void foo() override
   {
   }
};

```

Assuming that there are appropriate specialisations of `my_traits` for the classes `fobble` and `fraggle`, i.e. that they implement all functions that are required by the application, and assuming suitable Traits for `std::shared_ptr`, the actual Factory Traits are implemented as follows.

Note that when using the Traits for `std::shared_ptr` and `std::unique_ptr` from `tao/json/contrib/shared_ptr_traits.hpp` and `tao/json/contrib/unique_ptr_traits.hpp`, possibly via `tao/json/contrib/traits.hpp`, then the Traits for a derived clas need not implement an `as()` or `to()` function when said derived class has a constructor that can create an instance from a Value.

```c++
template<>
struct my_traits< std::shared_ptr< base > >
   : public tao::json::binding::factory< TAO_JSON_FACTORY_BIND( "fobble", fobble ),
                                         TAO_JSON_FACTORY_BIND( "fraggle", fraggle ) >
{
};
```

### Default Keys

For types with a default key, the macro `TAO_JSON_FACTORY_BIND1` can be used instead of `TAO_JSON_FACTORY_BIND`.
Like for Object bindings, it removes the key as argument and uses the default key instead.

## Multiple Versions

The class template `binding::versions` can be used when there are multiple encodings for the same type.

A typical use case is when an old legacy encoding needs to be supported for compatibility, while a new preferred encoding already exists.

With `binding::versions`, arbitrary many different traits can be supported simultaneously with the following behaviour:

* For `as()` and `consume()`, all given Traits are tried, in order, until one succeeds (i.e. doesn't throw an exception).
* All other Traits functions are taken from the **first** Traits supplied to `binding::versions`.

Here is another Traits specialisation for `std::pair`, this time it combines both the Array-based and the Object-based version.
Since the Object-based Traits are given first, the encodings created by `assign()` and `produce()` will be Objects.
In the other direction both Arrays and Objects will be accepted (with Objects being attempted first).

```c++
template< typename U, typename V >
using pair_array_traits = tao::json::binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                                                     TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >;

template< typename U, typename V >
using pair_object_traits = tao::json::binding::object< TAO_JSON_BIND_REQUIRED( "first", &std::pair< U, V >::first ),
                                                       TAO_JSON_BIND_REQUIRED( "second", &std::pair< U, V >::second ) >;

template< typename U, typename V >
struct my_traits< std::pair< U, V > >
   : public tao::json::binding::versions< pair_object_traits,
                                          pair_array_traits >
{
};
```

Note that `binding::versions` is a context in which using constants in Arrays or Objects can be useful in order to distinguish between different encodings without changing the definition of the type being represented.

```c++
struct person
{
   std::string name;
   std::string number;
};

template<>
struct old_person_traits
   : public tao::json::binding::object< TAO_JSON_BIND_REQUIRED( "name", &person::name ),
                                        TAO_JSON_BIND_OPTIONAL( "number", &person::number ) >
{
};

template<>
struct new_person_traits
   : public tao::json::binding::object< TAO_JSON_BIND_REQUIRED_UNSIGNED( "version", 2 ),
                                        TAO_JSON_BIND_REQUIRED( "name", &person::name ),
                                        TAO_JSON_BIND_REQUIRED( "number", &person::number ) >
{
};

template<>
struct my_traits< person >
   : public tao::json::binding::versions< new_person_traits,
                                          old_person_traits >
{
};
```

Note that `binding::versions` uses the traits' `to()` method to fill-in an existing or default-constructed object, and that it does *not* clear or reset the target object between attempts!

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
