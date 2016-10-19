### `tao::json::basic_value::operator=`

```c++
basic_value& operator=( basic_value other ); // (1)
```

Replaces the contents of the instance.

1. Replaces the contents with that of `other`.

#### Return value

`*this`

#### Exceptions

1. `noexcept`

#### Complexity

1. Linear to the number of nodes destructed

#### Notes

Both the `noexcept` specification as well as the complexity in C++ refers to the implementation of the method, *not* to the construction of the method's parameter(s). As the parameter is taken by-value, the construction of the parameter might throw exceptions as well as having a different complexity.

#### See also

[(constructor)](basic_value.md)
