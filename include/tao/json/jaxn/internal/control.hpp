// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_CONTROL_HPP
#define TAO_JSON_JAXN_INTERNAL_CONTROL_HPP

#include "../../external/pegtl/contrib/changes.hpp"

#include "binary_state.hpp"
#include "bunescape_action.hpp"
#include "errors.hpp"
#include "unescape_action.hpp"

#include "../../internal/key_state.hpp"
#include "../../internal/number_state.hpp"
#include "../../internal/string_state.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            template< typename Rule >
            struct control
               : errors< Rule >
            {
            };

            template< bool NEG >
            struct control< rules::number< NEG > >
               : json_pegtl::change_state< rules::number< NEG >, tao::json::internal::number_state< NEG >, errors >
            {
            };

            template<>
            struct control< rules::string >
               : json_pegtl::change_state_and_action< rules::string, tao::json::internal::string_state, unescape_action, errors >
            {
            };

            template<>
            struct control< rules::key >
               : json_pegtl::change_state_and_action< rules::key, tao::json::internal::key_state, unescape_action, errors >
            {
            };

            template<>
            struct control< rules::binary >
               : json_pegtl::change_state_and_action< rules::binary, binary_state, bunescape_action, errors >
            {
            };

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
