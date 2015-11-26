// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      void test_int64()
      {
         value a( 42 );
         value b( 43 );
         value c( 43 );

         TEST_ASSERT( a < b );
         TEST_ASSERT( c > a );
         TEST_ASSERT( a == a );
         TEST_ASSERT( b == c );
         TEST_ASSERT( a <= b );
         TEST_ASSERT( a <= a );
         TEST_ASSERT( a >= a );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( a != b );

         TEST_ASSERT( ! ( b < a ) );
         TEST_ASSERT( ! ( a > b ) );
         TEST_ASSERT( ! ( a != a ) );
         TEST_ASSERT( ! ( b != c ) );
         TEST_ASSERT( ! ( a == b ) );
         TEST_ASSERT( ! ( b <= a ) );
         TEST_ASSERT( ! ( a >= b ) );

         TEST_ASSERT( a == 42 );
         TEST_ASSERT( 42 == a );
      }

      void unit_test()
      {
         test_int64();
         // TODO...
      }

   } // json

} // tao

#include "main.hh"
