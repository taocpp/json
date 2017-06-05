// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_BINARY_TO_HEX_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_BINARY_TO_HEX_HPP

#include <vector>

#include "../byte.hpp"
#include "../internal/hexdump.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct binary_to_hex
            : public Consumer
         {
            using Consumer::Consumer;

            void binary( const std::vector< byte >& v )
            {
               Consumer::string( internal::hexdump( v ) );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
