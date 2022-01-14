# Common Use Cases

* [Include the right header](#include-the-right-header)
* [Read any JSON file](#read-any-json-file)
* [Parse any JSON string](#parse-any-json-string)
* [Serialise a JSON Value to a string](#serialise-a-json-value-to-a-string)
* [Serialise with base64 strings for binary data](#serialise-with-base64-strings-for-binary-data)
* [Serialise a JSON Value to an ostream](#serialise-a-json-value-to-an-ostream)
* [Create a Value with a JSON object](#create-a-value-with-a-json-object)

## Include the right header

The convenience include `<tao/json.hpp>` will include the entire core library.
Additionally, it might be useful to include `<tao/json/contrib/traits.hpp>` to add some commonly used Traits specialisations, mostly for containers and smart pointers.

```c++
#include <tao/json.hpp>
#include <tao/json/contrib/traits.hpp>
```

(To improve compile times, you can choose to directly use only some of the individual headers that it includes.)

## Read any JSON file

```c++
const tao::json::value v = tao::json::from_file( "filename.json" );
```

## Parse any JSON string

```c++
const tao::json::value v = tao::json::from_string( "{ \"a\": 1.0, \"b\": 2.0 }" );
```

## Serialise a JSON Value to a string

We will assume that `v` is a valid JSON Value instance.

```c++
const std::string j = tao::json::to_string( v );
```

Passing a `std::size_t` as second argument enable pretty-printing mode with the given size as indent.

Passing a `std::string` as third argument uses the given string as line ending instead of the `\n` default.

## Serialise with base64 strings for binary data

The JSON format does not allow for binary data.
Binary data in Values needs to be converted into something else when serialising to JSON.
This can be done on-the-fly with an [Events Transformer](Events-Interface.md#overview) like `tao::json::events::binary_to_base64`.

```c++
const std::string l = tao::json::to_string< tao::json::events::binary_to_base64 >( v );
```

Another similar common use case is to use a Transformer like `tao::json::events::non_finite_to_null` to convert non-finite floating-point values (not-a-number, infinity) to something that fits in the JSON data model.

See the [Events interface](Events-Interface.md) documentation for [all available Event Transformers](Events-Interface.md#included-transformers).

## Serialise a JSON Value to an ostream

```c++
tao::json::to_stream( std::cout, v );
```

The additional function and template arguments are just like for `to_string()`.

```c++
using namespace tao::json;

to_stream< events::binary_to_base64, events::non_finite_to_null >( std::cout, v, 3, "\r\n" );
```

When no Events Transformers are needed the usual `operator<<` overloads can be used, too.

```c++
std::cout << v << std::endl;  // Compact.
std::cout << std::setw( 2 ) << v << std::endl;  // Pretty-printed with indent 2.
```

## Create a Value with a JSON object

### First possibility

Parse a string that represents a JSON object, [see above](#parse-any-json-string).

This works similarly with files, and with strings and files in other supported formats.

### Second possibility

Use the literal string operator to parse a literal string.

```c++
using namespace tao::json;

const auto v = "{ \"a\": 1.0, \"b\": 2.0 }"_json;
```

### Third possibility

Use the `std::initalizer_list<>` constructor.

```c++
const tao::json::value v = {
   { "a", 1.0 },
   { "b", 2.0 }
};
```

### Fourth possibility

Use the appropriate value member functions.

```c++
int main()
{
   tao::json::value v;
   v.emplace( "a", 1.0 );
   v.emplace( "b", 2.0 );
   return 0;
}
```

Note that `v.emplace()` will work both on values that already represent an Object *and* on values that are in state `UNINITIALIZED` or `DISCARDED`.

### Fifth possibility

Use an Event Consumer to feed Events to a value.

```c++
int main()
{
   tao::json::events::to_value consumer;
   consumer.begin_object();
   consumer.key( "a" );
   consumer.number( 1.0 );
   consumer.member();
   consumer.key( "b" );
   consumer.number( 2.0 );
   consumer.member();
   consumer.end_object();
   const tao::json::value v = std::move( consumer.value );
   return 0;
}
```

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
