// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_NON_FINITE_TO_STRING_HPP
#define TAO_JSON_EVENTS_NON_FINITE_TO_STRING_HPP

#include <cmath>

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct non_finite_to_string
            : public Consumer
         {
            using Consumer::Consumer;

            using Consumer::number;

            void number( const double v )
            {
               if( !std::isfinite( v ) ) {
                  if( std::isnan( v ) ) {  // NOLINT
                     Consumer::string( "NaN" );
                  }
                  else if( v > 0 ) {
                     Consumer::string( "Infinity" );
                  }
                  else {
                     Consumer::string( "-Infinity" );
                  }
               }
               else {
                  Consumer::number( v );
               }
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
