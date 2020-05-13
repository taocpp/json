// Copyright (c) 2019-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"
#include "test_types.hpp"

#include <tao/json.hpp>

namespace tao::json
{
   void test_prefer_signed( const value& v, const std::string& j )
   {
      TEST_ASSERT( to_string< events::prefer_signed>( v ) == j );
   }

   void test_prefer_unsigned( const value& v, const std::string& j )
   {
      TEST_ASSERT( to_string< events::prefer_unsigned >( v ) == j );
   }

  
   void unit_test()
   {    
      std::int64_t val = 1;
      value v = val;

      test_prefer_signed( v, "1" );
      test_prefer_unsigned( v, "1" );
      
      val = -1;
      v = val;

      test_prefer_signed( v, "-1" );
      test_prefer_unsigned( v, "-1" );
   }

}  // namespace tao::json

#include "main.hpp"
