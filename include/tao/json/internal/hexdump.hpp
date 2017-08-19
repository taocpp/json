// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_HEXDUMP_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_HEXDUMP_HPP

#include <sstream>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         void hexdump( std::ostream& os, const T& v )
         {
            static const char h[] = "0123456789ABCDEF";
            for( const auto b : v ) {
               os.put( h[ static_cast< unsigned char >( b ) >> 4 ] );
               os.put( h[ static_cast< unsigned char >( b ) & 0xF ] );
            }
         }

         template< typename T >
         std::string hexdump( const T& v )
         {
            std::ostringstream os;
            internal::hexdump( os, v );
            return os.str();
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
