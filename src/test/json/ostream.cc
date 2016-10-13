// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/value.hh>
#include <tao/json/to_string.hh>
#include <tao/json/stream.hh>

#include <sstream>

namespace tao
{
   namespace json
   {
      void test_simple( const value & v, const std::string & s )
      {
         TEST_ASSERT( to_string( v ) == s );
         std::ostringstream oss;
         oss << v;
         TEST_ASSERT( oss.str() == s );
      }

      void test_pretty( const value & v, const std::string & s )
      {
         std::ostringstream oss;
         oss << std::setw( 2 ) << v;
         TEST_ASSERT( oss.str() == s );
      }

      void unit_test()
      {
         value e;
         value d = 42;

         const value v = std::move( d );

         TEST_THROWS( to_string( e ) );
         TEST_THROWS( to_string( d ) );

         test_simple( null, "null" );
         test_simple( true, "true" );
         test_simple( false, "false" );
         test_simple( 42, "42" );
         test_simple( 42u, "42" );
         test_simple( 42.1, "42.1" );
         test_simple( "foo", "\"foo\"" );
         test_simple( empty_array, "[]" );
         test_simple( value::array( {} ), "[]" );
         test_simple( value::array( { 1 } ), "[1]" );
         test_simple( value::array( { 1, 2, 3 } ), "[1,2,3]" );
         test_simple( empty_object, "{}" );
         test_simple( { { "foo", 42 } }, "{\"foo\":42}" );
         test_simple( { { "foo", 42u } }, "{\"foo\":42}" );
         test_simple( { { "foo", 42 }, { "bar", 43 } }, "{\"bar\":43,\"foo\":42}" );
         test_simple( { { "foo", 42 }, { "bar", 43u } }, "{\"bar\":43,\"foo\":42}" );
         test_simple( { { "foo", v }, { "bar", 43u } }, "{\"bar\":43,\"foo\":42}" );
         test_simple( { { "foo", &v }, { "bar", 43u } }, "{\"bar\":43,\"foo\":42}" );
         test_simple( { { "foo", nullptr } }, "{\"foo\":null}" );
         test_simple( { { "foo", value::array( { 1, { { "bar", 42 }, { "baz", value::array( { empty_object, 43 } ) } }, empty_array } ) } }, "{\"foo\":[1,{\"bar\":42,\"baz\":[{},43]},[]]}" );

         test_pretty( null, "null" );
         test_pretty( true, "true" );
         test_pretty( false, "false" );
         test_pretty( 42, "42" );
         test_pretty( 42.1, "42.1" );
         test_pretty( "foo", "\"foo\"" );
         test_pretty( empty_array, "[]" );
         test_pretty( value::array( {} ), "[]" );
         test_pretty( value::array( { 1 } ), "[\n  1\n]" );
         test_pretty( value::array( { 1, 2u, 3 } ), "[\n  1,\n  2,\n  3\n]" );
         test_pretty( empty_object, "{}" );
         test_pretty( { { "foo", 42 } }, "{\n  \"foo\": 42\n}" );
         test_pretty( { { "foo", 42 }, { "bar", 43u } }, "{\n  \"bar\": 43,\n  \"foo\": 42\n}" );
         test_pretty( { { "foo", v }, { "bar", 43u } }, "{\n  \"bar\": 43,\n  \"foo\": 42\n}" );
         test_pretty( { { "foo", &v }, { "bar", 43u } }, "{\n  \"bar\": 43,\n  \"foo\": 42\n}" );
         test_pretty( { { "foo", nullptr } }, "{\n  \"foo\": null\n}" );
         test_pretty( { { "foo", value::array( { 1, { { "bar", 42 }, { "baz", value::array( { empty_object, 43 } ) } }, empty_array } ) } }, "{\n  \"foo\": [\n    1,\n    {\n      \"bar\": 42,\n      \"baz\": [\n        {},\n        43\n      ]\n    },\n    []\n  ]\n}" );
      }

   } // json

} // tao

#include "main.hh"
