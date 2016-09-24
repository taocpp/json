// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/value.hh>
#include <tao/json/from_string.hh>
#include <tao/json/sax/compare.hh>
#include <tao/json/sax/traverse_value.hh>

namespace tao
{
   namespace json
   {
      bool test_value( const value & v, sax::compare & c )
      {
         c.reset();
         sax::traverse_value( v, c );
         return c.match();
      }

      void test_null()
      {
         sax::compare c_null( null );

         TEST_ASSERT( test_value( null, c_null ) );
         TEST_ASSERT( ! test_value( true, c_null ) );
         TEST_ASSERT( ! test_value( false, c_null ) );
         TEST_ASSERT( ! test_value( 0, c_null ) );
         TEST_ASSERT( ! test_value( 0u, c_null ) );
         TEST_ASSERT( ! test_value( 0.0, c_null ) );
         TEST_ASSERT( ! test_value( 42, c_null ) );
         TEST_ASSERT( ! test_value( 42u, c_null ) );
         TEST_ASSERT( ! test_value( 42.0, c_null ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_null ) );
         TEST_ASSERT( ! test_value( empty_array, c_null ) );
         TEST_ASSERT( ! test_value( empty_object, c_null ) );
         TEST_ASSERT( test_value( nullptr, c_null ) );
      }

      void test_boolean()
      {
         sax::compare c_true( true );

         TEST_ASSERT( ! test_value( null, c_true ) );
         TEST_ASSERT( test_value( true, c_true ) );
         TEST_ASSERT( ! test_value( false, c_true ) );
         TEST_ASSERT( ! test_value( 0, c_true ) );
         TEST_ASSERT( ! test_value( 0u, c_true ) );
         TEST_ASSERT( ! test_value( 0.0, c_true ) );
         TEST_ASSERT( ! test_value( 42, c_true ) );
         TEST_ASSERT( ! test_value( 42u, c_true ) );
         TEST_ASSERT( ! test_value( 42.0, c_true ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_true ) );
         TEST_ASSERT( ! test_value( empty_array, c_true ) );
         TEST_ASSERT( ! test_value( empty_object, c_true ) );
         TEST_ASSERT( ! test_value( nullptr, c_true ) );
      }

      void test_number()
      {
         sax::compare c_0( 0 );

         TEST_ASSERT( ! test_value( null, c_0 ) );
         TEST_ASSERT( ! test_value( true, c_0 ) );
         TEST_ASSERT( ! test_value( false, c_0 ) );
         TEST_ASSERT( test_value( 0, c_0 ) );
         TEST_ASSERT( test_value( 0u, c_0 ) );
         TEST_ASSERT( test_value( 0.0, c_0 ) );
         TEST_ASSERT( ! test_value( 42, c_0 ) );
         TEST_ASSERT( ! test_value( 42u, c_0 ) );
         TEST_ASSERT( ! test_value( 42.0, c_0 ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_0 ) );
         TEST_ASSERT( ! test_value( empty_array, c_0 ) );
         TEST_ASSERT( ! test_value( empty_object, c_0 ) );
         TEST_ASSERT( ! test_value( nullptr, c_0 ) );

         sax::compare c_42( 42 );

         TEST_ASSERT( ! test_value( null, c_42 ) );
         TEST_ASSERT( ! test_value( true, c_42 ) );
         TEST_ASSERT( ! test_value( false, c_42 ) );
         TEST_ASSERT( ! test_value( 0, c_42 ) );
         TEST_ASSERT( ! test_value( 0u, c_42 ) );
         TEST_ASSERT( ! test_value( 0.0, c_42 ) );
         TEST_ASSERT( test_value( 42, c_42 ) );
         TEST_ASSERT( test_value( 42u, c_42 ) );
         TEST_ASSERT( test_value( 42.0, c_42 ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_42 ) );
         TEST_ASSERT( ! test_value( empty_array, c_42 ) );
         TEST_ASSERT( ! test_value( empty_object, c_42 ) );
         TEST_ASSERT( ! test_value( nullptr, c_42 ) );
      }

      void test_string()
      {
         sax::compare c_hello_world( "Hello, world!" );

         TEST_ASSERT( ! test_value( null, c_hello_world ) );
         TEST_ASSERT( ! test_value( true, c_hello_world ) );
         TEST_ASSERT( ! test_value( false, c_hello_world ) );
         TEST_ASSERT( ! test_value( 0, c_hello_world ) );
         TEST_ASSERT( ! test_value( 0u, c_hello_world ) );
         TEST_ASSERT( ! test_value( 0.0, c_hello_world ) );
         TEST_ASSERT( ! test_value( 42, c_hello_world ) );
         TEST_ASSERT( ! test_value( 42u, c_hello_world ) );
         TEST_ASSERT( ! test_value( 42.0, c_hello_world ) );
         TEST_ASSERT( test_value( "Hello, world!", c_hello_world ) );
         TEST_ASSERT( ! test_value( "foo", c_hello_world ) );
         TEST_ASSERT( ! test_value( empty_array, c_hello_world ) );
         TEST_ASSERT( ! test_value( empty_object, c_hello_world ) );
         TEST_ASSERT( ! test_value( nullptr, c_hello_world ) );
      }

