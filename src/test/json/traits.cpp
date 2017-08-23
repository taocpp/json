// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      struct my_data
      {
      };

      template<>
      struct traits< my_data >
      {
         static const char* default_key;

         static void assign( custom_value& v, const my_data& )
         {
            v = {
               { "fuh", "bar" },
               { "bar", 42 },
               { "baz", true }
            };
         }
      };

      const char* traits< my_data >::default_key = "my_data";

      void unit_test()
      {
         const custom_value v{ { "foo", my_data() }, my_data() };
         std::cout << to_string( v ) << std::endl;

         std::string s2 = "bar";
         const custom_value v2{ { s2, my_data() }, my_data() };
         std::cout << to_string( v2 ) << std::endl;

         const std::string s3 = "baz";
         const custom_value v3{ { s3, my_data() }, my_data() };
         std::cout << to_string( v3 ) << std::endl;
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
