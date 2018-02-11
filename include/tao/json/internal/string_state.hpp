// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_STRING_STATE_HPP
#define TAO_JSON_INTERNAL_STRING_STATE_HPP

#include <string>
#include <utility>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct string_state
         {
            string_state() = default;

            string_state( const string_state& ) = delete;
            string_state( string_state&& ) = delete;

            ~string_state() = default;

            void operator=( const string_state& ) = delete;
            void operator=( string_state&& ) = delete;

            template< typename Consumer >
            void success( Consumer& consumer )
            {
               consumer.string( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
