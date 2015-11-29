// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_CONTROL_HH

#include "../external/pegtl/contrib/changes.hh"

#include "errors.hh"
#include "grammar.hh"

#include "value_action.hh"
#include "array_action.hh"
#include "object_action.hh"

#include "array_state.hh"
#include "object_state.hh"
#include "number_state.hh"
#include "string_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule > struct control : errors< Rule > {};

         template<> struct control< rules::value > : tao_json_pegtl::change_action< rules::value, value_action, tao_json_pegtl::normal > {};
         template<> struct control< rules::number > : tao_json_pegtl::change_state< rules::number, number_state, tao_json_pegtl::normal > {};
         template<> struct control< rules::string::content > : tao_json_pegtl::change_state< rules::string::content, string_state, tao_json_pegtl::normal > {};
         template<> struct control< rules::array::content > : tao_json_pegtl::change_state_and_action< rules::array::content, array_state, array_action, tao_json_pegtl::normal > {};
         template<> struct control< rules::object::content > : tao_json_pegtl::change_state_and_action< rules::object::content, object_state, object_action, tao_json_pegtl::normal > {};

      } // internal

   } // json

} // tao

#endif
