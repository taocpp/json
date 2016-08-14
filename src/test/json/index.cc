// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/from_string.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( "[42]"_json.at( 0 ).get_unsigned() == 42 );
         TEST_ASSERT( "[[42]]"_json.at( 0 ).at( 0 ).get_unsigned() == 42 );
         TEST_ASSERT( "[[[[42]]]]"_json.at( 0 ).at( 0 ).at( 0 ).at( 0 ).get_unsigned() == 42 );

         TEST_ASSERT( "[1, 2, 3]"_json.at( 0 ).get_unsigned() == 1 );
         TEST_ASSERT( "[1, 2, 3]"_json.at( 2 ).get_unsigned() == 3 );

         TEST_ASSERT( "[1, [2, [3, [[[4]], 5, 6]]]]"_json.at( 1 ).at( 1 ).at( 1 ).at( 1 ).get_unsigned() == 5 );
      }

   } // json

} // tao

#include "main.hh"
