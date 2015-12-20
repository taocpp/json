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

      template< typename N >
      void test_number()
      {
         value a( N( 42 ) );
         value b( N( 43 ) );
         value c( N( 43 ) );

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

         TEST_ASSERT( a == N( 42 ) );
         TEST_ASSERT( N( 42 ) == a );

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

      void test_array()
      {
         value a = from_string( "[ 1, 2 ]" );
         value b = from_string( "[ 1, 3 ]" );

         TEST_ASSERT( a.type() == type::ARRAY );
         TEST_ASSERT( b.type() == type::ARRAY );

         TEST_ASSERT( a == a );
         TEST_ASSERT( a != b );
         TEST_ASSERT( a <= a );
         TEST_ASSERT( a <= b );
         TEST_ASSERT( a >= a );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( a < b );
         TEST_ASSERT( b > a );
         TEST_ASSERT( ! ( a == b ) );
         TEST_ASSERT( ! ( a != a ) );
         TEST_ASSERT( ! ( a < a ) );
         TEST_ASSERT( ! ( b < a ) );
         TEST_ASSERT( ! ( a > a ) );
         TEST_ASSERT( ! ( a > b ) );
      }

      void unit_test()
      {
         test_null();
         test_bool();
         test_number< int64_t >();
         test_number< double >();
         test_string();
         test_array();
      }

   } // json

} // tao

#include "main.hh"
