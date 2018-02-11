// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_IS_IDENTIFIER_HPP
#define TAO_JSON_JAXN_IS_IDENTIFIER_HPP

#include <cctype>

#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         inline bool is_identifier( const tao::string_view v ) noexcept
         {
            if( v.empty() || std::isdigit( v[ 0 ] ) ) {  // NOLINT
               return false;
            }
            for( const auto c : v ) {
               if( !std::isalnum( c ) && c != '_' ) {  // NOLINT
                  return false;
               }
            }
            return true;
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
