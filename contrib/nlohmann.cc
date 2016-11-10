// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include "nlohmann/json.hpp"
#include "nlohmann/to_value.hh"
#include "nlohmann/from_value.hh"

#include <tao/json/sax/from_string.hh>
#include <tao/json/sax/to_string.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         tao::json::nlohmann::to_value handler;
         tao::json::sax::from_string( "[ null, true, false, 42, 43.0, \"foo\", [ 1, 2, 3 ], { \"a\" : \"b\", \"c\" : \"d\" } ]", handler );

         const auto & v = handler.value;

         TEST_ASSERT( v.type() == ::nlohmann::json::value_t::array );
         TEST_ASSERT( v.size() == 8 );
         TEST_ASSERT( v[ 0 ] == nullptr );
         TEST_ASSERT( v[ 1 ].get< bool >() == true );
         TEST_ASSERT( v[ 2 ].get< bool >() == false );
         TEST_ASSERT( v[ 3 ] == 42 );
         TEST_ASSERT( v[ 4 ] == 43.0 );
         TEST_ASSERT( v[ 5 ] == "foo" );
         TEST_ASSERT( v[ 6 ].type() == ::nlohmann::json::value_t::array );
         TEST_ASSERT( v[ 6 ].size() == 3 );
         TEST_ASSERT( v[ 6 ][ 0 ] == 1 );
         TEST_ASSERT( v[ 6 ][ 1 ] == 2 );
         TEST_ASSERT( v[ 6 ][ 2 ] == 3 );
         TEST_ASSERT( v[ 7 ].type() == ::nlohmann::json::value_t::object );
         TEST_ASSERT( v[ 7 ].size() == 2 );
         TEST_ASSERT( v[ 7 ].at( "a" ) == "b" );
         TEST_ASSERT( v[ 7 ].at( "c" ) == "d" );

         tao::json::sax::to_string oss_handler;
         tao::json::nlohmann::from_value( v, oss_handler );

         TEST_ASSERT( oss_handler.value() == "[null,true,false,42,43.0,\"foo\",[1,2,3],{\"a\":\"b\",\"c\":\"d\"}]" );
      }

   } // json

} // tao

#include "main.hh"
