# Quick Start

A few short indications on how to use this library:

* Requires Clang 3.5+ or GCC 4.8+ with `-std=c++11`, or Visual Studio 2017, or other compilers with sufficient C++11 support.
* The library is header-only, to install and use simply copy the directory [`include/tao`](include/tao) to a convenient place and include the file [`include/tao/json.hpp`](include/tao/json.hpp).
* The generic JSON value class is in [`include/tao/json/value.hpp`](include/tao/json/value.hpp).
* To parse a JSON string representation to a JSON Value, use one of the functions in [`include/tao/json/from_string.hpp`](include/tao/json/from_string.hpp) or [`include/tao/json/parse_file.hpp`](include/tao/json/parse_file.hpp).
* To produce a JSON string representation from a JSON Value, use one of the appropriate functions in [`include/tao/json/to_string.hpp`](include/tao/json/to_string.hpp) or [`include/tao/json/stream.hpp`](include/tao/json/stream.hpp).

The taocpp/json library is built around two main concepts, the [*Value class*](Value-Class.md) and the [*Events interface*](Events-Interface.md).

### How do I read a JSON file?

```c++
#include <tao/json/parse_file.hpp>

const tao::json::value v = tao::json::parse_file( "filename.json" );
```
