# Advanced Use Cases

* [Custom Base Class for Values](#custom-base-class-for-values)
* [Annotate with filename and line number](#annotate-with-filename-and-line-number)

## Custom Base Class for Values

The class template `tao::json::basic_value<>` has two template parameters, the type traits, and a `Base` class that it (privately) inherits from.

This allows applications to add arbitrary custom data to every single JSON Value instance, including, recursively, in all nested sub-values contained in Arrays and Objects.

The library preserves the contents of the base class, and otherwise ignores it.
More precisely:

* The Base is copied when the value is copy constructed or assigned.
* The Base is moved when the value is move constructed or assigned.
* Otherwise the Base is default initialised when constructing a value.
* The Base is ignored by the default comparison operators.

Access to the `Base` instance of a value is given through two accessor functions named `base()`.

```c++
template< template< typename... > class Traits, class Base >
Base& basic_value< Traits, Base >::base() noexcept;

template< template< typename... > class Traits, class Base >
const Base& basic_value< Traits, Base >::base() const noexcept;
```

The application can use the Base for whatever it wants, for example to add filenames and line numbers to parsed values as [shown below](#annotate-with-filename-and-line-number).

## Annotate with filename and line number

A natural use case for [custom Base classes for Values](#custom-base-class-for-values) is to add the filename, line number, and starting byte position to every (sub-)value during parsing.

The two functions `tao::json::parse_file_with_position()` and `tao::json::basic_parse_file_with_position()` in `include/tao/json/contrib/position.hpp` do just that and can be directly used, or customised as required.

The (private) base class of type `tao::json::position` is defined in the same header.
The following example shows how to read a filename, and, if the file contains a JSON array, prints the line numbers each array element starts on.

```c++
#include <iostream>

#include <tao/json.hpp>
#include <tao/json/contrib/position.hpp>

void example( const std::string& filename )
{
   const auto v = tao::json::parse_file_with_position( filename );
   if( v.is_array() ) {
      for( const auto& i : v.unsafe_get_array() ) {
         std::cout << "element on line " << i.base().line << std::endl;
      }
   }
}
```

Writing a recursive function that takes any value with positions and prints the types and line numbers of all sub-values is left as an exercise to the reader.

Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
