// Copyright (c) 2019-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"
#include "test_types.hpp"

#include <tao/json/events/non_finite_to_exception.hpp>
#include <tao/json/events/non_finite_to_null.hpp>
#include <tao/json/events/non_finite_to_string.hpp>

#include <tao/json.hpp>

namespace tao::json
{
   value test_non_finite_to_string( const value& v, const std::string& s )
   {
      return to_string< events::non_finite_to_string >( v ) == s;
   }

   value test_non_finite_to_exception( const value& v, const std::string& s )
   {
      return to_string< events::non_finite_to_exception >( v ) == s;
   }

   value test_non_finite_to_null( const value& v, const std::string& s )
   {
      return to_string< events::non_finite_to_null >( v ) == s;
   }
   void unit_test()
   {
      value v = NAN;
      TEST_ASSERT( test_non_finite_to_string( v, "NaN" ) );

      v = INFINITY;
      TEST_ASSERT( test_non_finite_to_string( v, "Infinity" ) );

      v = -1 * INFINITY;
      TEST_ASSERT( test_non_finite_to_string( v, "-Infinity" ) );

      v = 1.0;
      TEST_ASSERT( test_non_finite_to_string( v, "1.0" ) );

      //----------------------------------------------------

      v = 1.0;
      TEST_ASSERT( test_non_finite_to_exception( v, "1.0" ) );

      //----------------------------------------------------

      v = NAN;
      TEST_ASSERT( test_non_finite_to_null( v, "NULL" ) );

      v = 1.0;
      TEST_ASSERT( test_non_finite_to_null( v, "1.0" ) );
   }

}  // namespace tao::json
#include "main.hpp"