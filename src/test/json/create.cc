// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hh"

namespace tao
{
   namespace json
   {
      void test_null()
      {
         const value v;

         TEST_ASSERT( v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::NULL_ );

         const value u( nullptr );

         TEST_ASSERT( u.type() == type::NULL_ );
         TEST_ASSERT( u == v );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_bool( const bool b )
      {
         const value v( b );

         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::BOOL_ );
         TEST_ASSERT( v.get_bool() == b );
         TEST_ASSERT( v.unsafe_bool() == b );

         TEST_ASSERT( v == v );
         TEST_ASSERT( value( b ) == v );
         TEST_ASSERT( value( ! b ) != v );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      template< typename T >
      void test_int64( const T t )
      {
         const value v( t );

         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::INT64 );
         TEST_ASSERT( v.get_int64() == t );
         TEST_ASSERT( v.unsafe_int64() == t );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      template< typename T >
      void test_int64()
      {
         test_int64< T >( 0 );
         test_int64< T >( std::numeric_limits< T >::min() );
         test_int64< T >( std::numeric_limits< T >::max() );
      }

      void test_double( const double d )
      {
         const value v( d );

         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::DOUBLE );
         TEST_ASSERT( v.get_double() == d );
         TEST_ASSERT( v.unsafe_double() == d );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      template< unsigned N >
      void test_string( const char ( & s )[ N ] )
      {
         const value v( s );

         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::STRING );
         TEST_ASSERT( v.get_string() == s );
         TEST_ASSERT( v.unsafe_string() == s );

         const std::string t = s;

         TEST_ASSERT( v.get_string() == value( t ).get_string() );
         TEST_ASSERT( v.get_string() == value( std::string( s ) ).get_string() );
         TEST_ASSERT( v.get_string() == value( s, N - 1 ).get_string() );
         TEST_ASSERT( v.get_string() == value( s + 0 ).get_string() );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_empty_array( const value v)
      {
         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( ! v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::ARRAY );
         TEST_ASSERT( v.get_array().empty() );
         TEST_ASSERT( v.unsafe_array().empty() );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_empty_object( const value v )
      {
         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( ! v.is_array() );
         TEST_ASSERT( v.is_object() );

         TEST_THROWS( v.get_bool() );
         TEST_THROWS( v.get_int64() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );

         TEST_ASSERT( v.type() == type::OBJECT );
         TEST_ASSERT( v.get_object().empty() );
         TEST_ASSERT( v.unsafe_object().empty() );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_array_1234()
      {
         const value v{ 1, 2, 3, 4 };

         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( v.type() == type::ARRAY );

         const std::vector< value > r = { value( 1 ), value( 2 ), value( 3 ), value( 4 ) };

         TEST_ASSERT( v == value( r ) );
         TEST_ASSERT( v.get_array() == r );

         TEST_ASSERT( v[ 0 ].get_int64() == 1 );
         TEST_ASSERT( v[ 1 ].get_int64() == 2 );
         TEST_ASSERT( v[ 2 ].get_int64() == 3 );
         TEST_ASSERT( v[ 3 ].get_int64() == 4 );

         TEST_ASSERT( v( 0 ).get_int64() == 1 );
         TEST_ASSERT( v( 1 ).get_int64() == 2 );
         TEST_ASSERT( v( 2 ).get_int64() == 3 );
         TEST_ASSERT( v( 3 ).get_int64() == 4 );

         TEST_THROWS( v[ 4 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void unit_test()
      {
         test_null();

         test_bool( true );
         test_bool( false );

         test_int64< signed char >();
         test_int64< unsigned char >();
         test_int64< signed short >();
         test_int64< unsigned short >();
         test_int64< signed int >();
         test_int64< unsigned int >();
         test_int64< signed long >();
         test_int64< signed long long >();

         test_double( 0.0 );
         test_double( 42.0 );

         test_string( "" );
         test_string( "foo" );
         test_string( "abcdefghijklmnpqrstuvwxyz" );

         TEST_ASSERT( value( "\0" ).get_string().empty() );  // TODO?

         TEST_ASSERT( value( "baz" ).get_string().size() == 3 );
         TEST_ASSERT( value( "baz", 2 ).get_string().size() == 2 );
         TEST_ASSERT( value( "baz", 4 ).get_string().size() == 4 );

         test_empty_array( value( empty_array ) );
         test_empty_array( value( std::vector< value >() ) );

         test_empty_object( value( empty_object ) );
         test_empty_object( value( std::map< std::string, value >() ) );

         test_array_1234();

         TEST_ASSERT( value( { "foo", "bar", "baz" } ).is_array() );
      }

   } // json

} // tao

#include "main.hh"
