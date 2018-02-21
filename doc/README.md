# TAOCPP / JSON Documentation

*The documentation is still very much work-in-progress and will be completed in time for the 1.0.0 release -- after the functionality and interfaces are finalised.*

* [Project](https://github.com/taocpp/json)
* [Getting Started](Getting-Started.md)
* [Common Use Cases](Common-Use-Cases.md)
  * [Include the right header](Common-Use-Cases.md#include-the-right-header)
  * [Read any JSON file](Common-Use-Cases.md#read-any-json-file)
  * [Parse any JSON string](Common-Use-Cases.md#parse-any-json-string)
  * [Serialise a JSON Value to a string](Common-Use-Cases.md#serialise-a-json-value-to-a-string)
  * [Serialise with base64 strings for binary data](Common-Use-Cases.md#serialise-with-base64-strings-for-binary-data)
  * [Serialise a JSON Value to an ostream](Common-Use-Cases.md#serialise-a-json-value-to-an-ostream)
  * [Create a Value with a JSON object](Common-Use-Cases.md#create-a-value-with-a-json-object)
* Installing and Using - TODO: Similar to PEGTL?
* [Value Class](Value-Class.md)
* [Type Traits](Type-Traits.md)
  * [Traits Class Template](Type-Traits.md#traits-class-template)
  * [Create JSON Value from (custom) type](Type-Traits.md#create-value-from-type)
  * [Convert JSON Value into (custom) type](Type-Traits.md#convert-value-into-type)
  * [Compare JSON Value to (custom) type](Type-Traits.md#compare-value-with-type)
  * [Produce](Type-Traits.md#produce-events-from-type) [JSON Events](Events-Interface.md) [from (custom) type.](Type-Traits.md#produce-events-from-type)
  * [Default Traits Specialisations](Type-Traits.md#default-traits-specialisations)
* [Events Interface](Events-Interface.md)
* [Advanced Use Cases](Advanced-Use-Cases.md)
  * [Custom Value Annotations](Advanced-Use-Cases.md#custom-value-annotations)
  * [Annotate with filename and line number](Advanced-Use-Cases.md#annotate-with-filename-and-line-number)
  * [Instance sharing with Raw Pointers](Advanced-Use-Cases.md#instance-sharing-with-raw-pointers)
  * [Instance sharing with Ppaque Pointers](Advanced-Use-Cases.md#instance-sharing-with-opaque-pointers)
  * [Making Values Self Contained](Advanced-Use-Cases.md#making-values-self-contained)
* [Changelog](Changelog.md)

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
