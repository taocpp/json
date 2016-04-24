// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/literal.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( ( "[42]"_json )[ 0 ].get_unsigned_integer() == 42 );
         TEST_ASSERT( ( "[[42]]"_json )[ 0 ][ 0 ].get_unsigned_integer() == 42 );
         TEST_ASSERT( ( "[[[[42]]]]"_json )[ 0 ][ 0 ][ 0 ][ 0 ].get_unsigned_integer() == 42 );

         TEST_ASSERT( ( "[1, 2, 3]"_json )[ 0 ].get_unsigned_integer() == 1 );
         TEST_ASSERT( ( "[1, 2, 3]"_json )[ 2 ].get_unsigned_integer() == 3 );

         TEST_ASSERT( ( "[1, [2, [3, [[[4]], 5, 6]]]]"_json )[ 1 ][ 1 ][ 1 ][ 1 ].get_unsigned_integer() == 5 );
      }

   } // json

} // tao

#include "main.hh"
