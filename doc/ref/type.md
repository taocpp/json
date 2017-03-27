## `tao::json::type`

Defined in `<tao/json/type.hpp>`

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

For tracking the kind of data currently stored in a [`basic_value`](basic_value.md).

### Constants

| Name | Description |
| --- | --- |
| `UNINITIALIZED` | For [`basic_value`](basic_value.md)s that have not been assigned a value, or that have been `reset`. |
| `DISCARDED` | For [`basic_value`](basic_value.md)s that have been discarded, either explicitly or after being *moved*. |
| `NULL_` | For JSON null values. |
| `BOOLEAN` | For JSON boolean values. |
| `SIGNED` | For JSON numbers stored as `std::int64_t`. |
| `UNSIGNED` | For JSON numbers stored as `std::uint64_t`. |
| `DOUBLE` | For JSON numbers stored as `double`. |
| `STRING` | For JSON strings. |
| `ARRAY` | For JSON arrays. |
| `OBJECT` | For JSON objects. |
| `RAW_PTR` | For [`basic_value`](basic_value.md)s that wrap a plain pointer to a different [`basic_value`](basic_value.md). |
