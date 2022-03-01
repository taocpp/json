// Copyright (c) 2016-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../src/test/json/test.hpp"

#include "json.hpp"

#include <tao/json/binary_view.hpp>
#include <tao/json/contrib/from_nlohmann.hpp>
#include <tao/json/contrib/to_nlohmann.hpp>
#include <tao/json/events/from_string.hpp>
#include <tao/json/events/to_string.hpp>
#include <tao/json/from_string.hpp>

namespace tao::json
{
   void unit_test()
   {
      tao::json::events::to_nlohmann< nlohmann::json > value_consumer;

      auto value = tao::json::from_string( "[ null, true, false, 42, 43.0, \"foo\", [ 1, 2, 3 ], { \"a\" : \"b\", \"c\" : \"d\" } ]" );
      tao::json::events::from_value( value_consumer, value );
      // tao::json::events::from_string( value_consumer, "[ null, true, false, 42, 43.0, \"foo\", [ 1, 2, 3 ], { \"a\" : \"b\", \"c\" : \"d\" } ]" );

      const auto& v = value_consumer.value;

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

      tao::json::events::to_string output_consumer;
      tao::json::events::from_nlohmann( output_consumer, v );

      TEST_ASSERT( output_consumer.value() == "[null,true,false,42,43.0,\"foo\",[1,2,3],{\"a\":\"b\",\"c\":\"d\"}]" );
   }

}  // namespace tao::json

#include "../src/test/json/main.hpp"
