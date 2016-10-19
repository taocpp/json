### `tao::json::type`

Defined in `<tao/json/type.hh>`

```c++
enum class type : std::uint8_t
{
   UNINITIALIZED,
   DISCARDED,
   NULL_,
   BOOLEAN,
   SIGNED,
   UNSIGNED,
   DOUBLE,
   STRING,
   ARRAY,
   OBJECT,
   RAW_PTR
};
```

Specifies which contents is currently stored in a [`basic_value`](basic_value.md).

#### Constants

TODO
