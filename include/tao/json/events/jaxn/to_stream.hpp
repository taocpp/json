// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JAXN_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JAXN_TO_STREAM_HPP

#include "../../jaxn/is_identifier.hpp"
#include "../to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace jaxn
         {
            // Events consumer to build a JAXN string representation.

            struct to_stream : events::to_stream
            {
               using events::to_stream::to_stream;

               using events::to_stream::number;

               void number( const double v )
               {
                  next();
                  if( !std::isfinite( v ) ) {
                     if( std::isnan( v ) ) {
                        os << "NaN";
                     }
                     else if( v < 0 ) {
                        os << "-Infinity";
                     }
                     else {
                        os << "Infinity";
                     }
                  }
                  else {
                     json_double_conversion::Dtostr( os, v );
                  }
               }

               void key( const std::string& v )
               {
                  if( json::jaxn::is_identifier( v ) ) {
                     next();
                     os << v;
                  }
                  else {
                     string( v );
                  }
                  os.put( ':' );
                  first = true;
               }

               // TODO: Binary strings!
            };

         }  // namespace jaxn

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
