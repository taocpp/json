# Parser Interface

*Work in progress*

This interface is used when parsing directly into a C++ data type (without using an intermediate Value) *and not* using the [Binding Traits Facilities](Type-Traits.md#binding-traits-facilities) to auto-generate the [Traits](Type-Traits.md).

The public interface of a parts parser, an object that unconditionally attempts to read parts of an encoded Value from the input that it was supplied in the constructor.

```c++
struct parts_parser
{
   bool empty();

   bool null();  // Returns 'false' when next value is not a JSON Null.
   bool boolean();

   string_t string();  // Returns std::string or tao::string_view.
   binary_t binary();  // Returns std::vector< tao::byte > or tao::binary_view.
   string_t key();  // Returns std::string or tao::string_view.

   std::int64_t number_signed();
   std::uint64_t number_unsigned();
   double number_double();

   unknown_1 begin_array();
   void end_array( unknown_1& );
   void element( unknown_1& );
   bool element_or_end_array( unknown_1& );  // Returns 'false' at end of array.

   unknown_2 begin_object();
   void end_object( unknown_2& );
   void member( unknown_2& );
   bool member_or_end_object( unknown_2& );  // Returns 'false' at end of object.

   void skip_value();  // Skips one complete value in the input, whatever it is.

   unknown_3 mark();  // TODO: Explain, with reference to PEGTL markers and when/how to use them.

   void throw_parse_error( TBD ) const;  // TODO: Decide whether this is part of the public interface.
};
```

There are two patterns for how to use the Array and Object functions depending on whether the size is implicitly known by the code using the parser, or whether the code adapts to the size found in the parsed data.

In the following example we show the implicit form for Arrays, and the adaptive form for Objects, however both interfaces are available for both kinds of containers.

```c++
void implicit( parts_parser& p )
{
   auto state = p.begin_array()

   p.element( state );  // Throws when no further element available.
   do_something_with( p.read_unsigned() );  // Or read some other value.

   // ...read as many elements as desired...

   p.end_array( state );  // Throws when not at end of array.
}

void adaptive( parts_parser& p )
{
   auto state = p.begin_object();

   while( !p.member_or_end_object( p ) ) {
      const auto key = p.read_key();
      const auto value = p.read_string();  // Or read some other value.
      do_something_with( key, value );
   }
}
```

The individual values of an Array or Object can of course themselves be Arrays or Objects.

Remember to **not** write `do_something_with( p.read_key(), p.read_double() )` because the order of evaluation of function arguments in C++ is not defined.

Additional functions for the CBOR parts parser that throw when they encounter an indefinite-length string.

```c++
   tao::string_view string_view();
   tao::binary_view binary_view();
   tao::string_view key_view();
```

Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
