// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_VALUE_HPP
#define TAO_JSON_VALUE_HPP

#include "basic_value.hpp"
#include "operators.hpp"
#include "traits.hpp"

namespace tao
{
   namespace json
   {
      using value = basic_value< traits >;

   }  // namespace json

}  // namespace tao

#define TAO_JSON_DEFAULT_KEY( x ) \
   using default_key = TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, x )

#endif
