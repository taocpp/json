### `tao::json::basic_value::basic_value`

```c++
basic_value(); // (1)

basic_value( basic_value&& other ); // (2)

basic_value( const basic_value& other ); // (3)
basic_value( basic_value& other ); // (3)

basic_value( std::initializer_list<pair<Traits>>&& init ); // (4)

basic_value( const std::initializer_list<pair<Traits>>& init ); // (5)
basic_value( std::initializer_list<pair<Traits>>& init ); // (5)

template< typename T >
basic_value( T&& value ); // (6)
```

Constructs a new `basic_value` from a variety of data sources and optionally using a (user-specialized) `Traits` class.

1. Constructs a `basic_value` with type [`UNINITIALIZED`](../type.md#UNINITIALIZED).
2. Move constructor. Constructs a `basic_value` with the contents of `other` using move semantics.
3. Copy constructor. Constructs a `basic_value` with a copy of the contents of `other`.
4. Constructs a `basic_value` with the contents of `init` using move semantics. Uses (6) for the actual nodes, the initializer list may be nested.
5. Constructs a `basic_value` with the contents of `init`. Uses (6) for the actual nodes, the initializer list may be nested.
6. Constructs a `basic_value` with the value of `value`, using `Traits` to perform the actual mapping from source type to the stored type.

#### Exceptions

1. `noexcept`
2. `noexcept`
3. -
4. -
5. -
6. `noexcept(noexcept(Traits<std::decay_t<T>>::assign(std::declval<basic_value&>(), std::forward<T>(v))))`

#### Complexity

1. Constant
2. Constant
3. Linear to the number of nodes and the size of embedded strings
4. Linear to the number of nodes
5. Linear to the number of nodes and the size of embedded strings
6. Depends on `Traits`

#### Notes

The non-`const` overloads of (3) and (5) are necessary to take priority over (6). Overloads (4) and (5) are used to move the elements from the initializer list instead of copying them in overload (4). This is, given the nested structure of JSON as well as the embedded strings, an important optimization. It might be dangerous when a user *copies* an initializer list and one copy ends up accessing already moved-away instances (as both "copies" reference the same data). The problem is detectable by `taocpp/json` if `NDEBUG` is not set. Move-from instances are set to the type [`DISCARDED`](../type.md#DISCARDED) and trying to move them again (or use them in other ways) will trigger an `assert`.

Usage of the `Traits` class is discussed in more detail in chapter TODO:InsertLink.
