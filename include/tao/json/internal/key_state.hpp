// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_KEY_STATE_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_KEY_STATE_HPP

#include <string>
#include <utility>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct key_state
         {
            key_state() = default;
            key_state( const key_state& ) = delete;
            void operator=( const key_state& ) = delete;

            template< typename Consumer >
            void success( Consumer& consumer )
            {
               consumer.key( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
