// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()  // NOLINT
      {
         const auto v = value::array( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
         TEST_ASSERT( v.at( gonculator( "0" ) ) == 0 );
         TEST_ASSERT( v.at( gonculator( "9" ) ) == 9 );
         TEST_ASSERT( v.at( gonculator( "-1" ) ) == 9 );
         TEST_ASSERT( v.at( gonculator( "-10" ) ) == 0 );
         TEST_ASSERT( v.at( gonculator( "-" ) ) == 9 );
         const value w = { { "a", 1 }, { "b" , 2 }, { "c", 3 } };
         TEST_ASSERT( w.at( gonculator( "a" ) ) == 1 );
         TEST_ASSERT( w.at( gonculator( "c" ) ) == 3 );
         const value x = jaxn::from_string( "{ foo : [ { bar : [ 1, 2, 3, 4 ] } ] }" );
         TEST_ASSERT( x.at( gonculator( "foo.-.bar.3" ) ) == 4 );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
