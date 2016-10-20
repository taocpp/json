### `tao::json::basic_value::empty`

```c++
bool empty() const; // (1)
```

Returns whether the instance is considered "empty". An instance is considered empty if it is

* of type [`UNINITIALIZED`](../type.md#UNINITIALIZED), or
* of type [`STRING`](../type.md#STRING) and the embedded `std::string` is empty, or
* of type [`ARRAY`](../type.md#ARRAY) and the embedded `std::vector` is empty, or
* of type [`OBJECT`](../type.md#OBJECT) and the embedded `std::map` is empty, or
* of type [`RAW_PTR`](../type.md#RAW_PTR) and the embedded raw pointer is equal to `nullptr`

#### Preconditions

The instance must *not* be in a discarded state.

#### Return value

`true` when the instance is considered empty, `false` otherwise.

#### Exceptions

1. `noexcept`

#### Complexity

1. Constant
