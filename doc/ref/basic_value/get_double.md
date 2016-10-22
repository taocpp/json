### `tao::json::basic_value::get_double`

```c++
double get_double() const; // (1)
double unsafe_get_double() const; // (2)
```

Access the embedded floating point value, with (1) or without (2) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. `type() == json::type::DOUBLE`

#### Return value

Returns the value of the embedded `double`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::DOUBLE`
2. `noexcept`

#### Complexity

Constant
