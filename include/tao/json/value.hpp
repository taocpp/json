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

#endif
