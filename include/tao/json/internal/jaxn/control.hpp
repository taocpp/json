// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_CONTROL_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_CONTROL_HPP

#include "../../external/pegtl/contrib/changes.hpp"

#include "errors.hpp"
#include "string_state.hpp"
#include "unescape_action.hpp"

#include "../key_state.hpp"
#include "../number_state.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace jaxn
         {
            template< typename Rule >
            struct control
               : errors< Rule >
            {
            };

            template< bool NEG >
            struct control< rules::number< NEG > >
               : json_pegtl::change_state< rules::number< NEG >, number_state< NEG >, errors >
            {
            };

            template<>
            struct control< rules::string >
               : json_pegtl::change_state_and_action< rules::string, string_state, unescape_action, errors >
            {
            };

            template<>
            struct control< rules::key >
               : json_pegtl::change_state_and_action< rules::key, key_state, unescape_action, errors >
            {
            };

         }  // namespace jaxn

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
