// Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/contrib/position.hpp>

namespace tao::json
{
   void test_json()
   {
      const std::string f = "tests/taocpp/position.json";

      auto v = from_file_with_position( f );

      TEST_ASSERT( v.source() == f );
      TEST_ASSERT( v.line() == 1 );
      TEST_ASSERT( v.column() == 1 );

      TEST_ASSERT( v.at( 0 ).source() == f );
      TEST_ASSERT( v.at( 0 ).line() == 2 );
      TEST_ASSERT( v.at( 0 ).column() == 9 );

      TEST_ASSERT( v.at( 1 ).source() == f );
      TEST_ASSERT( v.at( 1 ).line() == 3 );
      TEST_ASSERT( v.at( 1 ).column() == 9 );

      TEST_ASSERT( v.at( 2 ).source() == f );
      TEST_ASSERT( v.at( 2 ).line() == 4 );
      TEST_ASSERT( v.at( 2 ).column() == 9 );

      TEST_ASSERT( v.at( 3 ).source() == f );
      TEST_ASSERT( v.at( 3 ).line() == 5 );
      TEST_ASSERT( v.at( 3 ).column() == 9 );

      TEST_ASSERT( v.at( 3 ).at( "hello" ).source() == f );
      TEST_ASSERT( v.at( 3 ).at( "hello" ).line() == 6 );
      TEST_ASSERT( v.at( 3 ).at( "hello" ).column() == 27 );
   }

   void unit_test()
   {
      test_json();
   }

}  // namespace tao::json

#include "main.hpp"
