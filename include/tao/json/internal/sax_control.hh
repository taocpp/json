// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_SAX_CONTROL_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_SAX_CONTROL_HH

#include "../external/pegtl/contrib/changes.hh"

#include "errors.hh"

#include "unescape_action.hh"

#include "sax_number_state.hh"
#include "sax_string_state.hh"
#include "sax_key_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule >
         struct sax_control : errors< Rule > {};

         template<>
         struct sax_control< rules::number > : tao_json_pegtl::change_state< rules::number, sax_number_state, tao_json_pegtl::normal > {};

         template<>
         struct sax_control< rules::string::content > : tao_json_pegtl::change_state_and_action< rules::string::content, sax_string_state, unescape_action, tao_json_pegtl::normal > {};

         template<>
         struct sax_control< rules::key::content > : tao_json_pegtl::change_state_and_action< rules::key::content, sax_key_state, unescape_action, tao_json_pegtl::normal > {};

      } // internal

   } // json

} // tao

#endif
