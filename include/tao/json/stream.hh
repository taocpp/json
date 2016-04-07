// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_STREAM_HH
#define TAOCPP_JSON_INCLUDE_STREAM_HH

#include <iosfwd>
#include <iomanip>

#include "value.hh"
#include "internal/to_stream.hh"

namespace tao
{
   namespace json
   {
      // Use ostream << std::setw( n ) for pretty-printing with indent n.

      template< template< typename ... > class Traits >
      std::ostream & operator<< ( std::ostream & o, const value_base< Traits > & v )
      {
         const auto w = o.width();

         o.width( 0 );

         if ( w > 0 ) {
            internal::to_stream( o, v, w );
         }
         else {
            internal::to_stream( o, v );
         }
         return o;
      }

   } // json

} // tao

#endif
