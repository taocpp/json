// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP

#include "internal/proxy.hpp"

#include "traits.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      using basic_value = internal::proxy< internal::data_holder, Traits >;

      using value = basic_value< traits >;

   }  // namespace json

}  // namespace tao

#endif
