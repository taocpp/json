// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_STRING_STATE_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_STRING_STATE_HPP

#include <string>
#include <utility>
#include <vector>

#include "../../byte.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace jaxn
         {
            struct string_state
            {
               string_state() = default;
               string_state( const string_state& ) = delete;
               void operator=( const string_state& ) = delete;

               template< typename Consumer >
               void success( Consumer& consumer )
               {
                  if( is_binary ) {
                     const byte* begin = reinterpret_cast< const byte* >( unescaped.data() );
                     const byte* end = begin + unescaped.size();
                     consumer.binary( std::vector< byte >( begin, end ) );
                  }
                  else {
                     consumer.string( std::move( unescaped ) );
                  }
               }

               std::string unescaped;
               bool is_binary = false;
            };

         }  // namespace jaxn

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
