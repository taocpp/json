### `tao::json::basic_value::reset`

```c++
void reset() const; // (1)
```

Resets the instance. The destructors of the nested values (if any) are called and the used storage is deallocated. Note, that if the values are pointers (`type::RAW_PTR`), the pointed-to objects are not destroyed. The instance is then set into an uninitialized state.

#### Exceptions

1. `noexcept`

#### Complexity

1. Linear to the number of nodes destructed
