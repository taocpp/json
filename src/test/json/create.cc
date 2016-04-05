// Copyright (c) 2015-2016 Dr. Colin Hirsch
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
         TEST_ASSERT( ! v.is_number() );

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
         TEST_ASSERT( ! v.is_number() );

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
         TEST_ASSERT( v.is_number() );

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
         test_int64< T >( 42 );
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
         TEST_ASSERT( v.is_number() );

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
         TEST_ASSERT( ! v.is_number() );

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
         TEST_ASSERT( v.get_string() == value( std::string( s, N - 1 ) ).get_string() );
         TEST_ASSERT( v.get_string() == value( std::string( s + 0 ) ).get_string() );

         TEST_THROWS( v[ 0 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_empty_array( const value v )
      {
         TEST_ASSERT( ! v.is_null() );
         TEST_ASSERT( ! v.is_bool() );
         TEST_ASSERT( ! v.is_int64() );
         TEST_ASSERT( ! v.is_double() );
         TEST_ASSERT( ! v.is_string() );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( ! v.is_object() );
         TEST_ASSERT( ! v.is_number() );

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
         TEST_ASSERT( ! v.is_number() );

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
         const value v = value::array( 1, 2, 3, 4 );

         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( ! v.is_number() );
         TEST_ASSERT( v.type() == type::ARRAY );

         const std::vector< value > r = { value( 1 ), value( 2 ), value( 3 ), value( 4 ) };

         TEST_ASSERT( v == value( r ) );
         TEST_ASSERT( v.get_array() == r );

         TEST_ASSERT( v[ 0 ].get_int64() == 1 );
         TEST_ASSERT( v[ 1 ].get_int64() == 2 );
         TEST_ASSERT( v[ 2 ].get_int64() == 3 );
         TEST_ASSERT( v[ 3 ].get_int64() == 4 );

         TEST_ASSERT( v[ 0 ].get_int64() == 1 );
         TEST_ASSERT( v[ 1 ].get_int64() == 2 );
         TEST_ASSERT( v[ 2 ].get_int64() == 3 );
         TEST_ASSERT( v[ 3 ].get_int64() == 4 );

         TEST_THROWS( v[ 4 ] );
         TEST_THROWS( v[ "foo" ] );
      }

      void test_object_1234()
      {
        const value v { { "foo", "bar" }, { "bar", 42 }, { "baz", { { "baz", value::array( true, false, 0 ) } } } };

        TEST_ASSERT( v.is_object() );
        TEST_ASSERT( ! v.is_number() );
        TEST_ASSERT( v.type() == type::OBJECT );

        // TODO: Add more tests

        TEST_THROWS(( value { { "foo", 1 }, { "foo", 2 } } ));
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

         value v;

         const double a = std::numeric_limits< double >::infinity();
         const double b = std::numeric_limits< double >::quiet_NaN();
         const double c = std::numeric_limits< double >::signaling_NaN();

         TEST_THROWS( v = a );
         TEST_THROWS( v = b );
         TEST_THROWS( v = c );

         v = nullptr;

         TEST_ASSERT( v.type() == type::NULL_ );

         v = true;

         TEST_ASSERT( v.type() == type::BOOL_ );
         TEST_ASSERT( v.get_bool() );

         v = 1;

         TEST_ASSERT( v.type() == type::INT64 );
         TEST_ASSERT( v.get_int64() == 1 );

         v = 2.0;

         TEST_ASSERT( v.type() == type::DOUBLE );
         TEST_ASSERT( v.get_double() == 2.0 );

         v = "hallo";

         TEST_ASSERT( v.type() == type::STRING );
         TEST_ASSERT( v.get_string() == "hallo" );

         v = std::vector< value >();

         TEST_ASSERT( v.type() == type::ARRAY );
         TEST_ASSERT( v.get_array().empty() );

         v = std::map< std::string, value >();

         TEST_ASSERT( v.type() == type::OBJECT );
         TEST_ASSERT( v.get_object().empty() );

         test_string( "" );
         test_string( "foo" );
         test_string( "abcdefghijklmnpqrstuvwxyz" );

         TEST_ASSERT( value( "\0" ).get_string().empty() );  // TODO?

         TEST_ASSERT( value( "baz" ).get_string().size() == 3 );

         test_empty_array( value( empty_array ) );
         test_empty_array( value( std::vector< value >() ) );

         test_empty_object( value( empty_object ) );
         test_empty_object( value( std::map< std::string, value >() ) );

         test_array_1234();
         test_object_1234();

         TEST_THROWS( from_string( "1" ).emplace_back( 2 ) );
         TEST_THROWS( from_string( "1" ).emplace( "foo", value( 3 ) ) );
         {
            value a;
            a.emplace_back( 4 );
            TEST_ASSERT( a.type() == type::ARRAY );
            TEST_ASSERT( a[ 0 ] == 4 );
            value b( a );
            TEST_ASSERT( a.get_array() == b.get_array() );
         } {
            value a;
            a.emplace( "foo", value( 5 ) );
            TEST_ASSERT( a.type() == type::OBJECT );
            TEST_ASSERT( a[ "foo" ] == 5 );
            value b( a );
            TEST_ASSERT( a.get_object() == b.get_object() );
         } {
            const value a( "foo" );
            const value b( a );
            TEST_ASSERT( a.get_string() == b.get_string() );
         }
      }

   } // json

} // tao

#include "main.hh"
