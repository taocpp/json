### `tao::json::basic_value::get_raw_ptr`

```c++
const basic_value* get_raw_ptr() const; // (1)
const basic_value* unsafe_get_raw_ptr() const; // (2)
```

Access the embedded raw pointer, with (1) or without (2) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. `type() == json::type::RAW_PTR`

#### Return value

Returns the value of the embedded `const basic_value*`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::RAW_PTR`
2. `noexcept`

#### Complexity

Constant

#### Note

The pointer itself can be the null pointer.
