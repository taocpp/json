### `tao::json::basic_value::get_signed`

```c++
std::int64_t get_signed() const; // (1)
std::int64_t unsafe_get_signed() const; // (2)
```

Access the embedded integer, with (1) or without (2) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. `type() == json::type::SIGNED`

#### Return value

Returns the value of the embedded `std::int64_t`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::SIGNED`
2. `noexcept`

#### Complexity

Constant
