// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( "42"_json.get_unsigned() == 42 );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
