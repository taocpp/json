// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <sstream>

#include <tao/json/external/double.hpp>
#include <tao/json/external/ryu.hpp>

namespace tao
{
   namespace json
   {
      std::string dc_string( const double d )
      {
         std::ostringstream oss;
         json_double_conversion::Dtostr( oss, d );
         return oss.str();
      }

      std::string ryu_string( const double d )
      {
         std::ostringstream oss;
         ryu::d2s_stream( oss, d );
         return oss.str();
      }

      void test_double( const double v )
      {
         const auto a = dc_string( v );
         const auto b = ryu_string( v );

         if( a != b ) {
            std::cout << a << " " << b << std::endl;
         }
      }

      void unit_test()
      {
         test_double( 0.0 );
         test_double( -0.0 );
         test_double( 1.0 );
         test_double( -1.0 );
         test_double( 1.5 );
         test_double( -1.5 );
         test_double( 0.0001 );
         test_double( -0.0001 );
         test_double( 3.1416 );
         test_double( 42.0 );
         test_double( -42.0 );
         test_double( 0.1 );
         test_double( 1e10 );
         test_double( -1e10 );
         test_double( -1E+10 );
         test_double( -1E-10 );
         test_double( 1.234E+10 );
         test_double( 1.234E-10 );
         test_double( 1.79769e+308 );
         test_double( 2.22507e-308 );
         test_double( -1.79769e+308 );
         test_double( -2.22507e-308 );
         test_double( 4.9406564584124654e-324 );  // Min denormal
         test_double( 2.2250738585072009e-308 );  // Max subnormal double
         test_double( 2.2250738585072014e-308 );  // Min normal positive double
         test_double( 1.7976931348623157e+308 );  // Max double
         test_double( 18446744073709551616.0 );      // 2^64 (max of uint64_t + 1, force to use double)
         test_double( -9223372036854775809.0 );      // -2^63 - 1(min of int64_t + 1, force to use double)
         test_double( 0.9868011474609375 );            // https://github.com/miloyip/rapidjson/issues/120
         test_double( 123e34 );                                    // Fast Path Cases In Disguise
         test_double( 45913141877270640000.0 );
         test_double( 2.2250738585072011e-308 );  // http://www.exploringbinary.com/php-hangs-on-numeric-value-2-2250738585072011e-308/
         test_double( 1.7976931348623157e+308 );  // Max double in another form

         // Since
         // abs((2^-1022 - 2^-1074) - 2.2250738585072012e-308) = 3.109754131239141401123495768877590405345064751974375599... ¡Á 10^-324
         // abs((2^-1022) - 2.2250738585072012e-308) = 1.830902327173324040642192159804623318305533274168872044... ¡Á 10 ^ -324
         // So 2.2250738585072012e-308 should round to 2^-1022 = 2.2250738585072014e-308
         test_double( 2.2250738585072014e-308 );  // http://www.exploringbinary.com/java-hangs-when-converting-2-2250738585072012e-308/

         // More closer to normal/subnormal boundary
         // boundary = 2^-1022 - 2^-1075 = 2.225073858507201136057409796709131975934819546351645648... 10^-308
         test_double( 2.2250738585072009e-308 );

         // 1.0 is in (1.0 - 2^-54, 1.0 + 2^-53)
         // 1.0 - 2^-54 = 0.999999999999999944488848768742172978818416595458984375
         test_double( 0.99999999999999989 );  // Previous double
         // 1.0 + 2^-53 = 1.00000000000000011102230246251565404236316680908203125
         test_double( 1.00000000000000022 );  // Next double

         // Numbers from https://github.com/floitsch/double-conversion/blob/master/test/cctest/test-strtod.cc

         test_double( 72057594037927928.0 );
         test_double( 72057594037927936.0 );

         test_double( 9223372036854774784.0 );
         test_double( 9223372036854775808.0 );

         test_double( 10141204801825834086073718800384.0 );
         test_double( 10141204801825835211973625643008.0 );

         test_double( 5708990770823838890407843763683279797179383808.0 );
         test_double( 5708990770823839524233143877797980545530986496.0 );

         test_double( 300.0 );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
