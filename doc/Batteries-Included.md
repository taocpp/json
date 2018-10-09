# Batteries Included

* [Default Traits](#default-traits)
* [Additional Traits](#additional-traits)
* [Binding Traits](Binding-Traits.md)
* [Conversion Utilities](#conversion-utilities)

## Default Traits

The included Type Traits contain (partial) specialisations for the following types.

| Specialised for | Remarks |
| -------------- | -------- |
| `null_t` | |
| `bool` | |
| *signed integers* | |
| *unsigned integers* | |
| `double`, `float` | |
| `empty_binary_t` | |
| `empty_array_t` | |
| `empty_object_t` | |
| `std::string` | |
| `tao::string_view` | |
| `const char*` | |
| `const std::string&` | |
| `std::vector< tao::byte >` | |
| `tao::binary_view` | |
| `const std::vector< tao::byte >&` | |
| `std::vector< basic_value< Traits, Base > >` | Corresponds to JSON Array. |
| `basic_value< Traits, Base >*` | Creates Value Pointer; must not be `nullptr`. |
| `const basic_value< Traits, Base >*` | Creates Value Pointer; must not be `nullptr`. |
| `std::map< std::string, basic_value< Traits, Base > >` | Corresponds to JSON Object. |
| `tao::optional< T >` | Empty optional corresponds to JSON Null. |

## Additional Traits

The following additional specialisations are part of various additional traits classes.
These classes are defined in various headers in `tao/json/contrib`.
It is also possible to include `tao/json/contrib/traits.hpp` which adds appropriate specialisations to the default traits `tao::json::traits<>`.

| Implementation for | Header |
| -------------- | -------- |
| `std::array< T, N >` | `tao/json/contrib/array_traits.hpp` |
| `std::deque< T >` | `tao/json/contrib/deque_traits.hpp` |
| `std::list< T >` | `tao/json/contrib/list_traits.hpp` |
| `std::map< std::string, T >` | `tao/json/contrib/map_traits.hpp` |
| `std::pair< U, V >` | `tao/json/contrib/pair_traits.hpp` |
| `std::set< T >` | `tao/json/contrib/set_traits.hpp` |
| `std::shared_ptr< T >` | `tao/json/contrib/shared_ptr_traits.hpp` |
| `std::tuple< Ts... >` | `tao/json/contrib/tuple_traits.hpp` |
| `std::unique_ptr< T >` | `tao/json/contrib/unique_ptr_traits.hpp` |
| `std::unordered_map< std::string, T >` | `tao/json/contrib/unordered_map_traits.hpp` |
| `std::unordered_set< T >` | `tao/json/contrib/unordered_set_traits.hpp` |
| `std::vector< T >` | `tao/json/contrib/vector_traits.hpp` |

## Conversion Utilities

The following conversion utilities are included in the `src/examples/json` directory.
Note that they convert between formats on-the-fly, i.e. without creating any intermediate in-memory representation.

```
cbor_to_jaxn
cbor_to_json
cbor_to_msgpack
cbor_to_pretty_jaxn
cbor_to_pretty_json
cbor_to_ubjson
jaxn_to_cbor
jaxn_to_jaxn
jaxn_to_msgpack
jaxn_to_pretty_jaxn
jaxn_to_ubjson
json_to_cbor
json_to_json
json_to_msgpack
json_to_pretty_json
json_to_ubjson
msgpack_to_cbor
msgpack_to_jaxn
msgpack_to_json
msgpack_to_pretty_jaxn
msgpack_to_pretty_json
msgpack_to_ubjson
ubjson_to_cbor
ubjson_to_jaxn
ubjson_to_json
ubjson_to_msgpack
ubjson_to_prety_jaxn
ubjson_to_prety_json
```

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
