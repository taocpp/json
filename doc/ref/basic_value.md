### `tao::json::basic_value`

Defined in `<tao/json/value.hpp>`

```c++
template<
   template< typename... > class Traits
> class basic_value;
```

Each instance is a JSON document. Each instance stores the type of the JSON document as well as the value itself. Documents of type `array` or `object` store nested JSON documents. Additionally, an instance may be uninitialized, discarded or it may contain a raw pointer to another value. TODO: Add more...

### Template parameter

`Traits` - Conversion from and to other data types. Can be specialized or replaced by the user.

### Member types

| Member | Description |
| --- | --- |
| `array_t` | Type to store an embedded array, `std::vector<basic_value>` |
| `object_t` | Type to store an embedded object, `std::map<std::string, basic_value>` |

### Member functions

| Method | Description |
| --- | --- |
| [(constructor)](basic_value/basic_value.md) | constructs the value |
| [(destructor)](basic_value/~basic_value.md) | destructs the value |
| [`operator=`](basic_value/operator=.md) | assigns the value |
| **Element type** |
| [`type`](basic_value/type.md) | the type of the element |
| [`is_null`](basic_value/is_null.md) | checks whether the element type is `type::NULL_` |
| [`is_boolean`](basic_value/is_boolean.md) | checks whether the element type is `type::BOOLEAN` |
| [`is_signed`](basic_value/is_signed.md) | checks whether the element type is `type::SIGNED` |
| [`is_unsigned`](basic_value/is_unsigned.md) | checks whether the element type is `type::UNSIGNED` |
| [`is_integer`](basic_value/is_integer.md) | checks whether the element type is `type::SIGNED` or `type::UNSIGNED` |
| [`is_double`](basic_value/is_double.md) | checks whether the element type is `type::DOUBLE` |
| [`is_number`](basic_value/is_number.md) | checks whether the element type is an integer or `type::DOUBLE` |
| [`is_string`](basic_value/is_string.md) | checks whether the element type is `type::STRING` |
| [`is_array`](basic_value/is_array.md) | checks whether the element type is `type::ARRAY` |
| [`is_object`](basic_value/is_object.md) | checks whether the element type is `type::OBJECT` |
| [`is_raw_ptr`](basic_value/is_raw_ptr.md) | checks whether the element type is `type::RAW_PTR` |
| **Element access** |
| [`operator bool`](basic_value/operator_bool.md) | checks whether the element is initialized |
| [`empty`](basic_value/empty.md) | checks whether the element is empty |
| [`get_boolean`](basic_value/get_boolean.md) | returns the value of the stored `bool` |
| [`get_signed`](basic_value/get_signed.md) | returns the value of the stored `std::int64_t` |
| [`get_unsigned`](basic_value/get_unsigned.md) | returns the value of the stored `std::uint64_t` |
| [`get_double`](basic_value/get_double.md) | returns the value of the stored`double` |
| [`get_string`](basic_value/get_string.md) | returns a reference to the stored `std::string` |
| [`get_array`](basic_value/get_array.md) | returns a reference to the underlying `std::vector` |
| [`get_object`](basic_value/get_object.md) | returns a reference to the underlying `std::map` |
| [`get_raw_ptr`](basic_value/get_raw_ptr.md) | returns the value of a `const basic_value *` |
| [`get`](basic_value/get.md) | returns the value or a reference of the underlying container |
| [`as`](basic_value/as.md) | converts the value to the requested type |
| [`optional`](basic_value/optional.md) | converts the value to the requested type, if possible |
| **Lookup** |
| [`at`](basic_value/at.md) | access specified element with bounds checking |
| [`operator[]`](basic_value/operator_at.md) | access specified element |
| [`find`](basic_value/find.md) | finds element with the specified position |
| **Modifiers** |
| [`unsafe_assign`](basic_value/unsafe_assign.md) | assigns a value to the element |
| [`unsafe_assign_null`](basic_value/unsafe_assign_null.md) | assigns a `null_t` value to the element |
| [`unsafe_assign_boolean`](basic_value/unsafe_assign_boolean.md) | assigns a `bool` value to the element |
| [`unsafe_assign_signed`](basic_value/unsafe_assign_signed.md) | assigns a `std::int64_t` value to the element |
| [`unsafe_assign_unsigned`](basic_value/unsafe_assign_unsigned.md) | assigns a `std::uint64_t` value to the element |
| [`unsafe_assign_double`](basic_value/unsafe_assign_double.md) | assigns a `double` value to the element |
| [`unsafe_assign_double_unchecked`](basic_value/unsafe_assign_double_unchecked.md) | assigns an unchecked `double` value to the element |
| [`emplace_string`](basic_value/emplace_string.md) | constructs an underlying `std::string` value in-place |
| [`emplace_array`](basic_value/emplace_array.md) | constructs an underlying `std::vector` value in-place |
| [`emplace_object`](basic_value/emplace_object.md) | constructs an underlying `std::map` value in-place |
| [`unsafe_assign_raw_ptr`](basic_value/unsafe_assign_raw_ptr.md) | assigns a `const basic_value *` value to the element |
| [`append`](basic_value/append.md) | appends elements at the specified position |
| [`insert`](basic_value/insert.md) | inserts elements at the specified position |
| [`prepare_array`](basic_value/prepare_array.md) | constructs an empty underlying `std::vector` if necessary |
| [`emplace_back`](basic_value/emplace_back.md) | appends a value to the underlying `std::vector` |
| [`prepare_object`](basic_value/prepare_object.md) | constructs an empty underlying `std::map` if necessary |
| [`emplace`](basic_value/emplace.md) | inserts a value to the underlying `std::map` |
| [`erase`](basic_value/erase.md) | erases elements with the specified position |
| [`swap`](basic_value/swap.md) | swaps the contents |
| **Other** |
| [`discard`](basic_value/discard.md) | discards the element |
| [`reset`](basic_value/reset.md) | resets the element |
| [`skip_raw_ptr`](basic_value/skip_raw_ptr.md) | returns a pointer to the first value skipping all raw pointers |

> Many `unsafe_*`-methods are omitted from the above table and only mentioned within the non-`unsafe_*` method as another "overload".

| **Non-member functions** | Description |
| --- | --- |
| `operator==` <br/> `operator!=` <br/> `operator<` <br> `operator<=` <br> `operator>` <br> `operator>=` | lexicographically compares the elements
| `operator+=` | inserts elements to an object
| `operator-=` | removes elements to an object
