// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/literal.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( ( "42"_json ).get_int64() == 42 );
      }

   } // json

} // tao

#include "main.hh"