      void test_array()
      {
         sax::compare c_empty_array( empty_array );

         TEST_ASSERT( ! test_value( null, c_empty_array ) );
         TEST_ASSERT( ! test_value( true, c_empty_array ) );
         TEST_ASSERT( ! test_value( false, c_empty_array ) );
         TEST_ASSERT( ! test_value( 0, c_empty_array ) );
         TEST_ASSERT( ! test_value( 0u, c_empty_array ) );
         TEST_ASSERT( ! test_value( 0.0, c_empty_array ) );
         TEST_ASSERT( ! test_value( 42, c_empty_array ) );
         TEST_ASSERT( ! test_value( 42u, c_empty_array ) );
         TEST_ASSERT( ! test_value( 42.0, c_empty_array ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_empty_array ) );
         TEST_ASSERT( ! test_value( "foo", c_empty_array ) );
         TEST_ASSERT( test_value( empty_array, c_empty_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, 3 } ), c_empty_array ) );
         TEST_ASSERT( ! test_value( empty_object, c_empty_array ) );
         TEST_ASSERT( ! test_value( nullptr, c_empty_array ) );

         sax::compare c_array( value::array( { 1, 2, 3 } ) );

         TEST_ASSERT( ! test_value( null, c_array ) );
         TEST_ASSERT( ! test_value( true, c_array ) );
         TEST_ASSERT( ! test_value( false, c_array ) );
         TEST_ASSERT( ! test_value( 0, c_array ) );
         TEST_ASSERT( ! test_value( 0u, c_array ) );
         TEST_ASSERT( ! test_value( 0.0, c_array ) );
         TEST_ASSERT( ! test_value( 42, c_array ) );
         TEST_ASSERT( ! test_value( 42u, c_array ) );
         TEST_ASSERT( ! test_value( 42.0, c_array ) );
         TEST_ASSERT( ! test_value( "Hello, world!", c_array ) );
         TEST_ASSERT( ! test_value( "foo", c_array ) );
         TEST_ASSERT( ! test_value( empty_array, c_array ) );
         TEST_ASSERT( test_value( value::array( { 1, 2, 3 } ), c_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, 4 } ), c_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, 4 } ), c_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 3, 2 } ), c_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, 3, 4 } ), c_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, value::array( { 3 } ) } ), c_array ) );
         TEST_ASSERT( ! test_value( empty_object, c_array ) );
         TEST_ASSERT( ! test_value( nullptr, c_array ) );

         sax::compare c_nested_array( value::array( { 1, 2, value::array( { 3, 4, value::array( { 5 } ) } ) } ) );

         TEST_ASSERT( ! test_value( value::array( { 1, 2, value::array( { 3 } ) } ), c_nested_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, value::array( { 3, 4 } ) } ), c_nested_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, value::array( { 3, 4, value::array( {} ) } ) } ), c_nested_array ) );
         TEST_ASSERT( test_value( value::array( { 1, 2, value::array( { 3, 4, value::array( { 5 } ) } ) } ), c_nested_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, value::array( { 3, 4, value::array( { 5 } ) } ) } ), c_nested_array ) );
         TEST_ASSERT( ! test_value( value::array( { 2, value::array( { 3, 4, value::array( { 5 } ) } ) } ), c_nested_array ) );
         TEST_ASSERT( ! test_value( value::array( { 1, 2, value::array( { 3, value::array( { 5 } ) } ) } ), c_nested_array ) );
      }

      void test_mixed()
      {
         value v = { { "foo", 0 }, { "bar", true }, { "baz", value::array( { null, true, false, 0, 1, "hallo" } ) } };

         sax::compare c( v );
         sax::traverse_value( v, c );
         TEST_ASSERT( c.match() );

         c.reset();
         v.at( "foo" ) = 42;
         sax::traverse_value( v, c );
         TEST_ASSERT( ! c.match() );

         c.reset();
         v.at( "foo" ) = 0;
         sax::traverse_value( v, c );
         TEST_ASSERT( c.match() );

         c.reset();
         v.at( "baz" ).at( 2 ) = 42;
         sax::traverse_value( v, c );
         TEST_ASSERT( ! c.match() );

         c.reset();
         v.at( "baz" ).at( 2 ) = true;
         sax::traverse_value( v, c );
         TEST_ASSERT( ! c.match() );

         c.reset();
         v.at( "baz" ).at( 2 ) = false;
         sax::traverse_value( v, c );
         TEST_ASSERT( c.match() );
      }

      void unit_test()
      {
         test_null();
         test_boolean();
         test_number();
         test_string();
         test_array();
         // TODO: test_object();
         test_mixed();
      }

   } // json

} // tao

#include "main.hh"
