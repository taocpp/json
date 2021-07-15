// Copyright (c) 2016-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_ESCAPE_HPP
#define TAO_JSON_INTERNAL_ESCAPE_HPP

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace tao::json::internal
{
   inline void escape( std::ostream& os, std::string_view s )
   {
      static const char* h = "0123456789abcdef";

      constexpr std::string_view esc = { "\\\"\b\f\n\r\t\x00\x01\x02\x03\x04\x05\x06\x07\x0B\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F", 35 };

      std::string_view::size_type n;
      while( ( n = s.find_first_of( esc ) ) != std::string_view::npos ) {
         os.write( s.data(), n );
         os.put( '\\' );
         switch( const auto c = s[ n ] ) {
            case 0:  // TODO: Is this allowed?
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 11:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 127:
               os << "u00" << h[ ( c & 0xf0 ) >> 4 ] << h[ c & 0x0f ];
               break;
            case '\b':
               os.put( 'b' );
               break;
            case '\f':
               os.put( 'f' );
               break;
            case '\n':
               os.put( 'n' );
               break;
            case '\r':
               os.put( 'r' );
               break;
            case '\t':
               os.put( 't' );
               break;
            case '\\':
            case '"':
               os.put( c );
               break;
            default:
               throw std::logic_error( "code should be unreachable" );  // LCOV_EXCL_LINE
         }
         s.remove_prefix( n + 1 );
      }
      os.write( s.data(), s.size() );
   }

   [[nodiscard]] inline std::string escape( const std::string_view s )
   {
      std::ostringstream o;
      escape( o, s );
      return o.str();
   }

}  // namespace tao::json::internal

#endif
