// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON5_TO_PRETTY_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON5_TO_PRETTY_STREAM_HPP

#include "../../json5/is_identifier.hpp"
#include "../to_pretty_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace json5
         {
            // Events consumer to build a JSON5 string representation.

            struct to_pretty_stream : events::to_pretty_stream
            {
               using events::to_pretty_stream::to_pretty_stream;

               using events::to_pretty_stream::number;

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
                  if( json::json5::is_identifier( v ) ) {
                     next();
                     os << v;
                  }
                  else {
                     string( v );
                  }
                  os.write( ": ", 2 );
                  first = true;
                  after_key = true;
               }
            };

         }  // namespace json5

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
