# Binding Traits

The Binding Traits facilities are used to "bind" existing [Type Traits](Type-Traits.md) together to create [Type Traits](Type-Traits.md) for new types or with new features.

* [Array](#array)
* [Object](#object)
* [Factory](#factory)
* [Versions](#versions)

## Array

The class template `binding::array` can be used to easily create Traits for types that are to be represented by Arrays.

It takes arbitrary many template arguments that are usually created by invoking the macro `TAO_JSON_BIND_ELEMENT` with a member object pointer, i.e. a pointer to a member object of the type for which the Traits are to be created.

For example consider the following hypothetical `my_traits` specialisation for `std::pair`, which looks just like the Traits defined in `tao/json/contrib/pair_traits.hpp`.

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

These Traits' `assign()` and `produce()` functions will always create Arrays with two elements, and conversely the `to()` and `consume()` functions will expect Arrays with two elements.

Handling of the two member objects `first` and `second` is delegated to the Traits' specialisations for their respective types.

### Constants

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

### Getters and Setters

## Object

The class template `binding::object` can be used to easily create Traits for types that are to be represented by Objects.

Compared to the Array binding explained above the Object binding has the advantages of being more expressive and flexible, and the disadvantage of being less efficient, both in terms of time and space.

...

### Optional Keys

### Default Keys

The following alternative forms of the binding macros use the [default key](Type-Traits.md#default-key-for-objects) instead of a string supplied to the macro.
Of course they can only be used when a [default key](Type-Traits.md#default-key-for-objects) is defined in the [Traits](Type-Traits.md) specialisation of the class/struct member's type.

```c++
TAO_JSON_BIND_REQUIRED1( &type::member1 );
TAO_JSON_BIND_OPTIONAL1( &type::member2 );
```

### Constants

Constants work for Objects similarly to how they work for Arrays.

...

### Inheritance

Inheritance works for Objects just like for Arrays, though "mixing and matching" is not supported.
Array bindings can "inherit" a base class' Array bindings, and Object bindings can "inherit" a base class' Object bindings.

### Unknown Keys

By default, the Object traits' `to()` and `consume()` functions will throw an exception when they encounter an unknown Object key while converting a Value, or parsing some input, respectively.

This can be changed by using `tao::json::binding::basic_object` instead of `tao::json::binding::object`, and passing `tao::json::binding::for_unknown_key::CONTINUE` as template parameter in the appropriate place (instead of `tao::json::binding::for_unknown_key::THROW`).

With this change, unknown keys will be ignored.

### Nothing Values

By default, the Object traits' `assign()` and `produce()` functions will encode all variables that it knows about, including those that might be considered "nothing".

This can be changed by again using `tao::json::binding::basic_object` instead of `tao::json::binding::object`, and passing `tao::json::binding::for_nothing_value::SUPPRESS` as template parameter in the appropriate place (instead of `tao::json::binding::for_nothing_value::ENCODE`).

To determine whether something is to be considered "nothing", the Traits' optional `is_nothing()` function is used.

```c++
template<>
class traits< some_type >
{
   template< template< typename... > class Traits >
   static bool is_nothing( const some_type& c ) noexcept
   {
      return whether c is to be considered "nothing";
   }
};
```

If the Traits specialisation for the bound member variable does not implement an `is_nothing()` function then the assumption is there is something (to encode).

## Factory

The class template `binding::factory` extends on the principle of a polymorphic object factory.
It combines traits for a set of derived classes to form traits for a common base class.

The `assign()` and `produce()` methods use RTTI to determine the dynamic type of the pointee and delegate to the approriate derived class' traits.

The `as()` and `consume()` methods expect an encoding as Object with a single member whose key must correspond to one of the types registered with the factory.

...

### Default Keys



## Versions


Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
