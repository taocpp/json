// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_BASE64URL_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_BASE64URL_HPP

#include <iomanip>
#include <sstream>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         inline std::string base64url( const T& v )
         {
            static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

            std::string s;
            s.reserve( ( v.size() + 2 ) / 3 * 4 );

            unsigned cycle = 0;
            unsigned encode = 0;
            for( const auto c : v ) {
               encode <<= 8;
               encode += static_cast< unsigned char >( c );
               s += table[ ( encode >> ( ++cycle * 2 ) ) & 0x3f ];
               if( cycle == 3 ) {
                  cycle = 0;
                  s += table[ encode & 0x3f ];
               }
            }

            switch( cycle ) {
               case 0:
                  break;

               case 1:
                  s += table[ ( encode << 4 ) & 0x3f ];
                  break;

               case 2:
                  s += table[ ( encode << 2 ) & 0x3f ];
                  break;

               default:
                  assert( !"code should be unreachable" );  // LCOV_EXCL_LINE
            }

            return s;
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
