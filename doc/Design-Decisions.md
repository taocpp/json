# Design Decisions

* [Require C++17](#require-c17)
* [Initialiser Lists](#initialiser-lists)
* [Omitted Functions](#omitted-functions)

This library is designed for correctness and simplicity, is based on C++ standard containers, and strives for speed through minimalism (rather than complicated optimisations).

## Require C++17

For the first three years this library was compatible with C++11, however we changed the required standard version to C++17 before reaching version 1.0.0.
There were two main reasons for making this change.

1. With C++11 the library had to include and maintain implementations of `std::optional` and `std::string_view`; the latter was not a full replacement since C++11 `std::string` does not know about string views.
2. Many places throughout the library that benefit from being able to use C++17 language features; for examples look at the flock of C++17-related commits made after the standards upgrade in October 2018.

## Initialiser-Lists

The Value constructor that accepts an initialiser-list as argument will always (attempt to) create a Value that is an Object, i.e. it will never attempt to create a Value that is an Array, because it is not possible for the same function to accept initialiser-lists for both Arrays and Objects without introducing ambiguities:

* Is `{ { "hallo", 1 }, { "world", 2 } }` an Array with two Arrays as elements, or an Object with two members?
* If `foo` has a [default key](Type-Traits.md#default-key-for-objects) in the traits, is `{ foo }` an Array with one element, or an Object with one member?

Creating an Array from an initialiser-list is still possible with the static `tao::json::value::array()` function.

## Omitted Functions

Class `tao::json::value` has neither a `size()` nor an `empty()` method due to the lack of unambiguous canonical semantics.

* Should atomic JSON Values have size `0` or size `1`, or should everything except for Null have size `1`?
* Should special values (like `UNINITIALIZED`) have size `0`, or should they throw when checking the size?
* Should strings use container semantics for `size()` and `empty()`, or should they behave like atomic Values?
* Should a Value Pointer have size `1`, or should it forward `size()` and `empty()` to the pointee Value?
* Should an Opaque Pointer have size `1`, or should it report the size of the JSON representation of the pointee?

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
