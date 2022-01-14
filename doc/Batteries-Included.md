# Batteries Included

* [Default Traits](#default-traits)
* [Additional Traits](#additional-traits)
* [Binding Traits](Binding-Traits.md)
* [Events Producers](#events-producers)
* [Events Consumers](#events-consumers)
* [Events Transformers](#events-transformers)
* [Convenience Functions](#convenience-functions)
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

## Events Producers

Events Producers are functions that take some input from which they generate Events function calls on a provided Consumer.

**The common namespace prefix `tao::json::` is omitted.**

| Producer | Description |
| -------- | ----------- |
| `events::from_stream` | Reads JSON from a `std::istream`. |
| `events::from_string` | Reads JSON from a `std::string`. |
| `events::from_value` | Generates Events for a `basic_value<>`. |
| `events::from_file` | Reads JSON from the file. |
| `jaxn::events::from_stream` | Reads JAXN from a `std::istream`. |
| `jaxn::events::from_string` | Reads JAXN from a `std::string`. |
| `jaxn::events::from_file` | Reads JAXN from the file. |
| `cbor::events::from_string` | Reads CBOR from a `std::string`. |
| `cbor::events::from_file` | Reads CBOR from the file. |
| `msgpack::events::from_string` | Reads MSGPACK from a `std::string`. |
| `msgpack::events::from_file` | Reads MSGPACK from the file. |
| `ubjson::events::from_string` | Reads UBJSON from a `std::string`. |
| `ubjson::events::from_file` | Reads UBJSON from the file. |

## Events Consumers

Events Consumers are structs or classes that implement (a complete subset of) the Events Interface.
Consumers are usually instantiated and then passed to an Events Producer function as argument.

**The common namespace prefix `tao::json::` is omitted.**

| Consumer | Description |
| -------- | ----------- |
| `events::debug` | Writes all Events to a `std::ostream` in human-readable form. |
| `events::discard` | Does nothing with the Events it receives. |
| `events::hash` | Calculates a SHA-256 hash for the received Events. |
| `events::statistics` | Counts Events, and string and binary lengths. |
| `events::to_pretty_stream` | Writes nicely formatted JSON to a `std::ostream`. |
| `events::to_stream` | Writes compact JSON to a `std::ostream`. |
| `events::to_string` | Writes compact JSON to a `std::string`. |
| `events::to_value` | Builds a Value with the received Events. |
| `events::validate_event_order` | Checks whether the order of received Events is consistent. |
| `jaxn::events::to_pretty_stream` | Writes nicely formatted JAXN to a `std::ostream`. |
| `jaxn::events::to_stream` | Writes compact JAXN to a `std::ostream`. |
| `jaxn::events::to_string` | Writes compact JAXN to a `std::string`. |
| `cbor::events::to_stream` | Writes CBOR to a `std::ostream`. |
| `cbor::events::to_string` | Writes CBOR to a `std::string`. |
| `msgpack::events::to_stream` | Writes MSGPACK to a `std::ostream`. |
| `msgpack::events::to_string` | Writes MSGPACK to a `std::string`. |
| `ubjson::events::to_stream` | Writes UBJSON to a `std::ostream`. |
| `ubjson::events::to_string` | Writes UBJSON to a `std::string`. |

## Events Transformers

Transformers are structs or classes that are both Event Consumers that implement the Events Interface functions, and Event Producers that call the Events Interface functions on one or more other Events Consumers.

**The common namespace prefix `tao::json::` is omitted.**

| Transformer | Description |
| ----------- | ----------- |
| `events::binary_to_base64` | Passes through all Events except for binary data which is converted to base64-encoded strings. |
| `events::binary_to_base64url` | Passes through all Events except for binary data which is converted to URL-safe base64-encoded strings. |
| `events::binary_to_exception` | Passes through all Events except for binary data which provokes an exception. |
| `events::binary_to_hex` | Passes through all Events except for binary data which is is converted to hex-dumped strings. |
| `events::key_camel_case_to_snake_case` | Passes through all Events except for keys in Objects which are converted from "CamelCaseStyle" to "snake_case_style". |
| `events::key_snake_case_to_camel_case` | Passes through all Events except for keys in Objects which are converted from "snake_case_style" to "CamelCaseStyle". |
| `events::limit_nesting_depth` | Passes through all Events but throws an exception when the nesting of Arrays and Objects exceeds a limit. |
| `events::limit_value_count` | Passes through all Events but throws an exception when the total number of (sub)-value exceeds a limit. |
| `events::events::non_finite_to_exception` | Passes through all Events except for numbers of type `double` which contain non-finite values which provoke an exception. |
| `events::non_finite_to_null` | Passes through all Events except for numbers of type `double` which contain non-finite values which are passed on as `null()`. |
| `events::non_finite_to_string` | Passes through all Events except for numbers of type `double` which contain non-finite values which are passed on as appropriate `string()`. |
| `events::prefer_signed` | Passes through all Events except for numbers of type `std::uint64_t` which fit into a `std::int64_t` and are passed on as such. |
| `events::prefer_unsigned` | Passes through all Events except for numbers of type `std::int64_t` which fit into a `std::uint64_t` and are passed on as such. |
| `events::ref` | Passes all Events to another Consumer or Transformer to which it holds a C++ reference. |
| `events::tee` | Passes all Events to an arbitrary number of other Consumers or Transformers which it holds in a `std::tuple<>`. |
| `events::validate_keys` | Passes through all Events except for keys which are first validated against a provided PEGTL grammar rule. |
| `events::virtual_ref` | Like `ref`, but implements the virtual Event functions from `virtual_base`. |

## Convenience Functions

Functions that combine the similarly named Events Producer from above with an Events Consumer to return a corresponding Value.

**The common namespace prefix `tao::json::` is omitted.**

| Function | Description |
| -------- | ----------- |
| `from_stream` | Reads JSON from a `std::istream`. |
| `from_string` | Reads JSON from a `std::string`. |
| `from_file` | Reads JSON from the file. |
| `jaxn::from_stream` | Reads JAXN from a `std::istream`. |
| `jaxn::from_string` | Reads JAXN from a `std::string`. |
| `jaxn::from_file` | Reads JAXN from the file. |
| `cbor::from_string` | Reads CBOR from a `std::string`. |
| `cbor::from_file` | Reads CBOR from the file. |
| `msgpack::from_string` | Reads MSGPACK from a `std::string`. |
| `msgpack::from_file` | Reads MSGPACK from the file. |
| `ubjson::from_string` | Reads UBJSON from a `std::string`. |
| `ubjson::from_file` | Reads UBJSON from the file. |

Functions that combine the similarly named Events Consumers with a Producer that generates Events from a Value.

**The common namespace prefix `tao::json::` is omitted.**

| Function | Description |
| -------- | ----------- |
| `to_stream` | Writes compact or nicely formatted JSON to a `std::ostream`. |
| `to_string` | Writes compact or nicely formatted JSON to a `std::string`. |
| `jaxn::to_stream` | Writes compact or nicely formatted JAXN to a `std::ostream`. |
| `jaxn::to_string` | Writes compact or nicely formatted JAXN to a `std::string`. |
| `jaxn::to_stream` | Writes CBOR to a `std::ostream`. |
| `jaxn::to_string` | Writes CBOR to a `std::string`. |
| `msgpack::to_stream` | Writes MSGPACK to a `std::ostream`. |
| `msgpack::to_string` | Writes MSGPACK to a `std::string`. |
| `ubjson::to_stream` | Writes UBJSON to a `std::ostream`. |
| `ubjson::to_string` | Writes UBJSON to a `std::string`. |

These functions optionally apply an arbitrary list of [Transformers](#events-transformers) given as additional template parameters.

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

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
