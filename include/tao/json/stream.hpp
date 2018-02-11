// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_STREAM_HPP
#define TAO_JSON_STREAM_HPP

#include <iomanip>
#include <iosfwd>
#include <stdexcept>

#include "to_stream.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      // Use ostream << std::setw( n ) for pretty-printing with indent n.

      template< template< typename... > class Traits, typename Base >
      std::ostream& operator<<( std::ostream& o, const basic_value< Traits, Base >& v )
      {
         const auto w = o.width( 0 );
         if( w > 0 ) {
            if( w >= 256 ) {
               throw std::runtime_error( "indentation too large" );  // NOLINT
            }
            json::to_stream( o, v, static_cast< std::size_t >( w ) );
         }
         else {
            json::to_stream( o, v );
         }
         return o;
      }

   }  // namespace json

}  // namespace tao

#endif
