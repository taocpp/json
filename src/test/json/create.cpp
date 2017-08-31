// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      void test_uninitialized()
      {
         const custom_value v{};

         TEST_ASSERT( !v );
         TEST_ASSERT( v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::UNINITIALIZED );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );

         custom_value u = v;
         TEST_ASSERT( u.type() == type::UNINITIALIZED );
         TEST_ASSERT( u == v );
         TEST_ASSERT( u <= v );
         TEST_ASSERT( u >= v );
         TEST_ASSERT( !( u != v ) );
         TEST_ASSERT( !( u < v ) );
         TEST_ASSERT( !( u > v ) );

         const custom_value w = std::move( u );
         TEST_ASSERT( w.type() == type::UNINITIALIZED );
         TEST_ASSERT( w == v );
         TEST_ASSERT( w <= v );
         TEST_ASSERT( w >= v );
         TEST_ASSERT( !( w != v ) );
         TEST_ASSERT( !( w < v ) );
         TEST_ASSERT( !( w > v ) );

#ifndef NDEBUG
         TEST_ASSERT( u.type() == type::DISCARDED );
         TEST_THROWS( u = u );
#endif

         u.reset();
         TEST_ASSERT( u.type() == type::UNINITIALIZED );
         TEST_ASSERT( u == v );

#ifndef NDEBUG
         {
            char memory[ sizeof( custom_value ) ];
            custom_value* ptr = static_cast< custom_value* >( static_cast< void* >( memory ) );
            new( ptr ) custom_value( "dummy" );
            ptr->~custom_value();
            TEST_ASSERT( ptr->type() == type::DESTROYED );
         }
