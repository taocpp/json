// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>
#include <tao/json/jaxn/parts_parser.hpp>

namespace tao::json
{
   void unit_test_1()
   {
      {
         jaxn::parts_parser p( "", __FUNCTION__ );
         TEST_ASSERT( p.empty() );
      }
      {
         jaxn::parts_parser p( " [ true, false, null ] ", __FUNCTION__ );
         auto a = p.begin_array();
         p.element( a );
         TEST_ASSERT( p.boolean() == true );
         p.element( a );
         TEST_ASSERT( p.boolean() == false );
         p.element( a );
         TEST_ASSERT( p.null() == true );
         p.end_array( a );
         TEST_ASSERT( p.empty() );
      }
      {
         jaxn::parts_parser p( "42", __FUNCTION__ );
         TEST_ASSERT( p.number_unsigned() == 42 );
      }
      {
         jaxn::parts_parser p( "42", __FUNCTION__ );
         TEST_ASSERT( p.number_signed() == 42 );
      }
      {
         jaxn::parts_parser p( "-42", __FUNCTION__ );
         TEST_ASSERT( p.number_signed() == -42 );
      }
      {
         jaxn::parts_parser p( "0x1234", __FUNCTION__ );
         TEST_ASSERT( p.number_unsigned() == 0x1234 );
      }
      {
         jaxn::parts_parser p( "0x1234", __FUNCTION__ );
         TEST_ASSERT( p.number_signed() == 0x1234 );
      }
      {
         jaxn::parts_parser p( "-0x1234", __FUNCTION__ );
         TEST_ASSERT( p.number_signed() == -0x1234 );
      }
   }

   void unit_test()
   {
      unit_test_1();
   }

}  // namespace tao::json

#include "main.hpp"
