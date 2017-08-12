// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/external/string_view.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         const std::string s = "Hello, world!";
         const string_view sv = "Hello, world!";

         TEST_ASSERT( s == "Hello, world!" );
         TEST_ASSERT( sv == "Hello, world!" );
         TEST_ASSERT( s == sv );

         TEST_ASSERT( sv.find( '!' ) == 12 );
         TEST_ASSERT( sv.find_first_not_of( "elo, word", 1 ) == 12 );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
