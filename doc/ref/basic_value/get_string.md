### `tao::json::basic_value::get_string`

```c++
std::string& get_string(); // (1)
const std::string& get_string() const; // (2)
std::string& unsafe_get_string(); // (3)
const std::string& unsafe_get_string() const; // (4)
```

Get a reference to the embedded string, with (1)&(2) or without (3)&(4) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. -
3. `type() == json::type::STRING`
4. `type() == json::type::STRING`

#### Return value

A const or non-const reference to the embedded `std::string`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::STRING`
2. `std::logic_error` if `type() != json::type::STRING`
3. `noexcept`
4. `noexcept`

#### Complexity

Constant
