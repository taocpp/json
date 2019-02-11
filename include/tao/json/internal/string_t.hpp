// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
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
            static constexpr const char value[] = { Cs..., 0 };

            static constexpr std::string_view as_string_view() noexcept
            {
               return std::string_view( value, sizeof...( Cs ) );
            }

            static std::string as_string()
            {
               return std::string( value, sizeof...( Cs ) );
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#define TAO_JSON_STRING_T( VaLue ) TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, VaLue )

#endif
