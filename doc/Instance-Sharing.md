# Instance Sharing

In order to speed up the creation of Values there are multiple possibilities of preventing the copying and building of data and data structures.

* [String Views](#string-views)
* [Binary Views](#binary-views)
* [Value Pointers](#value-pointers)
* [Opaque Pointers](#opaque-pointers)
* [Self Contained](#self-contained)

## String Views

TODO...

## Binary Views

TODO...

## Value Pointers

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

## Opaque Pointers

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

As an alternative to 1, a function with the same signature as `produce()` can be supplied to `tao::json::basic_value::assign_opaque_ptr()` as second argument.

As an alternative to 2, `assign_opaque_ptr()` can be called in other ways.
Or, when dealing with mutable `my_data` instances , it might be necessary to specialise the traits for `my_data*`, too.

```c++
template<>
struct my_traits< const my_data* >
{
   template< template< typename... > class Traits >
   static void assign( basic_value< Traits >& v, const my_data* const d )
   {
      v.assign_opaque_ptr( d );
   }
};
```

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
   template< template< typename... > class Traits >
   static void assign( basic_value< Traits >& v, my_data& d )
   {
      v.assign_opaque_ptr( &d );  // NOT recommended!
   }
};
```

## Self Contained

As we have seen there are several ways in which a JSON Value might reference data that it does not manage.

* Values of type `STRING_VIEW` or `BINARY_VIEW` reference memory managed elsewhere.
* Values of type `VALUE_PTR` and `OPAQUE_PTR` reference objects managed elsewhere.

In some instances it might be necessary to make a self contained version of such a Value, one that is in some sense equivalent to the original but does not contain such unmanaged references.

(The views and pointers in sub-values of a Value could be pointing to memory or objects elsewhere in the same Value object, but this case is not recognised or optimised for.)

The function `tao::json::is_self_contained()` can be used to check whether a JSON Value is fully self contained.
It recursively check the entire JSON Value and all sub-values and returns `false` if it finds a sub-value of type `STRING_VIEW`, `BINARY_VIEW`, `VALUE_PTR` or `OPAQUE_PTR`.

The function `tao::json::make_self_contained()` can be used to make a JSON Value fully self contained.
Sub-values of type `STRING_VIEW` and `BINARY_VIEW` are transformed into `STRING` and `BINARY`, respectively.
Occurrences of `VALUE_PTR` are replaced with a deep copy of the pointee.

Occurrences of `OPAQUE_PTR` are replaced with a JSON Value created by using the `produce()` function from the traits as Events producer (or the function supplied as second argument to `basic_value::assign_opaque_ptr()`) together with `tao::json::events::to_basic_value` as Events consumer.

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
