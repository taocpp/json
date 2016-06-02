// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hh"

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         value v = { { "foo", 1 } };
         value v2 = { { "bar", &v }, { "baz", value::array( { 2, &v, 3 } ) } };
         value v3 = { { "bar", v }, { "baz", value::array( { 2, v, 3 } ) } };
         value v4 = { { "bar", { { "foo", 1 } } }, { "baz", value::array( { 2, { { "foo", 1 } }, 3 } ) } };

         TEST_ASSERT( v2 == v3 );
         TEST_ASSERT( v2 == v4 );
         TEST_ASSERT( v2.at( "bar" ).type() == type::POINTER );
         TEST_ASSERT( v2.at( "baz" ).at( 1 ).type() == type::POINTER );
         TEST_ASSERT( v3.at( "bar" ).type() == type::OBJECT );
         TEST_ASSERT( v3.at( "baz" ).at( 1 ).type() == type::OBJECT );

         make_self_contained( v2 );

         TEST_ASSERT( v2 == v3 );
         TEST_ASSERT( v2.at( "bar" ).type() == type::OBJECT );
         TEST_ASSERT( v2.at( "baz" ).at( 1 ).type() == type::OBJECT );
      }

   } // json

} // tao

#include "main.hh"
