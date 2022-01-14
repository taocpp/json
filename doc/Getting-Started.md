# Getting Started

A few short indications on how to use this library:

* Requires Clang 5 or GCC 7 (or later) with `-std=c++17`, or Visual Studio 2017, or other compilers with sufficient C++17 support.
* The library is header-only, to install and use simply copy the directory [`include/tao`](include/tao) to a convenient place and include the file [`include/tao/json.hpp`](include/tao/json.hpp).
* The generic JSON value class is in [`include/tao/json/value.hpp`](include/tao/json/value.hpp).
* To parse a JSON string representation to a JSON Value, use one of the functions in [`include/tao/json/from_string.hpp`](include/tao/json/from_string.hpp) or [`include/tao/json/from_file.hpp`](include/tao/json/from_file.hpp).
* To produce a JSON string representation from a JSON Value, use one of the appropriate functions in [`include/tao/json/to_string.hpp`](include/tao/json/to_string.hpp) or [`include/tao/json/stream.hpp`](include/tao/json/stream.hpp).

TODO: Hello-world-sized example.
TODO: Which other pages to reference here?
TODO: Which other concepts to reference here?

The taocpp/json library is built around two main concepts, the [*Value class*](Value-Class.md) and the [*Events interface*](Events-Interface.md).

See also the page on [common use cases](Common-Use-Cases.md) for how to do some frequently required things.

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
