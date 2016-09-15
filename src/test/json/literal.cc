// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( "42"_json.get_unsigned() == 42 );
      }

   } // json

} // tao

#include "main.hh"
