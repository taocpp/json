# Advanced Use Cases

* [Advanced Binding Options](#advanced-binding-options)
* [Custom Value Annotations](#custom-value-annotations)
* [Annotate with filename and line number](#annotate-with-filename-and-line-number)
* [Instance sharing with Value Pointers](#instance-sharing-with-value-pointers)
* [Instance sharing with Opaque Pointers](#instance-sharing-with-opaque-pointers)
* [Making Values Self Contained](#making-values-self-contained)
* [Unsafe Value Class Functions](#unsafe-value-class-functions)
* [Poylymorphic Object Factory](#polymorphic-object-factory)
* [Disjunction of Single Type Traits](#disjunction-of-single-type-traits)

## Advanced Binding Options

The [binding traits facilities](Type-Traits.md#binding-traits-facilities) support the following options beyond what is explained on the page on [Type Traits](Type-Traits.md).

TODO: constants

TODO: basic_object

TODO: getters

## Custom Value Annotations

The class template `tao::json::basic_value<>` has two template parameters, the type traits, and a `Base` class that it (privately) inherits from.

This allows applications to add arbitrary custom data to every single JSON Value instance, including recursively in all nested sub-values contained in Arrays and Objects.

The library preserves the contents of the base class, and otherwise ignores it.
More precisely:

* The base is copied when the value is copy constructed or assigned.
* The base is moved when the value is move constructed or assigned.
* Otherwise the base is default initialised when constructing a value.
* The base is ignored by the default comparison operators.

Access to the `Base` instance of a value is given through two accessor functions named `base()`.

```c++
template< template< typename... > class Traits, class Base >
Base& basic_value< Traits, Base >::base() noexcept;

template< template< typename... > class Traits, class Base >
const Base& basic_value< Traits, Base >::base() const noexcept;
```

The application can use the base class for whatever it wants, for example to add filenames and line numbers to parsed values as shown below.

## Annotate with filename and line number

A frequent use case for Value annotations is to add the filename, line number, and starting byte position to every (sub-)value during parsing.

The two functions `tao::json::parse_file_with_position()` and `tao::json::basic_parse_file_with_position()` in `include/tao/json/contrib/position.hpp` do just that and can be directly used, or customised as required.

The (private) base class of type `tao::json::position` is defined in the same header.
The following example shows how to read a filename, and, if the file contains a JSON array, prints the line numbers each array element starts on.

```c++
#include <iostream>

#include <tao/json.hpp>
#include <tao/json/contrib/position.hpp>

void example( const std::string& filename )
{
   const auto v = tao::json::parse_file_with_position( filename );
   if( v.is_array() ) {
      for( const auto& i : v.unsafe_get_array() ) {
         std::cout << "element on line " << i.base().line << std::endl;
      }
   }
}
```

Writing a recursive function that takes any value with positions and prints the types and line numbers of all sub-values is left as an exercise to the reader.

## Instance sharing with Value Pointers

The JSON Value class has value semantics, copy construction and copy assignment result in a deep copy.
Similarly when adding a Value as sub-value to a Value representing a JSON Array or Object, if the source can not be moved then a deep copy is made.

```c++
const tao::json::value& big = some_big_json_value();
const tao::json::value log = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "big_data", big }
};
```

With Value Pointers, a (sub-)value can contain a plain and non-owning pointer to another Value instead of copying the pointee.
This performance optimisation is safe as long as the pointee does not go out of scope before the value containing the Value Pointer does.

Values with Value Pointers behave mostly "as if" they contained a deep copy of the pointee, serialising, comparing etc. automatically and transparently dereference all Value Pointers they encounter.

The above example is changed to use a Value Pointer, instead of copying, by passing a pointer to `big` instead of its value/reference.

```c++
const tao::json::value& big = some_big_json_value();
const tao::json::value log = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "big_data", &big }
};
```

Value Pointers MUST NOT contain a `nullptr`.
Usually a JSON Null is used instead of a `nullptr`.

## Instance sharing with Opaque Pointers

Constructing a JSON Value from a C++ type uses the [traits](Type-Traits.md)' `assign()` function for the type to build the corresponding data structure.
When the type has a large JSON representation this can be an expensive operation.

When the only reason to build the JSON Value is to use it with an Events Consumer, typically to produce JSON text or one of the other supported formats, the `tao::json::produce()` function can be directly used in conjunction with the target Consumer, bypassing creation of the JSON Value.

```c++
const my_data& big = some_data_with_big_json_representation();
const auto json = tao::json::produce< my_traits >( big );
```

The above code snippet assumes the `my_traits` class template from the [Type Traits](Type-Traits.md) page, and requires the presence of an appropriate `produce()` function in the traits specialisation for `my_data`.

The situation changes when `big` is not alone.

```c++
const my_data& big = some_data_with_big_json_representation();
const tao::json::value log = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "big_data", big }
};
const auto json = tao::json::to_string( log );
```

Opaque pointers exist to optimise the above creation of `json` by not requiring the JSON structure for `big` to be created.

The requirements to create an Opaque pointer to `my_data` are:

1. A `produce()` function in the traits for `my_data` (as shown [here](Type-Traits.md)).
2. An `assign()` function in the traits for `const my_data*` (as shown below).

As an alternative to 1, a function with the same signature as `produce()` can be supplied to `tao::json::basic_value::unsafe_assign_opaque_ptr()` as second argument.

As an alternative to 2, `unsafe_assign_opaque_ptr()` can be called in other ways.
Or, when dealing with mutable `my_data` instances , it might be necessary to specialise the traits for `my_data*`, too.

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

Just as for Value Pointers, the key to creating an Opaque pointer is again the address-of operator `&`, creating this more efficient code.

```c++
const my_data& big = some_data_with_big_json_representation();
const tao::json::value log = {
   { "app_id", "my_app" },
   { "log_id", 103152 },
   { "message", "successfully processed" },
   { "big_data", &big }
};
const auto json = tao::json::to_string( log );
```

Generating Events from a JSON Value with Opaque pointers will produce the same result as replacing all Opaque pointers with their corresponding JSON structure and then generating Events from the resulting JSON Value would (assuming that the traits' `produce()` function is consistent with its `assign()`).
In the example, the generated JSON text will be the same as in the first version above.

When inspecting a JSON Value, an Opaque pointer will report its `type()` as `tao::json::type::OPAQUE_PTR`.

The limitations of using an Opaque pointer are:

1. The pointer MUST NOT be `nullptr`.
2. Comparison operators MUST NOT be used with Opaque pointers.
3. They can't be inspected or visited much beyond checking their type.

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

## Making Values Self Contained

A JSON Value can contain references to data that it does not manage.

* Values of type `STRING_VIEW` or `BINARY_VIEW` reference memory managed elsewhere.
* Values of type `VALUE_PTR` and `OPAQUE_PTR` reference objects managed elsewhere.

(The views and pointers in sub-values of a Value could be pointing to memory or objects elsewhere in the same Value object, but this case is not recognised or optimised for.)

The function `tao::json::is_self_contained()` can be used to check whether a JSON Value is fully self contained.
It recursively check the entire JSON Value and all sub-values and returns `false` if it finds a sub-value of type `STRING_VIEW`, `BINARY_VIEW`, `VALUE_PTR` or `OPAQUE_PTR`.

The function `tao::json::make_self_contained()` can be used to make a JSON Value fully self contained.
Sub-values of type `STRING_VIEW` and `BINARY_VIEW` are transformed into `STRING` and `BINARY`, respectively.
Occurrences of `VALUE_PTR` are replaced with a deep copy of the pointee.

Occurrences of `OPAQUE_PTR` are replaced with a JSON Value created by using the `produce()` function from the traits as Events producer (or the function supplied as second argument to `basic_value::unsafe_assign_opaque_ptr()`) together with `tao::json::events::to_basic_value` as Events consumer.

## Unsafe Value Class Functions

Some of the member functions of class `tao::json::value` are also available in an "unsafe" version that, in the name of efficiency, makes certain assumptions or omits certain checks.

The accessors like `tao::json::value::get_boolean()` have an unsafe version `tao::json::value::unsafe_get_boolean()` that does **not** check the type.
Instead of throwing an exception on type mismatch it will return bogus data, crash the application or do some other *undefined* behaviour.

The assign methods like `tao::json::value::assign_boolean()` have an unsafe version `tao::json::value::unsafe_assign_boolean()` that does not check whether the previous value has a non-trivial destructor that needs to be called before being overwritten.

TODO: List all other unsafe functions.

## Polymorphic Object Factory

TODO: binding::factory

## Disjunction of Single Type Traits

TODO: binding::versions

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