#endif
      }

      void test_null()
      {
         const custom_value v = null;
         const custom_value v2( v );

         TEST_ASSERT( v );
         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );

         TEST_ASSERT( v.type() == type::NULL_ );

         const custom_value u( null );

         TEST_ASSERT( u.type() == type::NULL_ );
         TEST_ASSERT( u == v );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      void test_bool( const bool b )
      {
         const custom_value v( b );
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::BOOLEAN );
         TEST_ASSERT( v.get_boolean() == b );
         TEST_ASSERT( v.unsafe_get_boolean() == b );

         TEST_ASSERT( v == v );
         TEST_ASSERT( custom_value( b ) == v );
         TEST_ASSERT( custom_value( !b ) != v );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      template< typename T >
      void test_signed( const T t )
      {
         const custom_value v( t );
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( v.is_integer() );
         TEST_ASSERT( v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::SIGNED );
         TEST_ASSERT( v.get_signed() == t );
         TEST_ASSERT( v.unsafe_get_signed() == t );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      template< typename T >
      void test_signed()
      {
         test_signed< T >( 0 );
         test_signed< T >( 1 );
         test_signed< T >( -1 );
         test_signed< T >( 42 );
         test_signed< T >( -42 );
         test_signed< T >( std::numeric_limits< T >::min() );
         test_signed< T >( std::numeric_limits< T >::max() );
      }

      template< typename T >
      void test_unsigned( const T t )
      {
         const custom_value v( t );
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( v.is_integer() );
         TEST_ASSERT( v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::UNSIGNED );
         TEST_ASSERT( v.get_unsigned() == t );
         TEST_ASSERT( v.unsafe_get_unsigned() == t );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      template< typename T >
      void test_unsigned()
      {
         test_unsigned< T >( 0 );
         test_unsigned< T >( 1 );
         test_unsigned< T >( 42 );
         test_unsigned< T >( std::numeric_limits< T >::min() );
         test_unsigned< T >( std::numeric_limits< T >::max() );
      }

      void test_double( const double d )
      {
         const custom_value v( d );
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::DOUBLE );
         TEST_ASSERT( v.get_double() == d );
         TEST_ASSERT( v.unsafe_get_double() == d );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      template< unsigned N >
      void test_string( const char ( &s )[ N ] )
      {
         const custom_value v( s );
         const custom_value v2( v );

         TEST_ASSERT( v.empty() == ( N == 1 ) );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::STRING );
         TEST_ASSERT( v.get_string() == s );
         TEST_ASSERT( v.unsafe_get_string() == s );

         const std::string t = s;

         TEST_ASSERT( v.get_string() == custom_value( t ).get_string() );
         TEST_ASSERT( v.get_string() == custom_value( std::string( s ) ).get_string() );
         TEST_ASSERT( v.get_string() == custom_value( std::string( s, N - 1 ) ).get_string() );
         TEST_ASSERT( v.get_string() == custom_value( std::string( s + 0 ) ).get_string() );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      void test_empty_array( const custom_value v )
      {
         const custom_value v2( v );

         TEST_ASSERT( v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( !v.is_object() );
         TEST_ASSERT( !v.is_raw_ptr() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_object() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::ARRAY );
         TEST_ASSERT( v.get_array().empty() );
         TEST_ASSERT( v.unsafe_get_array().empty() );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      void test_empty_object( const custom_value v )
      {
         const custom_value v2( v );

         TEST_ASSERT( v.empty() );

         TEST_ASSERT( !v.is_null() );
         TEST_ASSERT( !v.is_boolean() );
         TEST_ASSERT( !v.is_signed() );
         TEST_ASSERT( !v.is_unsigned() );
         TEST_ASSERT( !v.is_double() );
         TEST_ASSERT( !v.is_string() );
         TEST_ASSERT( !v.is_array() );
         TEST_ASSERT( v.is_object() );
         TEST_ASSERT( !v.is_integer() );
         TEST_ASSERT( !v.is_number() );

         TEST_THROWS( v.get_boolean() );
         TEST_THROWS( v.get_signed() );
         TEST_THROWS( v.get_unsigned() );
         TEST_THROWS( v.get_double() );
         TEST_THROWS( v.get_string() );
         TEST_THROWS( v.get_array() );
         TEST_THROWS( v.get_raw_ptr() );

         TEST_ASSERT( v.type() == type::OBJECT );
         TEST_ASSERT( v.get_object().empty() );
         TEST_ASSERT( v.unsafe_get_object().empty() );

         TEST_THROWS( v.at( 0 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      void test_array_1234()
      {
         const custom_value v = custom_value::array( { 1, 2, 3, 4 } );
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( !v.is_number() );
         TEST_ASSERT( v.type() == type::ARRAY );

         const std::vector< data > r = { custom_value( 1 ), custom_value( 2 ), custom_value( 3 ), custom_value( 4 ) };

         TEST_ASSERT( v == custom_value( r ) );
         TEST_ASSERT( v.get_array() == r );

         TEST_ASSERT( v.at( 0 ).get_signed() == 1 );
         TEST_ASSERT( v.at( 1 ).get_signed() == 2 );
         TEST_ASSERT( v.at( 2 ).get_signed() == 3 );
         TEST_ASSERT( v.at( 3 ).get_signed() == 4 );

         TEST_ASSERT( v.at( 0 ).get_signed() == 1 );
         TEST_ASSERT( v.at( 1 ).get_signed() == 2 );
         TEST_ASSERT( v.at( 2 ).get_signed() == 3 );
         TEST_ASSERT( v.at( 3 ).get_signed() == 4 );

         TEST_THROWS( v.at( 4 ) );
         TEST_THROWS( v.at( "foo" ) );
      }

      void test_object_1234()
      {
         const custom_value v{ { "foo", "bar" }, { "bar", 42 }, { "baz", { { "baz", custom_value::array( { true, false, 0 } ) } } } };
         const custom_value v2( v );

         TEST_ASSERT( !v.empty() );

         TEST_ASSERT( v.is_object() );
         TEST_ASSERT( !v.is_number() );
         TEST_ASSERT( v.type() == type::OBJECT );

         // TODO: Add more tests

         TEST_THROWS( custom_value{ { "foo", 1 }, { "foo", 2 } } );
      }

      void unit_test()
      {
         test_uninitialized();

         test_null();

         test_bool( true );
         test_bool( false );

         test_signed< signed char >();
         test_signed< signed short >();
         test_signed< signed int >();
         test_signed< signed long >();
         test_signed< signed long long >();

         test_unsigned< unsigned char >();
         test_unsigned< unsigned short >();
         test_unsigned< unsigned int >();
         test_unsigned< unsigned long >();
         test_unsigned< unsigned long long >();

         test_double( 0.0 );
         test_double( 42.0 );

         custom_value v;
         {
            const double a = std::numeric_limits< double >::infinity();
            const double b = std::numeric_limits< double >::quiet_NaN();
            const double c = std::numeric_limits< double >::signaling_NaN();

            v = null;

            TEST_ASSERT( v.type() == type::NULL_ );

            v = true;

            TEST_ASSERT( v.type() == type::BOOLEAN );
            TEST_ASSERT( v.get_boolean() );

            v = 1;

            TEST_ASSERT( v.type() == type::SIGNED );
            TEST_ASSERT( v.get_signed() == 1 );

            v = 1u;

            TEST_ASSERT( v.type() == type::UNSIGNED );
            TEST_ASSERT( v.get_unsigned() == 1u );

            v = 2.0;

            TEST_ASSERT( v.type() == type::DOUBLE );
            TEST_ASSERT( v.get_double() == 2.0 );

            v = "hallo";

            TEST_ASSERT( v.type() == type::STRING );
            TEST_ASSERT( v.get_string() == "hallo" );

            v = a;

            TEST_ASSERT( v.type() == type::DOUBLE );
            TEST_ASSERT( v.get_double() == a );

            v = std::vector< data >();

            TEST_ASSERT( v.type() == type::ARRAY );
            TEST_ASSERT( v.get_array().empty() );

            v = b;

            TEST_ASSERT( v.type() == type::DOUBLE );
            TEST_ASSERT( std::isnan( v.get_double() ) );

            v = std::map< std::string, data >();

            TEST_ASSERT( v.type() == type::OBJECT );
            TEST_ASSERT( v.get_object().empty() );

            v = c;

            TEST_ASSERT( v.type() == type::DOUBLE );
            TEST_ASSERT( std::isnan( v.get_double() ) );

            v = nullptr;

            TEST_ASSERT( v.type() == type::RAW_PTR );
            TEST_ASSERT( v.get_raw_ptr() == nullptr );
            TEST_ASSERT( v.empty() );
            TEST_ASSERT( v == null );
         }

         {
            custom_value v2( v );
            TEST_ASSERT( v2.type() == type::RAW_PTR );
            TEST_ASSERT( v2.get_raw_ptr() == nullptr );
            TEST_ASSERT( v2.empty() );
            TEST_ASSERT( v2 == null );
         }

         {
            custom_value v2( &v );
            TEST_ASSERT( v2.type() == type::RAW_PTR );
            TEST_ASSERT( v2.get_raw_ptr() != nullptr );
            TEST_ASSERT( v2.get_raw_ptr()->type() == type::RAW_PTR );
            TEST_ASSERT( v2.get_raw_ptr()->get_raw_ptr() == nullptr );
            TEST_ASSERT( !v2.empty() );
            TEST_ASSERT( v2.get_raw_ptr()->empty() );
            TEST_ASSERT( v2 == null );
         }

         test_string( "" );
         test_string( "foo" );
         test_string( "abcdefghijklmnpqrstuvwxyz" );

         TEST_ASSERT( custom_value( "\0" ).get_string().empty() );  // TODO?

         TEST_ASSERT( custom_value( "baz" ).get_string().size() == 3 );

         test_empty_array( custom_value( empty_array ) );
         test_empty_array( custom_value( std::vector< data >() ) );

         test_empty_object( custom_value( empty_object ) );
         test_empty_object( custom_value( std::map< std::string, data >() ) );

         test_array_1234();
         test_object_1234();

         TEST_THROWS( custom_from_string( "1" ).emplace_back( 2 ) );
         TEST_THROWS( custom_from_string( "1" ).emplace( "foo", 3 ) );
         {
            custom_value a;
            a.emplace_back( 4 );
            TEST_ASSERT( a.type() == type::ARRAY );
            TEST_ASSERT( a.at( 0 ) == 4 );
            custom_value b( a );
            TEST_ASSERT( a.get_array() == b.get_array() );
         }
         {
            custom_value a;
            a.emplace( "foo", 5 );
            TEST_ASSERT( a.type() == type::OBJECT );
            TEST_ASSERT( a.at( "foo" ) == 5 );
            custom_value b( a );
            TEST_ASSERT( a == b );
            TEST_ASSERT( a.get_object() == b.get_object() );
         }
         {
            const custom_value a( "foo" );
            const custom_value b( a );
            TEST_ASSERT( a.get_string() == b.get_string() );
         }
         {
            const custom_value a( 4 );
            const custom_value b( 4.0 );
            TEST_ASSERT( a == b );
            TEST_ASSERT( a == 4 );
            TEST_ASSERT( a == 4.0 );
            TEST_ASSERT( b == 4 );
            TEST_ASSERT( b == 4.0 );
         }
         {
            custom_value v2 = { { "foo", { { "bar", { { "baz", 42 } } } } } };
            TEST_ASSERT( v2.at( "foo" ).at( "bar" ).at( "baz" ).is_signed() );
            TEST_ASSERT( v2.at( "foo" ).at( "bar" ).at( "baz" ).unsafe_get_signed() == 42 );
            v2 = v2.at( "foo" ).at( "bar" );
            TEST_ASSERT( v2.at( "baz" ).is_signed() );
            TEST_ASSERT( v2.at( "baz" ).unsafe_get_signed() == 42 );
         }
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
