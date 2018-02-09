# Advanced Features

## Value Traits

The `Traits` template parameter used most prominently with `tao::json::basic_value<>` is used to control some aspects of the interaction of the JSON library with all other, including user defined, data types.

1. How to create a JSON Value from another type.
2. How to convert a JSON Value into another type.
3. How to compare a JSON Value with another type.
4. How to produce [Events](Events-Interface.md) from another type.

The usual way to add support for a custom data type, or change the behaviour of the existing traits, is to define a custom traits class that derives from the supplied traits `tao::json::traits<>` by default, and specialise it for the types as needed. Then a type alias for the JSON Value class with the user defined traits can be defined.

```c++
template< typename T >
struct my_traits
   : public tao::json::traits< T >
{
};

struct my_data
{
   // Custom...
};

template<>
struct my_traits< my_data >
{
   // See below...
};

using my_value = tao::json::basic_value< my_traits >;
```

### Creation

### Conversion

### Comparison

### Produce Events

## Opaque Pointers

TODO: Explain lifetime issues.

TODO: Explain traits requirements.

TOOD: Give short example.

## Custom Annotations

TODO: Explain base-class mechanism and its behaviour.

TODO: Show how `include/tao/contrib/position.hpp` adds the filename and position they were parsed from to every (sub-)value.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
