// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_CONTROL_HPP
#define TAO_JSON_INTERNAL_CONTROL_HPP

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

         template< bool NEG >
         struct control< rules::number< NEG > >
            : json_pegtl::change_state< rules::number< NEG >, number_state< NEG >, errors >
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

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
