// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_BINARY_TO_BASE64_HPP
#define TAO_JSON_EVENTS_BINARY_TO_BASE64_HPP

#include "../byte_view.hpp"
#include "../internal/base64.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct binary_to_base64
            : public Consumer
         {
            using Consumer::Consumer;

            void binary( const tao::byte_view v )
            {
               Consumer::string( internal::base64( v ) );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
