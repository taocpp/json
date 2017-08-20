// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JAXN_IS_IDENTIFIER_HPP
#define TAOCPP_JSON_INCLUDE_JAXN_IS_IDENTIFIER_HPP

#include <cctype>
#include <string>

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         inline bool is_identifier( const tao::string_view v ) noexcept
         {
            if( v.empty() || std::isdigit( v[ 0 ] ) ) {
               return false;
            }
            for( const auto c : v ) {
               if( !std::isalnum( c ) && c != '_' ) {
                  return false;
               }
            }
            return true;
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
