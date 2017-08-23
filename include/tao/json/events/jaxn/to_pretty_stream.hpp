// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JAXN_TO_PRETTY_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JAXN_TO_PRETTY_STREAM_HPP

#include "../../internal/hexdump.hpp"
#include "../../jaxn/is_identifier.hpp"
#include "../to_pretty_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace jaxn
         {
            // Events consumer to build a JAXN string representation.

            struct to_pretty_stream : events::to_pretty_stream
            {
               using events::to_pretty_stream::to_pretty_stream;

               using events::to_pretty_stream::number;

               void number( const double v )
               {
                  next();
                  if( !std::isfinite( v ) ) {
                     if( std::isnan( v ) ) {
                        os.write( "NaN", 3 );
                     }
                     else if( v < 0 ) {
                        os.write( "-Infinity", 9 );
                     }
                     else {
                        os.write( "Infinity", 8 );
                     }
                  }
                  else {
                     json_double_conversion::Dtostr( os, v );
                  }
               }

               void key( const tao::string_view v )
               {
                  if( json::jaxn::is_identifier( v ) ) {
                     next();
                     os.write( v.data(), v.size() );
                  }
                  else {
                     string( v );
                  }
                  os.write( ": ", 2 );
                  first = true;
                  after_key = true;
               }

               void binary( const tao::byte_view v )
               {
                  next();
                  os.put( '$' );
                  internal::hexdump( os, v );
               }
            };

         }  // namespace jaxn

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
