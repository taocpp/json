// Copyright (c) 2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <sstream>

#include <tao/json.hh>

namespace tao
{
   namespace json
   {
      void test_both( const value & v, const std::string & s )
      {
         TEST_ASSERT( to_string( v ) == s );
         std::ostringstream oss;
         oss << v;
         TEST_ASSERT( oss.str() == s );
      }

      void unit_test()
      {
         test_both( value( nullptr ), "null" );
         test_both( value( true ), "true" );
         test_both( value( false ), "false" );
         test_both( value( 42 ), "42" );
         test_both( value( 42.1 ), "42.1" );
         test_both( value( "foo" ), "\"foo\"" );
         test_both( value::array( 1, 2, 3 ), "[1,2,3]" );
         test_both( { { "foo", 42 } }, "{\"foo\":42}" );
      }

   } // json

} // tao

#include "main.hh"
