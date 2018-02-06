# Advanced Features

## Value Traits

The `Traits` template parameter used most prominently with `tao::json::basic_value<>` is used to control some aspects of the interaction of the JSON library with other, including user defined, data types.

1. How to compare a JSON Value with a data type.
2. How to create a JSON Value from a data type.
3. How to convert a JSON Value into a data type.
4. How to produce [Events](Events-Interface.md) from a data type.

### Comparison

### Creation

### Conversion

### Produce Events

## Opaque Pointers

TODO: Explain lifetime issues.

TODO: Explain traits requirements.

TOOD: Give short example.

## Custom Annotations

TODO: Explain base-class mechanism and its behaviour.

TODO: Show how `include/tao/contrib/position.hpp` adds the filename and position they were parsed from to every (sub-)value.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
