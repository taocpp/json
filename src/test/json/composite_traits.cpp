// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      void test_optional()
      {
         {
            const tao::optional< std::string > f;
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_null() );
            const auto g = v.as< tao::optional< std::string > >();
            TEST_ASSERT( g == f );
         }
         {
            const tao::optional< std::string > f( "hallo" );
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_string() );
            TEST_ASSERT( v == "hallo" );
            TEST_ASSERT( v.get_string() == "hallo" );
            const auto g = v.as< tao::optional< std::string > >();
            TEST_ASSERT( *g == *f );
         }
      }

      void test_shared()
      {
         {
            const std::shared_ptr< std::uint64_t > f;
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_null() );
            const auto g = v.as< std::shared_ptr< std::uint64_t > >();
            TEST_ASSERT( g == f );
         }
         {
            const auto f = std::make_shared< std::uint64_t >( 42 );
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_number() );
            const auto g = v.as< std::shared_ptr< std::uint64_t > >();
            TEST_ASSERT( g != f );
            TEST_ASSERT( *g == *f );
         }
      }

      void test_unique()
      {
         {
            const std::unique_ptr< std::uint64_t > f;
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_null() );
            const auto g = v.as< std::unique_ptr< std::uint64_t > >();
            TEST_ASSERT( g == f );
         }
         {
            const std::unique_ptr< std::uint64_t > f( new std::uint64_t( 42 ) );
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_number() );
            const auto g = v.as< std::unique_ptr< std::uint64_t > >();
            TEST_ASSERT( g != f );
            TEST_ASSERT( *g == *f );
         }
      }

      void test_list()
      {
         const std::list< std::uint64_t > f = { 1, 2, 3, 4 };
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( v.unsafe_get_array().size() == 4 );
         TEST_ASSERT( v[ 0 ] == 1 );
         TEST_ASSERT( v[ 1 ] == 2 );
         TEST_ASSERT( v[ 2 ] == 3 );
         TEST_ASSERT( v[ 3 ] == 4 );
         const auto g = v.as< std::list< std::uint64_t > >();
         TEST_ASSERT( g == f );
      }

      void test_set()
      {
         const std::set< std::uint64_t > f = { 1, 2, 3, 4 };
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( v.unsafe_get_array().size() == 4 );
         TEST_ASSERT( v[ 0 ] == 1 );
         TEST_ASSERT( v[ 1 ] == 2 );
         TEST_ASSERT( v[ 2 ] == 3 );
         TEST_ASSERT( v[ 3 ] == 4 );
         const auto g = v.as< std::set< std::uint64_t > >();
         TEST_ASSERT( g == f );
      }

      void test_vector()
      {
         const std::vector< std::uint64_t > f = { 1, 2, 3, 4 };
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v >= f );
         TEST_ASSERT( v <= f );
         TEST_ASSERT( f == v );
         TEST_ASSERT( f >= v );
         TEST_ASSERT( f <= v );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( v.unsafe_get_array().size() == 4 );
         TEST_ASSERT( v[ 0 ] == 1 );
         TEST_ASSERT( v[ 1 ] == 2 );
         TEST_ASSERT( v[ 2 ] == 3 );
         TEST_ASSERT( v[ 3 ] == 4 );
         const auto g = v.as< std::vector< std::uint64_t > >();
         TEST_ASSERT( g == f );

         const std::vector< std::uint64_t > h = { 2, 3, 4 };
         TEST_ASSERT( h != v );
         TEST_ASSERT( h > v );
         TEST_ASSERT( h >= v );
         TEST_ASSERT( !( h < v ) );
         TEST_ASSERT( !( h <= v ) );
         TEST_ASSERT( !( h == v ) );
         TEST_ASSERT( v != h );
         TEST_ASSERT( v < h );
         TEST_ASSERT( v <= h );
         TEST_ASSERT( !( v > h ) );
         TEST_ASSERT( !( v >= h ) );
         TEST_ASSERT( !( v == h ) );
      }

      void test_map()
      {
         const std::map< std::string, std::uint64_t > f = { { "a", 1 }, { "b", 2 }, { "c", 3 } };
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v.is_object() );
         TEST_ASSERT( v.unsafe_get_object().size() == 3 );
         TEST_ASSERT( v[ "a" ] == 1 );
         TEST_ASSERT( v[ "b" ] == 2 );
         TEST_ASSERT( v[ "c" ] == 3 );
         const auto g = v.as< std::map< std::string, std::uint64_t > >();
         TEST_ASSERT( g == f );

         const std::map< std::string, std::uint64_t > h = { { "a", 1 }, { "b", 3 }, { "c", 1 } };
         TEST_ASSERT( h != v );
         TEST_ASSERT( h > v );
         TEST_ASSERT( h >= v );
         TEST_ASSERT( !( h < v ) );
         TEST_ASSERT( !( h <= v ) );
         TEST_ASSERT( !( h == v ) );
         TEST_ASSERT( v != h );
         TEST_ASSERT( v < h );
         TEST_ASSERT( v <= h );
         TEST_ASSERT( !( v > h ) );
         TEST_ASSERT( !( v >= h ) );
         TEST_ASSERT( !( v == h ) );

         const std::map< std::string, std::uint64_t > i = { { "a", 1 }, { "c", 2 }, { "d", 0 } };
         TEST_ASSERT( i != v );
         TEST_ASSERT( i > v );
         TEST_ASSERT( i >= v );
         TEST_ASSERT( !( i < v ) );
         TEST_ASSERT( !( i <= v ) );
         TEST_ASSERT( !( i == v ) );
         TEST_ASSERT( v != i );
         TEST_ASSERT( v < i );
         TEST_ASSERT( v <= i );
         TEST_ASSERT( !( v > i ) );
         TEST_ASSERT( !( v >= i ) );
         TEST_ASSERT( !( v == i ) );
      }

      void unit_test()
      {
         test_optional();
         test_shared();
         test_unique();

         test_list();
         test_set();
         test_vector();
         test_map();
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
