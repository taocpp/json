// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HPP

#include "../external/pegtl/contrib/changes.hpp"

#include "errors.hpp"

#include "unescape_action.hpp"

#include "key_state.hpp"
#include "number_state.hpp"
#include "string_state.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule >
         struct control
            : errors< Rule >
         {
         };

         template<>
         struct control< rules::number >
            : json_pegtl::change_state< rules::number, number_state, errors >
         {
         };

         template<>
         struct control< rules::string::content >
            : json_pegtl::change_state_and_action< rules::string::content, string_state, unescape_action, errors >
         {
         };

         template<>
         struct control< rules::key::content >
            : json_pegtl::change_state_and_action< rules::key::content, key_state, unescape_action, errors >
         {
         };

      }  // internal

   }  // json

}  // tao

#endif
