### `tao::json::basic_value::~basic_value`

```c++
~basic_value(); // (1)
```

Destructs the instance. The destructors of the nested values (if any) are called and the used storage is deallocated. Note, that if the values are pointers (`type::RAW_PTR`), the pointed-to objects are not destroyed.

#### Exceptions

1. `noexcept`

#### Complexity

1. Linear to the number of nodes
