// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EXTERNAL_FILL_OSTREAM_HPP
#define TAO_JSON_EXTERNAL_FILL_OSTREAM_HPP

#include <cstring>
#include <ostream>

namespace tao
{
   template< class charT, class traits >
   void fill_ostream( std::basic_ostream< charT, traits >& os, std::size_t len )
   {
      char buffer[ 32 ];
      std::memset( buffer, os.fill(), sizeof( buffer ) );
      while( len ) {
         const auto chunk = ( std::min )( len, sizeof( buffer ) );
         os.write( buffer, chunk );
         len -= chunk;
      }
   }
}  // namespace tao

#endif
