### `tao::json::basic_value::operator=`

```c++
basic_value& operator=( basic_value other ); // (1)
```

Assigns a new `basic_value` from a variety of data sources and optionally using a (user-specialized) `Traits` class.

1. Replaces the contents with that of `other`.

#### Return value

`*this`

#### Exceptions

1. `noexcept`

Note, however, that `noexcept` in C++ refers to the implementation of the method, *not* to the construction of the method's parameter.

#### Complexity

1. Constant
2. Constant
3. Linear to the number of nodes and the size of embedded strings
4. Linear to the number of nodes
5. Linear to the number of nodes and the size of embedded strings
6. Depends on `Traits`

#### Notes

The non-`const` overloads of (3) and (5) are necessary to take priority over (6). Overloads (4) and (5) are used to move the elements from the initializer list instead of copying them in overload (4). This is, given the nested structure of JSON as well as the embedded strings, an important optimization. It might be dangerous when a user *copies* an initializer list and one copy ends up accessing already moved-away instances (as both "copies" reference the same data). The problem is detectable by `taocpp/json` if `NDEBUG` is not set. Move-from instances are set to the type `DISCARDED` and trying to move them again (or use them in other ways) will trigger an `assert`.

Usage of the `Traits` class is discussed in more detail in chapter TODO:InsertLink.

#### See also

[(constructor)](value_ctor.md)
