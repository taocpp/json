# Quick Start

A few short indications on how to use this library:

* Requires Clang 3.5+ or GCC 4.8+ with `-std=c++11`, or Visual Studio 2017, or other compilers with sufficient C++11 support.
* The library is header-only, to install and use simply copy the directory [`include/tao`](include/tao) to a convenient place and include the file [`include/tao/json.hpp`](include/tao/json.hpp).
* The generic JSON value class is in [`include/tao/json/value.hpp`](include/tao/json/value.hpp).
* To parse a JSON string representation to a JSON Value, use one of the functions in [`include/tao/json/from_string.hpp`](include/tao/json/from_string.hpp) or [`include/tao/json/parse_file.hpp`](include/tao/json/parse_file.hpp).
* To produce a JSON string representation from a JSON Value, use one of the appropriate functions in [`include/tao/json/to_string.hpp`](include/tao/json/to_string.hpp) or [`include/tao/json/stream.hpp`](include/tao/json/stream.hpp).

The taocpp/json library is built around two main concepts, the [*Value class*](Value-Class.md) and the [*Events interface*](Events-Interface.md).



## How To...

* [...include the right header?](#include-the-right-header)
* [...read a JSON file?](#read-a-json-file)
* [...parse a JSON string?](#parse-a-json-string)
* [...serialise a JSON Value to a string?](#serialise-a-json-value-to-a-string)
* [...serialise with base64 strings for binary data?](#serialise-with-base64-strings-for-binary-data)
* [...serialise a JSON Value to an ostream?](#serialise-a-json-value-to-an-ostream)
* [...create a Value with a JSON object?](#create-a-value-with-a-json-object)

### ...include the right header?

The convenience include `<tao/json.hpp>` will include the whole library.
If you need to improve compile time, you can choose to directly use only some of the individual headers that it includes.

```c++
#include <tao/json.hpp>
```

### ...read a JSON file?

```c++
const tao::json::value v = tao::json::parse_file( "filename.json" );
```

### ...parse a JSON string?

```c++
const tao::json::value v = tao::json::from_string( "{ \"a\": 1.0, \"b\": 2.0 }" );
```

### ...serialise a JSON Value to a string?

We will assume that `v` is a valid JSON Value instance.

```c++
const std::string j = tao::json::to_string( v );
```

Passing a `std::size_t` as second argument enable pretty-printing mode with the given size as indent.

Passing a `std::string` as third argument uses the given string as line ending instead of the `\n` default.

### ...serialise with base64 strings for binary data?

```c++
const std::string l = tao::json::to_string< tao::json::events::binary_to_base64 >( v );
```

See the [Events interface](Events-Interface.md) documentation for all available Event filters.

### ...serialise a JSON Value to an ostream?

```c++
tao::json::to_stream( std::cout, v );
```

The additional function and template arguments are just like for `to_string()`.

```c++
using namespace tao::json;

to_stream< events::binary_to_base64, events::non_finite_to_null >( std::cout, v, 3, "\r\n" );
```

The usual `operator<<` overloads can be used, too.

```c++
std::cout << v << std::endl;  // Compact.
std::cout << std::setw( 2 ) << v << std::endl;  // Pretty-printed with indent 2.
```

### ...create a Value with a JSON object?

First possibility, parse a string that represents an object, see [above](#parse-a-json-string).

Second possibility, use the literal string operator to parse a literal string.

```c++
using namespace tao::json;

const auto v = "{ \"a\": 1.0, \"b\": 2.0 }"_json;
```

Third possibility, use the `std::initalizer_list<>` constructor.

```c++
const tao::json::value v = {
   { "a", 1.0 },
   { "b", 2.0 }
};
```

Fourth possibility, use the appropriate value member functions.

```c++
int main()
{
   tao::json::value v;
   v.emplace( "a", 1.0 );
   v.emplace( "b", 2.0 );
   return 0;
}
```

Fifth possibility, use the appropriate Event consumer.

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

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
