// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_SAX_KEY_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_SAX_KEY_STATE_HH

#include <string>
#include <utility>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct sax_key_state
         {
            sax_key_state() = default;
            sax_key_state( const sax_key_state & ) = delete;
            void operator= ( const sax_key_state & ) = delete;

            template< typename Handler >
            void success( Handler & handler )
            {
               handler.commit_key( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      } // internal

   } // json

} // tao

#endif
