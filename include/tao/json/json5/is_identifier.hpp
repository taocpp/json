// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON5_IS_IDENTIFIER_HPP
#define TAOCPP_JSON_INCLUDE_JSON5_IS_IDENTIFIER_HPP

#include <string>

namespace tao
{
   namespace json
   {
      namespace json5
      {
         inline bool is_identifier( const std::string& v ) noexcept
         {
            // TODO: JSON5 allows more characters, but this needs unicode support...
            return !v.empty() && v.find_first_not_of( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_$" ) == std::string::npos && !std::isdigit( v[ 0 ] );
         }

      }  // namespace json5

   }  // namespace json

}  // namespace tao

#endif
