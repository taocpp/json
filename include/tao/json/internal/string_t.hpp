// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_STRING_T_HPP
#define TAO_JSON_INTERNAL_STRING_T_HPP

#include <string>
#include <string_view>

#include "../external/pegtl/internal/pegtl_string.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< char... Cs >
         struct string_t
            : public json_pegtl::string< Cs... >
         {
            static std::string as_string()
            {
               static const char s[] = { Cs..., 0 };
               return std::string( s, sizeof...( Cs ) );
            }

            static std::string_view as_string_view() noexcept
            {
               static const char s[] = { Cs..., 0 };
               return std::string_view( s, sizeof...( Cs ) );
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
