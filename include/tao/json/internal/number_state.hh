// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_NUMBER_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_NUMBER_STATE_HH

#include <cstdlib>
#include <inttypes.h>

#include "../external/double.hh"

#include "result_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         static const size_t max_mantissa_digits = 772;

         struct number_state
         {
            number_state()
            {
               mantissa[ 0 ] = '-';
            }

            int32_t exponent10 = 0;
            uint16_t msize = 0;  // Excluding sign.
            bool isfp = false;
            bool mneg = false;
            bool eneg = false;
            bool drop = false;
            char mantissa[ 1 + max_mantissa_digits + 1 ];

            template< template< typename ... > class Traits >
            void success( result_state< Traits > & result )
            {
               if ( ! isfp ) {
                  errno = 0;
                  char * p;
                  mantissa[ msize + 1 ] = 0;
                  const int64_t ll = std::strtoll( mantissa + 1 - mneg, & p, 10 );
                  if ( ( errno != ERANGE ) && ( p == mantissa + msize + 1 ) ) {
                     result.result.unsafe_assign( ll );
                     return;
                  }
               }
               if ( drop ) {
                  mantissa[ 1 + msize++ ] = '1';
                  --exponent10;
               }
               const auto d = json_double_conversion::Strtod( json_double_conversion::Vector< const char >( mantissa + 1, msize ), exponent10 );
               result.result.unsafe_assign( mneg ? -d : d );
            }
         };

      } // internal

   } // json

} // tao

#endif
