// Copyright (c) 2016-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_KEY_STATE_HPP
#define TAO_JSON_INTERNAL_KEY_STATE_HPP

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
            template< typename Input, typename... States >
            explicit key_state( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
            {
            }

            key_state( const key_state& ) = delete;
            key_state( key_state&& ) = delete;

            ~key_state() = default;

            void operator=( const key_state& ) = delete;
            void operator=( key_state&& ) = delete;

            template< typename Input, typename Consumer >
            void success( const Input& /*unused*/, Consumer& consumer )
            {
               consumer.key( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
