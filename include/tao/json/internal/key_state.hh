// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_KEY_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_KEY_STATE_HH

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
            key_state( const key_state & ) = delete;
            void operator= ( const key_state & ) = delete;

            template< typename Handler >
            void success( Handler & handler )
            {
               handler.key( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      } // internal

   } // json

} // tao

#endif
