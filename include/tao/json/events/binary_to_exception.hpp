// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_BINARY_TO_EXCEPTION_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_BINARY_TO_EXCEPTION_HPP

#include <stdexcept>

#include "../byte_view.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct binary_to_exception
            : public Consumer
         {
            using Consumer::Consumer;

            void binary( const tao::byte_view )
            {
               throw std::runtime_error( "invalid binary data" );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
