// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

// prevent false positives from GCC 7
#if defined( __GNUC__ ) && ( __GNUC__ == 7 )
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#include <tao/json/self_contained.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         value e;

         value d = { { "foo", 1 } };
         value v = std::move( d );

         value v1 = { { "bar", v }, { "baz", value::array( { 2, v, 3 } ) } };
         value v2 = { { "bar", &v }, { "baz", value::array( { 2, &v, 3 } ) } };
         value v4 = { { "bar", { { "foo", 1 } } }, { "baz", value::array( { 2, { { "foo", 1 } }, 3 } ) } };

         TEST_ASSERT( v1 == v2 );
         TEST_ASSERT( v1 == v4 );
         TEST_ASSERT( v2 == v4 );

         TEST_ASSERT( v1.at( "bar" ).type() == type::OBJECT );
         TEST_ASSERT( v1.at( "baz" ).at( 1 ).type() == type::OBJECT );

         TEST_ASSERT( v2.at( "bar" ).type() == type::RAW_PTR );
         TEST_ASSERT( v2.at( "baz" ).at( 1 ).type() == type::RAW_PTR );

         TEST_ASSERT( is_self_contained( e ) );

         TEST_ASSERT( is_self_contained( v1 ) );
         TEST_ASSERT( !is_self_contained( v2 ) );
         TEST_ASSERT( is_self_contained( v4 ) );

         make_self_contained( e );
#ifndef NDEBUG
         TEST_THROWS( make_self_contained( d ) );
#endif
         make_self_contained( v1 );
         make_self_contained( v2 );

         TEST_ASSERT( is_self_contained( v1 ) );
         TEST_ASSERT( is_self_contained( v2 ) );
         TEST_ASSERT( v1 == v2 );
         TEST_ASSERT( v1 == v4 );
         TEST_ASSERT( v2 == v4 );

         TEST_ASSERT( v1.at( "bar" ).type() == type::OBJECT );
         TEST_ASSERT( v1.at( "baz" ).at( 1 ).type() == type::OBJECT );

         TEST_ASSERT( v2.at( "bar" ).type() == type::OBJECT );
         TEST_ASSERT( v2.at( "baz" ).at( 1 ).type() == type::OBJECT );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
