// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hpp"
#include "test_events.hpp"

#include <tao/json.hpp>

#include <tao/json/contrib/traits.hpp>

namespace tao
{
   namespace json
   {
      void test_shared()
      {
         using namespace test;  // NOLINT
         {
            const auto f = std::make_shared< std::uint64_t >( 42 );
            TEST_ASSERT( !traits< std::shared_ptr< uint64_t > >::is_nothing< traits >( f ) );
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_number() );
            const auto g = v.as< std::shared_ptr< std::uint64_t > >();
            TEST_ASSERT( g != f );
            TEST_ASSERT( *g == *f );
            std::shared_ptr< std::uint64_t > h;
            v.to( h );
            TEST_ASSERT( h != f );
            TEST_ASSERT( g != h );
            TEST_ASSERT( *g == *h );
            consumer c = { uint64( 42 ) };
            events::produce( c, f );
         }
      }

      void test_unique()
      {
         const std::unique_ptr< std::uint64_t > f( new std::uint64_t( 42 ) );
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v.is_number() );
         const auto g = v.as< std::unique_ptr< std::uint64_t > >();
         TEST_ASSERT( g != f );
         TEST_ASSERT( *g == *f );
      }

      void test_deque()
      {
         const std::deque< std::uint64_t > f = { 1, 2, 3, 4 };
         value v = f;
         TEST_ASSERT( v == f );
         TEST_ASSERT( v.is_array() );
         TEST_ASSERT( v.unsafe_get_array().size() == 4 );
         TEST_ASSERT( v[ 0 ] == 1 );
         TEST_ASSERT( v[ 1 ] == 2 );
         TEST_ASSERT( v[ 2 ] == 3 );
         TEST_ASSERT( v[ 3 ] == 4 );
         const auto g = v.as< std::deque< std::uint64_t > >();
         TEST_ASSERT( g == f );
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

      void test_pointer()
      {
         const pointer p( "/foo/1/bar/-" );
         value v = p;
         TEST_ASSERT( v == value::array( { "foo", 1, "bar", "-" } ) );
         events::to_value t;
         events::produce( t, p );
         TEST_ASSERT( t.value == v );
      }

      void unit_test()
      {
         test_shared();
         test_unique();

         test_deque();
         test_list();
         test_set();
         test_vector();
         test_map();

         test_pointer();
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
