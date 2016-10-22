### `tao::json::basic_value::at`

```c++
basic_value& at( std::size_t pos ); // (1)
const basic_value& at( std::size_t pos ) const; // (2)
basic_value& at( const std::string& key ); // (3)
const basic_value& at( const std::string& key ) const; // (4)

basic_value& at( const json::pointer& jp ); // (5)
const basic_value& at( const json::pointer& jp ) const; // (6)

basic_value& unsafe_at( std::size_t pos ); // (7)
const basic_value& unsafe_at( std::size_t pos ) const; // (8)
basic_value& unsafe_at( const std::string& key ); // (9)
const basic_value& unsafe_at( const std::string& key ) const; // (10)
```

1), 2): Returns a reference to the array element at specified location `pos`, with bounds checking.
3), 4): Returns a reference to the mapped value of the object element with key equivalent to `key`, with bounds checking.
5), 6): Returns a reference to the element specified by the JSON Pointer `js`, with bounds checking.
7), 8): Returns a reference to the array element at specified location `pos`.
9), 10): Returns a reference to the mapped value of the object element with key equivalent to `key`.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. -
3. -
4. -
5. -
6. -
7. `type() == json::type::ARRAY` and `pos` must be within range
8. `type() == json::type::ARRAY` and `pos` must be within range
9. `type() == json::type::OBJECT` and an element with key equivalent to `key` must exist
10. `type() == json::type::OBJECT` and an element with key equivalent to `key` must exist

#### Return value

A reference to the specified element.

#### Exceptions

1. `std::logic_error` if `type() != json::type::ARRAY`, `std::out_of_range` if `pos < get_array().size()`
2. `std::logic_error` if `type() != json::type::ARRAY`, `std::out_of_range` if `pos < get_array().size()`
3. `std::logic_error` if `type() != json::type::OBJECT`, `std::out_of_range` if the container does not have an element with the specified key.
4. `std::logic_error` if `type() != json::type::OBJECT`, `std::out_of_range` if the container does not have an element with the specified key.
5. TODO
6. TODO
7. `noexcept`
8. `noexcept`
9. `noexcept`
10. `noexcept`

#### Complexity

1. Constant
2. Constant
3. Logarithmic in the size of the object container
4. Logarithmic in the size of the object container
5. TODO
6. TODO
7. Constant
8. Constant
9. Logarithmic in the size of the object container
10. Logarithmic in the size of the object container
