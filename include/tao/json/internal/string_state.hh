// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_STRING_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_STRING_STATE_HH

#include <string>
#include <utility>

#include "result_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct string_state
         {
            string_state() = default;
            string_state( const string_state & ) = delete;
            void operator= ( const string_state & ) = delete;

            template< template< typename ... > class Traits >
            void success( result_state< Traits > & result )
            {
               result.result.unsafe_assign( std::move( unescaped ) );
            }

            std::string unescaped;
         };

      } // internal

   } // json

} // tao

#endif
