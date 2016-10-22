### `tao::json::basic_value::get_object`

```c++
object_t& get_object(); // (1)
const object_t& get_object() const; // (2)
object_t& unsafe_get_object(); // (3)
const object_t& unsafe_get_object() const; // (4)
```

Get a reference to the embedded map of nested `basic_string` values, with (1)&(2) or without (3)&(4) checking the type first.

#### Preconditions

The instance must *not* be in a discarded state.

1. -
2. -
3. `type() == json::type::OBJECT`
4. `type() == json::type::OBJECT`

#### Return value

A const or non-const reference to the embedded `std::vector<basic_value>`.

#### Exceptions

1. `std::logic_error` if `type() != json::type::OBJECT`
2. `std::logic_error` if `type() != json::type::OBJECT`
3. `noexcept`
4. `noexcept`

#### Complexity

Constant
