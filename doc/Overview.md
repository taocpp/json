# Overview

The main parts of the library are

* the [JSON Value class](Value-Class.md) to hold (and inspect and modify) arbitrary (extended) JSON data in memory,
* the [JSON Events interface](Events-Interface.md) as universal adapter to connect different parts of the library (and even other libraries), and
* the [JSON type traits](Type-Traits.md) to control the interaction between other/custom C++ data types and the Value class and Events interface.

![Overview](Overview.png)

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
