### `tao::json::basic_value::get_boolean`

```c++
bool get_boolean() const; // (1)
bool unsafe_get_boolean() const; // (2)
```

Access the embedded boolean, with (1) or without (2) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. `type() == json::type::BOOLEAN`

#### Return value

Returns the value of the embedded `bool`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::BOOLEAN`
2. `noexcept`

#### Complexity

Constant
