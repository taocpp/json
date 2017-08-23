// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_STREAM_HPP

#include <iomanip>
#include <iosfwd>

#include "data.hpp"
#include "to_stream.hpp"

namespace tao
{
   namespace json
   {
      // Use ostream << std::setw( n ) for pretty-printing with indent n.

      inline std::ostream& operator<<( std::ostream& o, const data& v )
      {
         const auto w = o.width();

         o.width( 0 );

         if( w > 0 ) {
            json::to_stream( o, v, w );
         }
         else {
            json::to_stream( o, v );
         }
         return o;
      }

   }  // namespace json

}  // namespace tao

#endif
