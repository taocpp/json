# Overview

The main parts of the JSON library are

* the [Value Class](Value-Class.md) to handle (extended) JSON data in memory,
* the [Events Interface](Events-Interface.md) as universal adapter between different parts of the library (and other libraries), and
* the [Type Traits](Type-Traits.md) to control the interaction between C++ data types and the Value Class and Events Interface.

![Overview](Overview.png)

* The [Value Class](Value-Class.md) can be customised with [Type Traits](Type-Traits.md) and [custom annotations](Advanced-Use-Cases.md#custom-value-annotations).
* The [Events Interface](Events-Interface.md) is used to convert the [Value Class](Value-Class.md) from and to external representations.
* One or more [Events Filters](Events-Interface.md#included-filters) can be used to modify, filter, ... Events and to send them to multiple Consumers.
* The [Type Traits](Type-Traits.md) are used to convert the [Value Class](Value-Class.md) from and to arbitrary C++ data types.
* They also generate Events, and thus external representations, directly from C++ data types, and
* are used to directly parse external representations into C++ data types.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
