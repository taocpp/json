// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      void unit_test_1()
      {
         {
            parts_parser p( "", __FUNCTION__ );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( "1  ", __FUNCTION__ );
            TEST_ASSERT( p.number_signed() == 1 );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( "1  ", __FUNCTION__ );
            TEST_ASSERT( p.number_unsigned() == 1 );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( "1  ", __FUNCTION__ );
            TEST_ASSERT( p.number_double() == 1.0 );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( "1.0  ", __FUNCTION__ );
            TEST_ASSERT( p.number_double() == 1.0 );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( "1.234e5  ", __FUNCTION__ );
            TEST_ASSERT( p.number_double() == 1.234e5 );
            TEST_ASSERT( p.empty() );
         }
         {
            parts_parser p( " [ true, -42.7, false ] ", __FUNCTION__ );
            auto a = p.begin_array();
            p.element( a );
            TEST_ASSERT( p.boolean() == true );
            p.element( a );
            TEST_ASSERT( p.number_double() == -42.7 );
            p.element( a );
            TEST_ASSERT( p.boolean() == false );
            p.end_array( a );
            TEST_ASSERT( p.empty() );
         }
      }

      void unit_test()
      {
         unit_test_1();
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
