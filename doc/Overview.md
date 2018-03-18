# Overview

The main parts of the JSON library are

* the [Value Class](Value-Class.md) to handle (extended) JSON data in memory,
* the [Events Interface](Events-Interface.md) as universal adapter between different parts of the library (and other libraries), and
* the [Type Traits](Type-Traits.md) to control the interaction between C++ data types and the Value Class and Events Interface.

![Overview](Overview.png)

* The Value Class can be customised with Type Traits and custom annotations.
* The Events Interface is used for conversion of external representation from and to the Value Class.
* One or more Events Filters can be used to modify, filter, verify, ... or duplicate Events.
* The Type Traits control conversion of C++ data types from and to the Value Class.
* They also control how to generate Events, and thus external representations, from C++ data types, and
* how to directly parse external representations into C++ data types.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
