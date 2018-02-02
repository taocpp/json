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

Unlike some other JSON libraries, a default-initialised Value object will be of type `UNINITIALIZED` rather than `NULL_`.

The `explicit operator bool () const noexcept` that is part of the Value class returns whether the Value contains *anything*, including a `NULL_`.
In other words, it only returns `false` when its `type()` returns `type::UNITIALIZED`.

### Discarded

TODO

### Destroyed

TODO

### Null

Null exists only on the type/meta-level; there is no C++ type corresponding to `null` values.

Test for with `bool tao::json::basic_value<>::is_null() const noexcept`.

### Boolean

Stored as C++ `bool`.

Test for with `bool tao::json::basic_value<>::is_boolean() const noexcept`.

Obtain with `bool tao::json::basic_value<>::get_boolean() const`.

### Signed

Signed integers are stored as `std::int64_t`.

### Unsigned

Unsigned integers are stored as `std::uint64_t`.

### Double

Floating point values are stored as `double`.

### Numbers

TODO: Describe `is_number()` and how to get a number as `T` independent of how it's stored.

### String and String-View

Strings are sequences of Unicode code points stored as UTF-8 in `std::string` and `tao::string_view` objects, the latter of which is an alias for `std::string_view` when possible.

Checking for valid UTF-8, and (un)escaping according to the representation format like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

### Binary and Binary-View

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

## Unsafe Functions

Some of the member functions of class `tao::json::basic_value<>` are also available in "unsafe" versions that, in the name of efficiency, make certain assumptions, or omit/make impossible certain checks.

All of the accessors like `get_boolean()` etc. have an unsafe version `unsafe_get_boolean()` that does not check whether the result of `type()` is the correct one.

## Base Annotations

The Value class template `tao::json::basic_value< Traits, Base >` privately derives from its second template argument, allowing the addition of custom members to Value instances, including all sub-values.

The base class is always default-constructed, but fully preserved during copy/move construct and assignment operations from other (compatible) Value instances.
Access to the base class is provided by two getter-style functions, `Base& tao::json::basic_value< Traits, Base >::base() noexcept` and `const Base& bao::json::basic_value< Traits, Base >::base() const noexcept`.

The file `tao/include/json/contrib/position.hpp` shows how to parse a JSON file into a Value object where the Value, and all sub-values, are annotated with the filename and position in the file their respective definitions started at.

## Value Traits

TODO: List everything customised via traits...
