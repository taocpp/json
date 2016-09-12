// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_NUMBER_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_NUMBER_STATE_HH

#include <cstdint>
#include <cstdlib>
#include <inttypes.h>

#include "../external/double.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         static const std::size_t max_mantissa_digits = 772;

         struct number_state
         {
            number_state()
            {
               mantissa[ 0 ] = '-';
            }

            number_state( const number_state & ) = delete;
            void operator= ( const number_state & ) = delete;

            int32_t exponent10 = 0;
            uint16_t msize = 0;  // Excluding sign.
            bool isfp = false;
            bool mneg = false;
            bool eneg = false;
            bool drop = false;
            char mantissa[ 1 + max_mantissa_digits + 1 ];

            template< typename Handler >
            void success( Handler & handler )
            {
               if ( ! isfp ) {
                  errno = 0;
                  char * p;
                  mantissa[ msize + 1 ] = 0;
                  if ( mneg ) {
                    const std::int64_t ll = std::strtoll( mantissa, & p, 10 );
                    if ( ( errno != ERANGE ) && ( p == mantissa + msize + 1 ) ) {
                      handler.number( ll );
                      return;
                    }
                  }
                  else {
                    const std::uint64_t ull = std::strtoull( mantissa + 1, & p, 10 );
                    if ( ( errno != ERANGE ) && ( p == mantissa + msize + 1 ) ) {
                      handler.number( ull );
                      return;
                    }
                  }
               }
               if ( drop ) {
                  mantissa[ 1 + msize++ ] = '1';
                  --exponent10;
               }
               const auto d = json_double_conversion::Strtod( json_double_conversion::Vector< const char >( mantissa + 1, msize ), exponent10 );
               handler.number( mneg ? -d : d );
            }
         };

      } // internal

   } // json

} // tao

#endif
