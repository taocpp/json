# Advanced Use Cases

* [Custom Base Class for Values](#custom-base-class-for-values)
* [Annotate with filename and line number](#annotate-with-filename-and-line-number)

## Custom Base Class for Values

By default, `tao::json::basic_value<>` uses `tao::json::internal::empty_base`, an empty dummy class, as public base class.
This can be changed to a custom public base class, allowing applications to add arbitrary custom member variables or member functions to every single JSON Value instance, including, recursively, all nested sub-values contained in Arrays and Objects.

To set up a custom public base class it is necessary to use custom [traits](Type-Traits.md) where the specialisation for `void` defines a different `public_base`.

```c++
template< typename T >
struct my_traits
   : tao::json::traits< T >
{};

struct my_base
{};

template<>
struct my_traits< void >
   : tao::json::traits< void >
{
   template< typename > using public_base = my_base;
};
```

Note that `public_base` is a template, and `tao::json::value< my_traits >` will use `my_traits< void >::public_base< tao::json::value< my_traits > >` as public base class.
This design pattern is also known as CRTP, the *curiously recurring template pattern*.

In the example `my_base` is not a template and the template parameter to `my_traits< void >::public_base<>` is ignored, however it is possible and to let `my_base` use the `tao::json::value< my_traits >` template parameter when necessary.

The library preserves the contents of the public base class, and otherwise ignores it.
More precisely:

* The base is copied when the value is copy constructed or assigned.
* The base is moved when the value is move constructed or assigned.
* Otherwise the base is default initialised when constructing a value.
* The base is ignored by the default comparison operators.

Access to the public base class instance of a value is given through two accessor functions named `public_base()`.

```c++
template< template< typename... > class Traits >
Traits< void >::public_base< basic_value< Traits > >& basic_value< Traits >::public_base() noexcept;

template< template< typename... > class Traits, class Base >
const Traits< void >::public_base< basic_value< Traits > >& basic_value< Traits, Base >::public_base() const noexcept;
```

The application can use the public base for whatever it wants, for example to add filenames and line numbers to parsed values as [shown below](#annotate-with-filename-and-line-number), or to add new public member functions to all values.

## Annotate with filename and line number

A natural use case for [custom base classes for Values](#custom-base-class-for-values) is to add the filename, line number, and starting byte position to every (sub-)value during parsing.

The two functions `tao::json::from_file_with_position()` and `tao::json::basic_from_file_with_position()` in `include/tao/json/contrib/position.hpp` do just that and can be directly used, or customised as required.

The public base class of type `tao::json::position`, and a helper to add the base class definition to any traits class template, is defined in the same header.
The following example shows how to read a filename, and, if the file contains a JSON array, prints the line numbers each array element starts on.

```c++
#include <iostream>

#include <tao/json.hpp>
#include <tao/json/contrib/position.hpp>

void example( const std::string& filename )
{
   const auto v = tao::json::from_file_with_position( filename );
   if( v.is_array() ) {
      for( const auto& i : v.get_array() ) {
         std::cout << "element on line " << i.base().line << std::endl;
      }
   }
}
```

Writing a recursive function that takes any value with positions and prints the types and line numbers of all sub-values is left as an exercise to the reader.

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
