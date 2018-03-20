# Design Decisions

* [Initialiser Lists](#initialiser-lists)
* [Omitted Functions](#omitted-functions)

This library is designed for correctness and simplicity, is based on C++ standard containers, and strives for speed through minimalism (rather than complicated optimisations).

## Initialiser-Lists

To prevent the following (and more) ambiguities, only Values for Objects can be created with an initialiser-list argument to the constructor.

* Is `{ { "hallo", 1 }, { "world", 2 } }` an Array with two Arrays as elements, or an Object with two members?
* If `foo` and `bar` both have a default key in the traits, is `{ foo, bar }` an Array with two elements, or an Object with two members?

Creating an Array from an initialiser-list is still possible with the static `tao::json::value::array()` function.

## Omitted Functions

Class `tao::json::value` has neither a `size()` nor an `empty()` method due to the lack of unambiguous canonical semantics.

* Should a JSON Null have size 0 although, unlike Uninitialized, it is a JSON Value?
* Should a string have size 1 (act as one Value) or return its size (act as container)?
* Should a binary have size 1 (act as one Value) or return its size (act as container)?
* Should a Value Pointer have size 1 or forward `size()` and `empty()` to the pointee?

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
