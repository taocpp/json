# Value Class

* [Overview](#overview)
* [Value and Data Types](#value-and-data-types)
* [Creating Values](#creating-values)
* [Uniform Initialization](#uniform-initialization)
* [Accessing Values](#accessing-values)
* [Manipulating Values](#manipulating-values)
* [Container Functions](#container-functions)
* [Comparing Values](#comparing-values)

## Overview

The Value Class `tao::json::value` is a C++ class that implements the JAXN data model, a superset of the JSON data model.

It is implemented as sum-type, or discriminated union, of the JSON types, extended for the JAXN data model.
On the C++ level the class consists of an `enum` to indicate which type it contains, and a `union` to store the corresponding data.

More precisely, class `tao::json::value` is an instantiation of the class template `tao::json::basic_value<>` with the [included default traits](Type-Traits.md) and an [empty Base class](Advanced-Use-Cases.md#custom-base-class-for-values) as template parameters.

In order to simplify the following discussion, we will mostly ignore the traits and pretend that the behaviour from the [default traits](Type-Traits.md) is hard-wired.
Please read the [page on Type Traits](Type-Traits.md) to see which aspects of the behaviour of the JSON Value Class can be extended and customised, and how.

Just remember that everywhere `tao::json::value` is referenced it could also be a customised version `tao::json::value< MyTraits, MyBase >` with [custom traits](Type-Traits.md) and [custom Base class for Values](Advanced-Use-Cases.md#custom-base-class-for-values).

A JSON Value that is an Array or an Object contains -- when not empty -- other JSON Values, whereby a single JSON Value can store arbitrarily large and complicated JSON data structures consisting of arbitrarily nested Arrays and Objects with many sub-values, that are again instances of class `tao::json::value`.

The Value class API has a large set of functions to create, inspect, and manipulate JSON (or JAXN) values.

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
   VALUE_PTR,
   OPAQUE_PTR
};
```

The set of types is larger than that defined by the data model, with the additional types allowing for greater efficiency and better error detection in some cases.

The type of a JSON Value can be queried with the member function `tao::json::value::type()` which returns one of the enumeration values given above.

### Uninitialized

A default initialised Value object will enter an explicit uninitialized state and report its type as `type::UNINITIALIZED`, rather than the more common default initialisation to a JSON Null.

The `explicit operator bool()` returns whether the Value contains *any* valid value, including a `NULL_`.
In other words, it only returns `false` when its `type()` returns `type::UNITIALIZED` (it MUST NOT be called for `type::DISCARDED` or `type::DESTROYED`).

The function `tao::json::value::reset()` returns a Value to this state from any other type or state (unless `tao::json::type::DESTROYED`).

### Discarded

The `type::DISCARDED` indicates that this value was the source of a move operation, meaning the object is in a moved-from state.
According to the C++ standard the only allowed operations in this state are assigning another value and calling the destructor.
The discarded type is only used when the `NDEBUG` macro is not set.

### Destroyed

The `type::DESTROYED` indicates that this value either never completed a constructor without throwing an exception, or the destructor has already completed.
At this point there is no actual value object, we are outside of its lifetime and just happen to be looking at memory that (nearly) contained a value in the past.
The destroyed type is only used when the `NDEBUG` macro is not set.

### Null

Represents JSON Null.

Can be tested for with the member function `tao::json::value::is_null()`.

### Boolean

Represents JSON True and False, stored as C++ `bool`.

Can be tested for with the member function `tao::json::value::is_boolean()`.

### Numbers

JSON Numbers are stored as either `std::int64_t` with `type::SIGNED`, as `std::uint64_t` with `type::UNSIGNED`, or as `double` with `type::DOUBLE`.

Can be tested for with the member function `is_number()`, which will return `true` for all three types, or with the more specific `is_integer()`, `is_signed()`, `is_unsigned()` and `is_double()`.

Unlike JSON, non-finite and `NaN` values are allowed for floating point values!

When serialising to a format that does not support these values, an [Events Transformers](Events-Interface.md#included-transformers) can be used to [transform these values to something else on-the-fly](Common-Use-Cases.md#serialise-with-base64-strings-for-binary-data).

### Strings

Strings are sequences of Unicode code points stored as UTF-8 as `std::string` with `type::STRING`, or as `tao::string_view` with `type::STRING_VIEW`; the view class is an alias for `std::string_view` when available.

Checking for valid UTF-8, and (un)escaping according to the representation format like JSON, is performed "at the edges" by the respective parsers and encoders (although some parsers might choose to omit this check for performance reasons).

Can be tested for with the member functions `is_string_type()`, which will return `true` for both types, or with the more specific `is_string()` and `is_string_view()`.

### Binary Data

Binary data is stored as `std::vector< tao::byte >` with `type::BINARY`, or as `tao::binary_view` with `type::BINARY_VIEW`; here `tao::byte` is an alias for `std::byte` when available.

Can be tested for with the member function `is_binary_type()`, which will return `true` for both types, or with the more specific `is_binary()` and `is_binary_view()`.

When serialising to a format that does not support binary data, an [Events Transformer](Events-Interface.md#included-transformers) can be used to [transform binary data to something else on-the-fly](Common-Use-Cases.md#serialise-with-base64-strings-for-binary-data).

### Arrays

JSON Arrays are stored as `std::vector< tao::json::value >`.

Can be tested for with the member function `tao::json::value::is_array()`.

(The sub-values share the [type traits](Type-Traits.md) and [base class](Advanced-Use-Cases.md#custom-base-class-for-values) of the containing `basic_value<>`.)

### Objects

JSON Objects are stored as `std::map< std::string, tao::json::value >`.

Can be tested for with the member function `tao::json::value::is_object()`.

(The sub-values share the [type traits](Type-Traits.md) and [base class](Advanced-Use-Cases.md#custom-base-class-for-values) of the containing `basic_value<>`.)

### Value Pointers

See the [section on instance sharing with Value Pointers](Instance-Sharing.md#value-pointers) on the [More Performance](More -Performance.md).

### Opaque Pointers

See the [section on instance sharing with Opaque Pointers](Instance-Sharing.md#opaque-pointers) on the [More Performance](More -Performance.md).

## Creating Values

The Value Class has a default constructor that creates a Value with `type::UNINITIALIZED`.
It also has copy and move constructors (and copy and move assignment operators, and a `swap()` member function).

There are also two **non**-explicit constructors, one that takes an argument of any type and uses [the Type Traits](Type-Traits.md) for the details of how to create the Value, and one that takes an initialiser-list to create an Object.

The first allows creating (or assigning to) a Value from [a list of types](Batteries-Included.md#default-traits) [that can be extended](Type-Traits.md) [like here](Batteries-Included.md#additional-traits).

```c++
const tao::json::value v1 = true;
const tao::json::value v2 = "Hi!";
const std::shared_ptr< std::map< std::string, std::vector< int > > > data = ...;
const tao::json::value v3 = data;
```

The second allows creating Values that represent JSON Objects with an initialiser-list of key-value pairs.
As per the JSON standard, the keys must be strings (`std::string`, `tao::string_view` or `const char*`).
The values can individually be converted to JSON Values, and can therefore be of any type that a JSON Value can be constructed from.

```c++
const tao::json::value v = {
   { "hello", "world" },
   { "answer", 41.0 },
   { "correct", false },
   { "sub-object", {
      { "value", 1 },
      { "frobnicate", true }
   } }
};
```

If a certain C++ type is frequently used with the same key in a JSON Object, the key can be put in the [traits for that type](Type-Traits.md).
When an instance of such a type is used in a JSON Object intialiser-list *without* a key, then [the key from the traits](Type-Traits.md#default-key-for-objects) will be used by default.

```c++
const type_with_default_key t;
const tao::json::value v( {
   { "foo", 42 },
   { "bar", true },
   t  // Shortcut for: { tao::json::traits< type_with_default_key >::default_key< traits >::as_string(), t }
} );
```

See the [section on default keys](Type-Traits.md#default-key-for-objects) on the [Type Traits page](Type-Traits.md) for details on how to set up a default key for a type.
Object initialiser-lists can still use (different) keys for types with a default key when they are given explicitly just like for all (other) types.

Since it is [not possible to use initialiser-lists in a way that efficiently and unambiguously works for both Arrays and Objects](Design-Decisions.md#initialiser-lists), it is necessary to disambiguate manually by using the static `tao::json::value::array()` function to create a Value with an Array from an initialiser-list.

```c++
const auto array = tao::json::value::array( {
   42.0,
   "fish",
   true,
   // an element which is itself an array:
   tao::json::value::array( { 1, 2, 3 } ),
   // an element which is an object:
   {
     { "an", 1 },
     { "object", 2 },
     { "as sub-value", "of the array" }
   }
} );
```

To simplify creating Values that represent a JSON Null value, an empty array, an empty object, or empty binary data, there are global objects `tao::json::null`, `tao::json::empty_array`, `tao::json::empty_object`, and `tao::json::empty_binary`.
They can construct Values, and, like everything else that a Value can be constructed from, can be assigned to Values. Additionally, you can compare Values against the global helper objects.

```c++
tao::json::value v = tao::json::null;
v = tao::json::empty_array;
v = {
   { "empty object", tao::json::empty_object },
   { "empty binary", tao::json::empty_binary }
};
```

## Uniform Initialization

The Value class has both a constructor and an assignment operator accepting an initializer list which assigns a special meaning to curly brackets in these contexts and has consequences regarding the so called "uniform initialization".
The idealized intention of uniform initialization was to "always" use curly brackets instead of round brackets to construct an object.
However as curly brackets for Values are used to denote a JSON Object, they should not be confused with, or blindly replace, round brackets.

### `empty_object`

**Guideline:** Always use `tao::json::empty_object` instead of `{}` to create an empty JSON Object.

The curly brackets are the default constructor in several important cases and thereby create an uninitialized value, i.e. a Value of `type::UNINITIALIZED`.

The following lines behave as expected:

```c++
tao::json::value v( tao::json::empty_object );
tao::json::value v = tao::json::empty_object;

auto v = tao::json::value( tao::json::empty_object );

v = tao::json::empty_object;
v = { { "foo", tao::json::empty_object } };
```

The following lines using `{}` might produce surprising behaviour:

```c++
tao::json::value v{}; // default ctor, not initializer_list ctor
tao::json::value v = {}; // default ctor, not initializer_list ctor

tao::json::value v( {} ); // initializer_list ctor, creates an empty object

v = {}; // assign a default-constructed value
v = { { "foo", {} } }; // object with one member, key "foo" with an uninitialized value
```

A few more points to consider:

```c++
// creates a variable 'v' with type tao::json::empty_object_t, not tao::json::value
auto v = tao::json::empty_object;

// default ctor:
tao::json::value v;

// most vexing parse, declares a function
tao::json::value v();
```

### Single Argument Constructors

To copy/move a Value, or construct a Value from a single value via the traits, use round brackets or assignment.

**Guideline:** Avoid using curly brackets *unless* you mean to create/assign a non-empty JSON Object.

The following should behave as expected:

```c++
tao::json::value v = 42; // uses traits
tao::json::value v = "Hallo"; // uses traits

tao::json::value v( 42 ); // uses traits
tao::json::value v( "Hallo" ); // uses traits

tao::json::value v2 = v; // copy ctor
tao::json::value v2 = std::move( v ); // move ctor

tao::json::value v2( v ); // copy ctor
tao::json::value v2( std::move( v ) ); // move ctor

// any non-empty initializer list for a JSON object is OK
tao::json::value v = { { "foo", 42 } };
tao::json::value v( { { "foo", 42 } } );

v = { { "foo", 42 } };
```

Again, as a reminder:

```c++
// avoid empty curly brackets:
tao::json::value v{}; // an uninitialized Value
tao::json::value v = {}; // an uninitialized Value
tao::json::value v( {} ); // empty JSON object (*)
tao::json::value v( tao::json::empty_object ); // preferred

v = {}; // Assigns a default constructed (uninitialized) Value
```

Note that the line marked with (\*) does not work on GCC 7 due to a compiler bug. Hence it should also be avoided, even if it is technically correct.

### Implicit construction

**Guideline:** When calling a function with a Value as parameter, only use curly brackets to indicate non-empty JSON Objects.

Examples:

```c++
void f( const tao::json::value& );

f( 42 );
f( "Hello" );
f( tao::json::empty_object );
f( { { "foo", 42 } } );
```

With the problematic cases being:

```c++
f( {} ); // default ctor for v, uninitialized object
f( { 42 } ); // won't call the ctor with int (*)
f( { v2 } ); // won't call the copy ctor (*)
```

Again, having a ctor taking an initializer list breaks "uniform initialization", meaning that you can not provide parameters for other constructors using curly brackets.

The following works as expected (albeit being overly verbose and therefore slightly pointless):

```c++
f( tao::json::value( 42 ) );
f( tao::json::value( v2 ) );
```

## Accessing Values

The function `tao::json::value::type()`, and the functions like `tao::json::value::is_boolean()` mentioned above, can be used to determine the type of a Value.

The data held by a Value can be extracted and converted with the `tao::json::value::as< T >()` and `tao::json::value::to< T >( T& )` functions.

1. The basic form of `as()` returns the value of type `T`, while
2. `to()` takes a non-const reference to an instance of type `T`.

```c++
const tao::json::value v = 3;
int i = v.as< int >();  // Initialised i to 3.
v.to( i );  // Sets i to 3.
```

Note that the [default Type Traits](Type-Traits.md) do **not** perform range checking when converting between different arithmetic types!

As usual, the conversion works with [all types and nested types that are supported by the default Traits](Batteries-Included.md#default-traits) and the [additional Traits](Batteries-Included.md#additional-traits), for example shared pointers and vectors.

```c++
const tao::json::value v = tao::json::value::array( { 1, 2, tao::json::null, 4 } );
const auto w = v.as< std::vector< std::shared_ptr< int > > >();
```

When the Type Traits are correctly specialised for a custom type then [a JSON Value can be directly converted into the custom type](Type-Traits.md#convert-value-into-type) with `tao::json::value::as()`.

The function `tao::json::value::optional< T >()` is similar to `tao::json::value::as< T >()`, but it returns a `tao::optional< T >`.
The return value is an empty optional when the Value on which the method was called is Null, else an optional initalised with the result of a call to `tao::json::value::as< T >()` is returned.

When the Value in question is an Object, the variants of `as()`, `to()` and `optional()` that take an Object key as (additional) argument can be used.

The `optional()` function with a key returns an empty optional when the key is not found in the Object; when the Value is not an Object or the unconditional conversion to the target type fails, an exception is thrown.

```c++
const tao::json::value v = {
   { "foo", 4 },
   { "bar", tao::json::null }
};
int i = v.as< int >( "foo" );  // Like v.at( "foo" ).as< int >()
v.to( i, "foo" );  // Like v.at( "foo" ).to( i )

auto a = v.optional< int >( "foo" );  // a has value 4
auto b = v.optional< int >( "xyz" );  // b is empty
auto c = v.optional< int >( "bar" );  // throws -- null is not an int
auto d = v.at( "foo" ).optional< int >( "bar" );  // throws -- 4 is not an Object
```

The following member functions of class `tao::json::value` bypass the Type Traits and provide direct access to the data in a Value.
They all throw an exception when the type of the Value is not the expected one.

| Value Class Member Function | Remarks |
|-----------------------------|---------|
| `bool get_boolean() const` ||
| `std::int64_t get_signed() const` ||
| `std::uint64_t get_unsigned() const` ||
| `double get_double() const` ||
| `std::string& get_string()` ||
| `const std::string& get_string() const` ||
| `tao::string_view get_string_view() const` ||
| `tao::string_view get_string_type() const` | `type::STRING` and `type::STRING_VIEW` |
| `std::vector< tao::byte >& get_binary()` ||
| `const std::vector< tao::byte >& get_binary() const` ||
| `tao::binary_view get_binary_view() const` ||
| `tao::binary_view get_binary_type() const` | `type::BINARY` and `type::BINARY_VIEW` |
| `std::vector< value >& get_array()` ||
| `const std::vector< value >& get_array() const` ||
| `std::map< std::string, value >& get_object()` ||
| `const std::map< std::string, value >& get_object() const` ||
| `const value* get_value_ptr() const` ||

For particular templated code the `get<>()` accessor function that is templated over the `tao::json::type`-enumeration can be used.
It behaves just like the correspondingly named get function, i.e. `get< tao::json::type::STRING >` is the same as `get_string()`.

```c++
tao::json::value v = "hallo";
const std::string s1 = v.get_string();
const std::string s2 = v.get< tao::json::type::STRING >();
assert( s1 == s2 );  // OK
```

## Manipulating Values

Values can always (unless of type `tao::json::DESTROYED`, which should never be encountered in code that doesn't invoke unspecified or undefined behaviour) be assigned a new value using copy-assignment or move-assignment, and the contents of two Values can be swapped with the `tao::json::value::swap()` member function.

Mutable references to standard containers returned by the mutable getters above can be normally manipulated manually and with standard algorithms etc.

Like the equivalent constructor, the assignment operators that accept any type use the Type Traits to assign the Value from the argument.

The following member functions of class `tao::json::value` bypass the Type Traits and directly assign to the underlying data in a Value.

| Value Class Member Function | Remarks |
|-----------------------------|---------|
| `void set_null() noexcept` ||
| `void set_boolean( const bool ) noexcept` ||
| `void set_signed( const std::int64_t ) noexcept` ||
| `void set_unsigned( const std::uint64_t ) noexcept` ||
| `void set_double( const double ) noexcept` ||
| `void set_string( std::string&& ) noexcept` ||
| `void set_string( const std::string& )` ||
| `void set_string_view( const tao::string_view ) noexcept` ||
| `template< typename... Ts > void emplace_string( Ts&&... )` | `noexcept` depending on arguments |
| `void set_binary( std::vector< tao::byte >&& ) noexcept` ||
| `void set_binary( const std::vector< tao::byte >& )` ||
| `void set_binary_view( const tao::binary_view ) noexcept` ||
| `template< typename... Ts > void emplace_binary( Ts&&... )` | `noexcept` depending on arguments |
| `void set_array( std::vector< value >&& ) noexcept` ||
| `void set_array( const std::vector< value >& )` ||
| `template< typename... Ts > void emplace_array( Ts&&... )` | `noexcept` depending on arguments |
| `void set_object( std::map< std::string, value >&& ) noexcept` ||
| `void set_object( const std::map< std::string, value >& )` ||
| `template< typename... Ts > void emplace_object( Ts&&... )` | `noexcept` depending on arguments |
| `void set_value_ptr( const value* ) noexcept` ||
| `template< typename T > void set_opaque_ptr( T* ) noexcept` ||
| `template< typename T > void set_opaque_ptr( T*, const producer_t ) noexcept` ||

Remember that the functions `tao::json::value::get_array()` and `tao::json::value::get_object()` are actually `tao::json::basic_value< Traits, Base >::get_array()` and `tao::json::basic_value< Traits, Base >::get_object()`, and that the sub-values of the returned containers have the same `Traits` and `Base` class as the Value on which the method was invoked.

```c++
tao::json::value x = tao::json::empty_array;
std::vector< tao::json::value >& a = x.get_array();

tao::json::basic_value< my_traits > y = tao::json::empty_array;
std::vector< tao::json::basic_value< my_traits > >& b = y.get_array();

tao::json::basic_value< my_traits, my_base > z = tao::json::empty_array;
std::vector< tao::json::basic_value< my_traits, my_base > >& c = z.get_array();
```

Similarly, when assigning to a Value, or adding further sub-values to a Value representing an Array or Object, Values with the same Traits and Base need to be used.

## Container Functions

There are several convenience functions that simplify access and manipulation of Values representing Arrays and Objects.

The following functions have several overloads and can be called with a `std::size_t` on Values representing an Array, a `std::string` for Values representing an Object, or with a [JSON Pointer](Extended-Use-Cases#json-pointer).

When called with a [JSON Pointer](Extended-Use-Cases#json-pointer), the type of index, integer or string, must correspond to the type of container, Array or Object, on all steps along the path.

The `tao::json::value::at()` function indexes Arrays and Objects via `std::vector::at()` and `std::map::at()`, respectively.
When called with a [JSON Pointer](Extended-Use-Cases#json-pointer), the contents of the pointer must be consistent with the type of Value.

Similarly `tao::json::value::operator[]` indexes Arrays and Objects via `std::vector::operator[]` and `std::map::operator[]`, respectively.
For Objects, it keeps the semantics of the underlying `std::map` and inserts a default-constructed Value when the key string is not found.

The `tao::json::value::erase()` function erases a sub-value of an Array or Object via `std::vector::erase()` and `std::map::erase()`, respectively.

The `tao::json::value::find()` function can only be called with a `std::size_t` or a `std::string` for Arrays and Objects, respectively.
It returns a plain pointer to the sub-value, or `nullptr` when no matching entry was found.
As usual there are both a const overload that returns a `const value*`, and a non-const overload that returns a `value*`.

The `tao::json::value::optional< T >()` function exists in two versions, one without arguments [explained above](#accessing-values), and one that can take any argument that `tao::json::value::find()` can.
The latter returns an empty optional when no entry is found; it does *not* return an empty optional when a JSON Null entry is found but throws an exception when the found entry can not be converted to `T` via `tao::json::value::as< T >()`.

The following functions include calls to `tao::json::value::prepare_array()` and `tao::json::value::prepare_object()` which are used on empty (`tao::json::type::UNITIALIZED` or `tao::json::type::DISCARDED`) Values to initialise to an empty array, or empty object, respectively.

The `tao::json::value::push_back()` and `tao::json::value::emplace_back()` functions for Arrays, and `tao::json::value::insert()` and `tao::json::value::emplace()` for Objects, again forward to the corresponding functions of the underlying standard container.

A more general version of `tao::json:value::insert()` inserts the JSON Value from the second argument in the position indicated by the JSON Pointer in its first argument.

To add multiple entries to an Array or Object with a single function call use `tao::json::value::append()` with Arrays, and `tao::json::value::insert()` with Objects.
Both take an initialiser-list, `tao::json::value::append()` one with only values like `static tao::json::value::array()`, and `tao::json::value::insert()` one with keys and values like the constructor that creates an Object from an initialiser-list does.

## Comparing Values

The library contains the full complement of comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=` ) to compare Values with other Values, and to compare Values with any other C++ type.

Comparison of two Values performs a structural recursive comparison ignoring both the Type Traits and the base class template parameters.
The comparison is performed at the data model level, abstracting away from implementation details and optimisations.

* Numbers are compared by value, independent of which of the possible representations they use.
* Strings and string views are compared by comparing character sequences, independent of which representation is used.
* Binary data and binary views are compared by comparing byte sequences, independent of which representation is used.
* Value Pointers are skipped, the comparison behaves "as if" the pointee Value were copied to the Value with the pointer.
* Values of different incompatible types will be ordered by the numeric Values of their type enum.

Comparison between a JSON Value and another type is performed by either

1. creating a Value object from the other type and performing the comparison between the two Value, or
2. using the [Type Traits](Type-Traits.md) to directly perform the comparison without creating a temporary Value.

See the [documentation on Type Traits](Type-Traits.md#compare-value-with-type) for details on how to enable comparison operators without temporary Values for a type.

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
