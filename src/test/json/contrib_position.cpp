// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/contrib/position.hpp>

namespace tao
{
   namespace json
   {
      void test_json()
      {
         const std::string f = "tests/taocpp/position.json";

         auto v = parse_file_with_position( f );

         TEST_ASSERT( v.base().source == f );
         TEST_ASSERT( v.base().line == 1 );
         TEST_ASSERT( v.base().byte_in_line == 0 );

         TEST_ASSERT( v.at( 0 ).base().source == f );
         TEST_ASSERT( v.at( 0 ).base().line == 2 );
         TEST_ASSERT( v.at( 0 ).base().byte_in_line == 8 );

         TEST_ASSERT( v.at( 1 ).base().source == f );
         TEST_ASSERT( v.at( 1 ).base().line == 3 );
         TEST_ASSERT( v.at( 1 ).base().byte_in_line == 8 );

         TEST_ASSERT( v.at( 2 ).base().source == f );
         TEST_ASSERT( v.at( 2 ).base().line == 4 );
         TEST_ASSERT( v.at( 2 ).base().byte_in_line == 8 );

         TEST_ASSERT( v.at( 3 ).base().source == f );
         TEST_ASSERT( v.at( 3 ).base().line == 5 );
         TEST_ASSERT( v.at( 3 ).base().byte_in_line == 8 );

         TEST_ASSERT( v.at( 3 ).at( "hello" ).base().source == f );
         TEST_ASSERT( v.at( 3 ).at( "hello" ).base().line == 6 );
         TEST_ASSERT( v.at( 3 ).at( "hello" ).base().byte_in_line == 26 );
      }

      void unit_test()
      {
         test_json();
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
