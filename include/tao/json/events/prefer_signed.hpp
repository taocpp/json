// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_PREFER_SIGNED_HPP
#define TAO_JSON_EVENTS_PREFER_SIGNED_HPP

#include <cstdint>

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct prefer_signed
            : public Consumer
         {
            using Consumer::Consumer;

            using Consumer::number;

            void number( const std::uint64_t v )
            {
               if( v <= 9223372036854775807ull ) {
                  Consumer::number( std::int64_t( v ) );
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
