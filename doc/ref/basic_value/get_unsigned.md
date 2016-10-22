### `tao::json::basic_value::get_unsigned`

```c++
std::uint64_t get_unsigned() const; // (1)
std::uint64_t unsafe_get_unsigned() const; // (2)
```

Access the embedded integer, with (1) or without (2) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. `type() == json::type::UNSIGNED`

#### Return value

Returns the value of the embedded `std::uint64_t`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::UNSIGNED`
2. `noexcept`

#### Complexity

Constant
