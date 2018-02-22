# Value Class

* [Overview](#overview)
* [Value and Data Types](#value-and-data-types)
* [Creating Values](#creating-values)
* [Accessing Values](#accessing-values)
* [Manipulating Values](#manipulating-values)
* [Comparing Values](#comparing-values)
* [Unsafe Functions](#unsafe-functions)

## Overview

The Value class `tao::json::value` is a C++ class that implements the JAXN data model, a superset of the implied JSON data model.

It is implemented as sum-type, or discriminated union, of the JSON types, extended for the JAXN data model.
On the C++ level the class consists of an `enum` to indicate which type it contains, and a `union` to store the corresponding data.

More precisely the class `tao::json::value` is an instantiation of the class template `tao::json::basic_value` with the [included default traits](Type-Traits.md) and an [empty base class](Advanced-Use-Cases.md#custom-value-annotations) as template parameters.

In order to simplify the following discussion, we will mostly ignore the traits and pretend that the behaviour from the [default traits](Type-Traits.md) is hard-wired.
Please read the [page on traits](Type-Traits.md) to see which aspects of the behaviour of the JSON Value class can be extended and customised, and how.

Just remember that everywhere `tao::json::value` is referenced it could also be a customised version `tao::json::value< MyTraits, MyBase >` with [custom traits](Type-Traits.md) and [custom annotation base class](Advanced-Use-Cases.md#custom-value-annotations).

A JSON Value that is an Array or an Object contains -- when not empty -- other JSON Values, whereby a single JSON Value can store arbitrarily large and complicated JSON structures consisting of arbitrarily nested Arrays and Objects with many sub-values.

The Value class API has a large set of functions to create, inspect, and manipulate JSON Values.

## Value and Data Types

The `enum` part of the discriminated union is a `tao::json::type`.

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
   RAW_PTR,
   OPAQUE_PTR
};
```

The set of types is larger than that defined by the data model, with the additional types allowing for greater efficiency and better error detection in some cases.

The type of a JSON Value can be queried with the member function `tao::json::value::type()` which returns one of the enumeration values given above.

### Uninitialized

A default initialised Value object will enter an explicit uninitialized state and report its type as `type::UNINITIALIZED`, rather than the more common default initialisation to a JSON Null.

The `explicit operator bool()` returns whether the Value contains *any* valid value, including a `NULL_`.
In other words, it only returns `false` when its `type()` returns `type::UNITIALIZED` (it MUST NOT be called for `type::DISCARDED` or `type::DESTROYED`).

### Discarded

The discarded type indicates that this value was the source of a move operation, meaning the object is in a moved-from state.
According to the C++ standard performing any operation other than assigning another value or calling the destructor
on such an object is not allowed.
The discarded type is only used when the `NDEBUG` macro is not set.

### Destroyed

The destroyed type indicates that this value either never completed a constructor without throwing an exception, or that the destructor has already completed.
At this point there is no actual value object, we are outside of its lifetime and just happen to be looking at memory that (nearly) contained a value in the past.
The destroyed type is only used when the `NDEBUG` macro is not set.

### Null

Represents JSON Null.

Can be tested for with `tao::json::value::is_null()`.

### Boolean

Represents JSON True and False, stored as C++ `bool`.

Can be tested for with `tao::json::value::is_boolean()`.

### Numbers

JSON Numbers are stored as either `std::int64_t` with `type::SIGNED`, as `std::uint64_t` with `type::UNSIGNED`, or as `double` with `type::DOUBLE`.

Can be tested for with `...is_number()`, which will return `true` for all three types, or with the more specific `...is_integer()`, `...is_signed()`, `...is_unsigned()` and `...is_double()`.

Unlike JSON, non-finite and `NaN` values are allowed for floating point values.
When serialising to a format that does not support these values, an [Event filter](Events-Interface.md#included-filters) can be used to transform them on-the-fly.

### Strings

Strings are sequences of Unicode code points stored as UTF-8 as `std::string` with `type::STRING`, or as `tao::string_view` with `type::STRING_VIEW`; the view is an alias for `std::string_view` when available.

Checking for valid UTF-8, and (un)escaping according to the representation format like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

Can be tested for with `...is_string_type()`, which will return `true` for both types, or with the more specific `...is_string()` and `...is_string_view()`.

### Binary Data

Binary data is stored as `std::vector< tao::byte >` with `type::BINARY`, or as `tao::byte_view` with `type::BINARY_VIEW`; here `tao::byte` is an alias for `std::byte` when available.

Can be tested for with `...is_binary_type()`, which will return `true` for both types, or with the more specific `...is_binary()` and `...is_binary_view()`.

When serialising to a format that does not support binary data, an [Event filter](Events-Interface.md#included-filters) can be used to transform it on-the-fly.

### Arrays

JSON Arrays are stored as `std::vector< basic_value >`.

Can be tested for with `...is_array()`.

### Objects

JSON Objects are stored as `std::map< std::string, basic_value >`.

Can be tested for with `...is_object()`.

### Raw Pointers

See the [section on instance sharing with Raw pointers](Advanced-Use-Cases.md#instance-sharing-with-raw-pointers) on the [advanced use cases page](Advanced-Use-Cases.md).

### Opaque Pointers

See the [section on instance sharing with Opaque pointers](Advanced-Use-Cases.md#instance-sharing-with-opaque-pointers) on the [advanced use cases page](Advanced-Use-Cases.md).

## Creating Values

The Value class has a default constructor that creates a Value with `type::UNINITIALIZED`.
It also has copy and move constructors (and copy and move assignment operators, and a `swap()` member function).

There are also two **non**-explicit constructors, one that takes an argument of any type and uses [the traits](Type-Traits.md) for the details of how to create the Value, and one that takes an initialiser-list for an Object.

The first allows creating (or assigning to) a Value from [a list of types](Type-Traits.md#default-traits-specialisations) [that can be extended](Type-Traits.md).

```c++
const tao::json::value v1 = true;
const tao::json::value v2 = "Hi!";
const std::shared_ptr< std::map< std::string, std::vector< int > > > data = ...;
const tao::json::value v3 = data;
```

The second allows creating Values that represent JSON Objects with an initialiser-list of key-value pairs.
As per the JSON standard, the keys must be strings.
The values can are individually conveted to JSON Values, and can therefore be of any type that a JSON Value can be constructed from.

```c++
const tao::json::value v{
   { "hello", "world" },
   { "answer", 41.0 },
   { "correct", false },
   { "sub-object", {
      { "value", 1 },
      { "frobnicate", true }
   } } };
```

If a certain C++ type is frequently used with the same key in a JSON Object, the key can be put in the [traits for that type](Type-Traits.md).
When an instances of such a type is used in a JSON Object intialiser-lists *without* a key, then [the key from the traits](Type-Traits.md#default-key-for-objects) will be used by default.

```c++
const type_with_default_key t;
const tao::json::value v( {
   { "foo", 42 },
   { "bar", true },
   t  // Uses default key from traits.
} );
```

See the [section on default keys](Type-Traits.md#default-key-for-objects) on the [type traits page](Type-Traits.md) for details on how to set up a default key for a type.
And note that Object initialiser-lists can still use (different) keys for types with a default key, just like for all (other) types.

Since it is not possible to use initaliser-lists in a way that efficiently and unambiguously works for both Arrays and Objects, it is necessary to disambiguate by using the static `...array()` function to create a Value with an Array from an initialiser-list.

```c++
const tao::json::value array = tao::json::value::array( {
   42.0,
   "fish",
   true,
   tao::json::value::array( { 1, 2, 3 } ),
   { { "an", 1 },
     { "object", 2 },
     { "as sub-value", "of the array" }
   } } );
```

To simplify creating Values that represent a JSON Null, an empty array, an empty object, or empty binary data, there are types `tao::json::null_t`, `tao::json::empty_array_t`, `tao::json::empty_object_t`, and `tao::json::empty_binary_t` with global `constexpr` instances `tao::json::null`, `tao::json::empty_array`, `tao::json::empty_object`, and `tao::json::empty_binary`.
They can construct Values, and, like everything else that a Value can be constructed from, can be assigned to Values.

```c++
tao::json::value v = tao::json::null;
v = tao::json::empty_array;
v = {
   { "empty object", tao::json::empty_object },
   { "empty binary", tao::json::empty_binary }
};
```

## Accessing Values

TODO

## Manipulating Values

TODO

## Comparing Values

The library contains the full complement of comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=` ) to compare Values with other Values, and to compare Values with any other C++ type.

Comparison of two Values performs a structural recursive comparison ignoring both the traits and the base class template parameters.
The comparison is performed at the data model level, abstracting away from implementation details and optimisations.

* Numbers are compared by value, independent of which of the three possible representations they use.
* Strings and string views are compared by comparing character sequences, independent of which representation is used.
* Binary data and binary views are compared by comparing byte sequences, indpendent of which representation is used.
* Raw pointers are skipped, the comparison behaves "as if" the pointee Value were copied to the Value with the pointer.
* Values of different incompatible types will be ordered by the numeric Values of their type enum.

Comparison between a JSON Value and another type is performed by either

1. creating a Value object from the other type and performing the comparison between the two Value, or
2. using the [traits](Type-Traits.md) to directly perform the comparison without creating a temporary Value.

See the [documentation on type traits](Type-Traits.md#compare-value-with-type) for details.

## Unsafe Functions

Some of the member functions of class `tao::json::value` are also available in "unsafe" versions that, in the name of efficiency, make certain assumptions, or omit/make impossible certain checks.

All of the accessors like `get_boolean()` have an unsafe version `unsafe_get_boolean()` that does not check the type and throw an exception on mismatch.

All of the assign methods like `assign_boolean()` have an unsafe version `unsafe_assign_boolean()` that does not check whether the previous value has a non-trivial destructor that needs to be called before being overwritten.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
