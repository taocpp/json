# Value Class

The Value class `tao::json::value` is a C++ class that implements the JAXN data model, a superset of the implied JSON data model.

It can be considered the sum-type, or discriminated union, of all possible types a JSON or JAXN value can be.
On the C++ level the class consists of an `enum` to indicate which type it contains, and a `union` to store the corresponding data.

More precisely `tao::json::value` is an instantiation of the class template `tao::json::basic_value` with the [included default traits](Type-Traits.md) and an [empty base class](Advanced-Use-Cases.md#custom-value-annotations) as template parameters.

In order to simplify the following discussion, it will mostly pretend that the behaviour from the [default traits](Type-Traits.md) is hard-wired and that there is no alternative.
Please read the [page on traits](Type-Traits.md) to see which aspects of the behaviour of the JSON Value class can be extended and customised, and how.

## Value Type

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
   RAW_PTR,
   OPAQUE_PTR
};
```

The set of types is larger than that defined by the data model, with the additional types allowing for greater efficiency and better error detection in some cases.

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

The corresponding C++ type is `tao::json::null_t`, and `tao::json::null` is a `constexpr` instance of that type.
You can construct from, assign, or compare a Value against `tao::json::null`.

Test for `null` values with the `is_null()` method.

### Boolean

Stored as C++ `bool`.

Test for with the `is_boolean()` method.

Access the contained `bool` with the `as< bool >()` method, or the low-level `get_boolean()`.

### Number

Numbers are stored either as `std::int64_t` with `type::SIGNED`, as `std::uint64_t` with `type::UNSIGNED`, or as `double` with `type::DOUBLE`.

Apart from the `type()` method, and beyond the usual `is_signed()`, `is_unsigned()` and `is_double()` methods, these types can be checked for with the `is_integer()` and `is_number()` convenience methods.

Access via the `as<>()` method which can be used with any numeric data type.
The default traits will convert the contained number from any of the three possible types to the desired target type just like a `static_cast<>` would.
See the section on Value Traits below on how to customise this behaviour.

It is again possible to directly access the contained number with the `get_signed()`, `get_unsigned()` and `get_double()` methods.

### String (and View)

Strings are sequences of Unicode code points stored as UTF-8 in `std::string` and `tao::string_view` objects, the latter of which is an alias for `std::string_view` when possible.

Checking for valid UTF-8, and (un)escaping according to the representation format like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

### Binary (and View)

Binary data is stored in `std::vector< tao::byte >` and `tao::byte_view`, where `tao::byte` is an alias for `std::byte` when possible.

### Date-time

TODO: Document when implemented.

### Array

Arrays are stored as `std::vector< basic_value >`.

### Object

Objects are stored as `std::map< std::string, basic_value >`.

### Raw Pointers

See the [section on raw and opaque pointers](Advanced-Features.md#raw-and-opaque-pointers) on the [advanced features page](Advanced-Features.md).

### Opaque Pointers

See the [section on raw and opaque pointers](Advanced-Features.md#raw-and-opaque-pointers) on the [advanced features page](Advanced-Features.md).

## Constructors

## Value Comparison

The library contains the full complement of comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=` ) to compare values with other values, and to compare values with any other C++ types.

Comparison of two values performs a structural recursive comparison ignoring both the traits and the base class template parameters.
The comparison is performed at the data model level, abstracting away from implementation details and optimisations.

* Numbers are compared by value, independent of which of the three possible representations they use.
* Strings and string views are compared by comparing character sequences, independent of which representation is used.
* Binary data and binary views are compared by comparing byte sequences, indpendent of which representation is used.
* Raw pointers are skipped, the comparison behaves "as if" the pointee object were at the position the pointer is.
* Values of different incompatible types will be ordered by the numeric values of their type enum.

Comparison between a JSON Value and other types is performed either by

1. creating a Value object from the other type and performing the comparison between the two Value, or by
2. using the [traits](Type-Traits.md) to directly perform the comparison without creating a temporary Value.

## Unsafe Functions

Some of the member functions of class `tao::json::basic_value<>` are also available in "unsafe" versions that, in the name of efficiency, make certain assumptions, or omit/make impossible certain checks.

All of the accessors like `get_boolean()` etc. have an unsafe version `unsafe_get_boolean()` that does not check the type and throw an exception on mismatch.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
