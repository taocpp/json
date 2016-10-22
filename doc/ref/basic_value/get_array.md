### `tao::json::basic_value::get_array`

```c++
array_t& get_array(); // (1)
const array_t& get_array() const; // (2)
array_t& unsafe_get_array(); // (3)
const array_t& unsafe_get_array() const; // (4)
```

Get a reference to the embedded vector of nested `basic_string` values, with (1)&(2) or without (3)&(4) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. -
3. `type() == json::type::ARRAY`
4. `type() == json::type::ARRAY`

#### Return value

A const or non-const reference to the embedded `std::vector<basic_value>`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::ARRAY`
2. `std::logic_error` if `type() != json::type::ARRAY`
3. `noexcept`
4. `noexcept`

#### Complexity

Constant
