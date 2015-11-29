// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ESCAPE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ESCAPE_HH

#include <string>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline std::string escape( const std::string & data )
         {
            std::string r = "\"";
            r.reserve( data.size() + 7 );
            static const char * h = "0123456789abcdef";
            const unsigned char * d = reinterpret_cast< const unsigned char * >( data.data() );

            for ( size_t i = 0; i < data.size(); ++i ) {
               switch ( const auto c = d[ i ] ) {
                  case '\b':
                     r += "\\b";
                     break;
                  case '\f':
                     r += "\\f";
                     break;
                  case '\n':
                     r += "\\n";
                     break;
                  case '\r':
                     r += "\\r";
                     break;
                  case '\t':
                     r += "\\t";
                     break;
                  case '\\':
                     r += "\\\\";
                     break;
                  case '\"':
                     r += "\\\"";
                     break;
                  default:
                     if ( ( c < 32 ) || ( c == 127 ) ) {
                        r += "\\u00";
                        r += h[ ( c & 0xf0 ) >> 4 ];
                        r += h[   c & 0x0f        ];
                        continue;
                     }
                     r += c;  // Assume valid UTF-8.
                     break;
               }
            }
            r += '"';
            return r;
         }

      } // internal

   } // json

} // tao

#endif
