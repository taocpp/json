// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_EVENTS_TO_PRETTY_STREAM_HPP
#define TAO_JSON_JAXN_EVENTS_TO_PRETTY_STREAM_HPP

#include "../../internal/hexdump.hpp"

#include "../../events/to_pretty_stream.hpp"

#include "../is_identifier.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace events
         {
            // Events consumer to build a JAXN string representation.

            struct to_pretty_stream
               : public json::events::to_pretty_stream
            {
               using json::events::to_pretty_stream::number;
               using json::events::to_pretty_stream::to_pretty_stream;

               void number( const double v )
               {
                  next();
                  if( !std::isfinite( v ) ) {
                     if( std::isnan( v ) ) {  // NOLINT
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
                  json::internal::hexdump( os, v );
               }
            };

         }  // namespace events

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
