// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_VALUE_HPP

#include "basic_value.hpp"
#include "operators.hpp"
#include "traits.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct empty_base
         {
         };

      } // namespace internal

      using value = basic_value< traits, internal::empty_base >;

   }  // namespace json

}  // namespace tao

#endif
