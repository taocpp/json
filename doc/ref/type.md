## `tao::json::type`

Defined in `<tao/json/type.hpp>`

```c++
enum class type : std::uint8_t
{
   UNINITIALIZED,
   DISCARDED,
   DESTROYED,
   NULL_,
   BOOLEAN,
   SIGNED,
   UNSIGNED,
   DOUBLE,
   STRING,
   STRING_VIEW,
   BINARY,
   BINARY_VIEW,
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
| `DISCARDED` | For [`basic_value`](basic_value.md)s that have been discarded, either explicitly or after being moved. |
| `DESTROYED` | For [`basic_value`](basic_value.md)s that have been destroyed. |
| `NULL_` | For null values. |
| `BOOLEAN` | For boolean values. |
| `SIGNED` | For numbers stored as `std::int64_t`. |
| `UNSIGNED` | For numbers stored as `std::uint64_t`. |
| `DOUBLE` | For numbers stored as `double`. |
| `STRING` | For strings stored as `std::string`. |
| `STRING_VIEW` | For strings stored as `tao::string_view`. |
| `BINARY` | For binary data stored as `std::vector< tao::byte >`. |
| `BINARY_VIEW` | For binary data stored as `tao::byte_view`. |
| `ARRAY` | For arrays. |
| `OBJECT` | For objects. |
| `RAW_PTR` | For [`basic_value`](basic_value.md)s that wrap a plain pointer to a different [`basic_value`](basic_value.md). |
