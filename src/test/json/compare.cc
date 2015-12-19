// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      void test_null()
      {
         value a;
         value b( nullptr );

         TEST_ASSERT( a.type() == type::NULL_ );
         TEST_ASSERT( b.type() == type::NULL_ );

         TEST_ASSERT( a == b );
         TEST_ASSERT( ! ( a != b ) );

         TEST_ASSERT( a <= b );
         TEST_ASSERT( a >= b );

         TEST_ASSERT( ! ( a < b ) );
         TEST_ASSERT( ! ( a > b ) );

         TEST_ASSERT( a == nullptr );
         TEST_ASSERT( nullptr == b );
      }

      void test_bool()
      {
         value t( true );
         value f( false );

         TEST_ASSERT( t.type() == type::BOOL_ );
         TEST_ASSERT( f.type() == type::BOOL_ );

         TEST_ASSERT( t == t );
         TEST_ASSERT( f == f );
         TEST_ASSERT( t == value( true ) );
         TEST_ASSERT( f == value( false ) );
         TEST_ASSERT( t != value( nullptr ) );
         TEST_ASSERT( f != value( nullptr ) );
         TEST_ASSERT( t != f );
         TEST_ASSERT( f != t );
         TEST_ASSERT( ! ( t != value( true ) ) );
         TEST_ASSERT( ! ( f != value( false ) ) );
         TEST_ASSERT( ! ( f == value( true ) ) );
         TEST_ASSERT( ! ( t == value( false ) ) );

         TEST_ASSERT( t <= t );
         TEST_ASSERT( f <= f );
         TEST_ASSERT( f <= t );
         TEST_ASSERT( ! ( t <= f ) );
         TEST_ASSERT( t >= t );
         TEST_ASSERT( f >= f );
         TEST_ASSERT( t >= f );
         TEST_ASSERT( ! ( f >= t ) );

         TEST_ASSERT( ! ( t < t ) );
         TEST_ASSERT( ! ( f < f ) );
         TEST_ASSERT( ! ( t > t ) );
         TEST_ASSERT( ! ( f > f ) );
         TEST_ASSERT( ! ( f > t ) );
         TEST_ASSERT( ! ( t < f ) );
         TEST_ASSERT( t > f );
         TEST_ASSERT( f < t );

         TEST_ASSERT( t == true );
         TEST_ASSERT( false == f );
      }

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

         // TODO
      }

      void test_string()
      {
         value a( "a" );
         value b( "b" );
         value foo( "foo" );

         TEST_ASSERT( a.type() == type::STRING );
         TEST_ASSERT( foo.type() == type::STRING );

         TEST_ASSERT( a == a );
         TEST_ASSERT( foo == foo );
         TEST_ASSERT( a <= a );
         TEST_ASSERT( a <= b );
         TEST_ASSERT( a < b );
         TEST_ASSERT( a != b );
         TEST_ASSERT( b > a );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( foo >= foo );
         TEST_ASSERT( ! ( a == b ) );
         TEST_ASSERT( ! ( b != b ) );
         TEST_ASSERT( ! ( a < a ) );
         TEST_ASSERT( ! ( b > b ) );
         TEST_ASSERT( ! ( b <= a ) );
         TEST_ASSERT( ! ( a >= b ) );

         TEST_ASSERT( a == "a" );
         TEST_ASSERT( "foo" == foo );
      }

      void unit_test()
      {
         test_null();
         test_bool();
         test_int64();
         test_string();
         // TODO
      }

   } // json

} // tao

#include "main.hh"
