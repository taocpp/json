// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_HEXDUMP_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_HEXDUMP_HPP

#include <iomanip>
#include <sstream>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         std::string hexdump( const T& v )
         {
            std::ostringstream os;
            os << std::hex << std::setfill( '0' );
            for( const auto c : v ) {
               os << std::setw( 2 ) << static_cast< unsigned char >( c );
            }
            return os.str();
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
