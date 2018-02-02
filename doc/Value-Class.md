# Value Class

The Value class `tao::json::value` is a C++ class that implements the JAXN data model, a superset of the implied JSON data model.

It can be considered the sum-type, or discriminated union, of all possible types a JSON or JAXN value can be.
On the C++ level the class consists of an `enum` to indicate which type it contains, and a `union` to store the corresponding data.

More precisely `tao::json::value` is a specialisation of the class template `tao::json::basic_value` with the default `tao::json::traits`, and an empty base class.

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
   RAW_PTR
};
```

The set of types is larger than that defined by the data model, with the additional types allowing for greater efficiency and better error detection in some cases.

### Uninitialized

Unlike some other JSON libraries, a default-initialised Value object will have `type::UNINITIALIZED` rather than `NULL_`.

The `explicit operator bool () const noexcept` returns whether the Value contains *anything*, including a `NULL_`.
In other words, it only returns `false` when its `type()` returns `type::UNITIALIZED`.

### Discarded

The discarded type indicates that this value was the source of a move operation.
According to the C++ standard performing any operation other than calling the destructor on such an object is not allowed.
The discarded type is only used when the `NDEBUG` macro is not set.

### Destroyed

The destroyed type indicates that this value either never completed a constructor without throwing an exception, or that the destructor has already completed.
At this point there is no actual value object, we are outside of its lifetime and just happen to be looking at memory that (nearly) contained a value in the past.
The destroyed type is only used when the `NDEBUG` macro is not set.

### Null

Null exists only on the type/meta-level; there is no C++ type corresponding to `null` values.

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

## Constructors

## Value Comparison

The library contains the full complement of comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=` ) to compare values with other values, and to compare values with any other C++ types.

Comparison of two values performs a structural recursive comparison ignoring both the traits and the base class.
The comparison is performed at the data model level, abstracting away from implementation details and optimisations.

* Numbers are compared by value, independent of which of the three possible representations they use.
* Strings and string views are compared by comparing character sequences, independent of which representation is used.
* Binary data and binary views are compared by comparing byte sequences, indpendent of which representation is used.
* Raw pointers are skipped, the comparison behaves "as if" the pointee object were at the position the pointer is.
* Values of different incompatible types will be ordered by the numeric values of their type enum.

Comparison between a value and other types is conceptually performed by first creating a value object from the other type, and then performing a comparison between two values.
See the section on Value Traits below on how the traits class is used to optimise these operations by not actually creating the value object from the other type.

## Unsafe Functions

Some of the member functions of class `tao::json::basic_value<>` are also available in "unsafe" versions that, in the name of efficiency, make certain assumptions, or omit/make impossible certain checks.

All of the accessors like `get_boolean()` etc. have an unsafe version `unsafe_get_boolean()` that does not check the type and throw an exception on mismatch.

## Base Annotations

The Value class template `tao::json::basic_value< Traits, Base >` privately derives from its second template argument, allowing the addition of custom members to Value instances, including all sub-values.

The base class is always default-constructed, but fully preserved during copy/move construct and assignment operations from other (compatible) Value instances.
Access to the base class is provided by two getter-style functions, `Base& tao::json::basic_value< Traits, Base >::base() noexcept` and `const Base& bao::json::basic_value< Traits, Base >::base() const noexcept`.

The file `tao/include/json/contrib/position.hpp` shows how to parse a JSON file into a Value object where the Value, and all sub-values, are annotated with the filename and position in the file their respective definitions started at.

## Value Traits

TODO: List everything customised via traits...
