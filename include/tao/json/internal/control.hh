// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HH

#include "../external/pegtl/contrib/changes.hh"

#include "errors.hh"

#include "unescape_action.hh"

#include "number_state.hh"
#include "string_state.hh"
#include "key_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule >
         struct control : errors< Rule > {};

         template<>
         struct control< rules::number > : tao_json_pegtl::change_state< rules::number, number_state, tao_json_pegtl::normal > {};

         template<>
         struct control< rules::string::content > : tao_json_pegtl::change_state_and_action< rules::string::content, string_state, unescape_action, tao_json_pegtl::normal > {};

         template<>
         struct control< rules::key::content > : tao_json_pegtl::change_state_and_action< rules::key::content, key_state, unescape_action, tao_json_pegtl::normal > {};

      } // internal

   } // json

} // tao

#endif
