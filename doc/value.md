### `tao::json::basic_value`

Defined in `<tao/json/value.hh>`

```c++
template<
   template< typename... > class Traits
> class basic_value;
```

Each instance is a JSON document. Each instance stores the type of the JSON document as well as the value itself. Documents of type `array` or `object` store nested JSON documents. Additionally, an instance may be uninitialized, discarded or it may contain a raw pointer to another value. TODO: Add more...

### Template parameter

`Traits` - Conversion from and to other data types. Can be specialized or replaced by the user.

### Member functions

| Method | Description |
| --- | --- |
| [(constructor)](value_ctor.md) | constructs the value |
| [(destructor)](value_dtor.md) | destructs the value |
| `operator=` | assigns the value |
| **Element type** |
| `type` | the type of the element |
| `is_null` | checks whether the element type is `type::NULL_` |
| `is_boolean` | checks whether the element type is `type::BOOLEAN` |
| `is_signed` | checks whether the element type is `type::SIGNED` |
| `is_unsigned` | checks whether the element type is `type::UNSIGNED` |
| `is_integer` | checks whether the element type is `type::SIGNED` or `type::UNSIGNED` |
| `is_double` | checks whether the element type is `type::DOUBLE` |
| `is_number` | checks whether the element type is an integer or `type::DOUBLE` |
| `is_string` | checks whether the element type is `type::STRING` |
| `is_array` | checks whether the element type is `type::ARRAY` |
| `is_object` | checks whether the element type is `type::OBJECT` |
| `is_raw_ptr` | checks whether the element type is `type::RAW_PTR` |
| **Element access** |
| `empty` | checks whether the element is empty |
| `get_null` | returns the value of the stored `null_t` |
| `get_boolean` | returns the value of the stored `bool` |
| `get_signed` | returns the value of the stored `std::int64_t` |
| `get_unsigned` | returns the value of the stored `std::uint64_t` |
| `get_double` | returns the value of the stored`double` |
| `get_string` | returns the value of the stored `std::string` |
| `get_array` | returns a reference to the underlying `std::vector` |
| `get_object` | returns a reference to the underlying `std::map` |
| `get_raw_ptr` | returns the value of a `const basic_value *` |
| `get` | returns the value or a reference of the underlying container |
| `as` | converts the value to the requested type |
| `optional` | converts the value to the requested type, if possible |
| **Lookup** |
| `at` | access specified element with bounds checking |
| `operator[]` | access specified element |
| `find` | finds element with the specified position |
| **Modifiers** |
| `unsafe_assign` | assigns a value to the element |
| `unsafe_assign_null` | assigns a `null_t` value to the element |
| `unsafe_assign_boolean` | assigns a `bool` value to the element |
| `unsafe_assign_signed` | assigns a `std::int64_t` value to the element |
| `unsafe_assign_unsigned` | assigns a `std::uint64_t` value to the element |
| `unsafe_assign_double` | assigns a `double` value to the element |
| `unsafe_assign_double_unchecked` | assigns an unchecked `double` value to the element |
| `emplace_string` | constructs an underlying `std::string` value in-place |
| `emplace_array` | constructs an underlying `std::vector` value in-place |
| `emplace_object` | constructs an underlying `std::map` value in-place |
| `unsafe_assign_raw_ptr` | assigns a `const basic_value *` value to the element |
| `append` | appends elements at the specified position |
| `insert` | inserts elements at the specified position |
| `prepare_array` | constructs an empty underlying `std::vector` if necessary |
| `emplace_back` | appends a value to the underlying `std::vector` |
| `prepare_object` | constructs an empty underlying `std::map` if necessary |
| `emplace` | inserts a value to the underlying `std::map` |
| `erase` | erases elements with the specified position |
| `swap` | swaps the contents |
| **Other** |
| `discard` | discards the element |
| `reset` | resets the element |
| `skip_raw_ptr` | returns a pointer to the first value skipping all raw pointers |

> Many `unsafe_*`-methods are omitted from the above table and only mentioned within the non-`unsafe_*` method as another "overload".

| **Non-member functions** | Description |
| --- | --- |
| `operator==` <br/> `operator!=` <br/> `operator<` <br> `operator<=` <br> `operator>` <br> `operator>=` | lexicographically compares the elements
| `operator+=` | inserts elements to an object
| `operator-=` | removes elements to an object
