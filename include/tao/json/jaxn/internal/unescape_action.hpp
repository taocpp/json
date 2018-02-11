// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_UNESCAPE_ACTION_HPP
#define TAO_JSON_JAXN_INTERNAL_UNESCAPE_ACTION_HPP

#include "../../external/pegtl/contrib/unescape.hpp"
#include "../../external/pegtl/nothing.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            // clang-format off
            template< typename Rule > struct unescape_action : json_pegtl::nothing< Rule > {};

            template<> struct unescape_action< rules::escaped_char > : json_pegtl::unescape::unescape_c< rules::escaped_char, '"', '\'', '\\', '/', '\b', '\f', '\n', '\r', '\t', '\v', '\0' > {};
            template<> struct unescape_action< rules::escaped_unicode > : json_pegtl::unescape::unescape_j {};
            template<> struct unescape_action< rules::escaped_unicode_code_point_content > : json_pegtl::unescape::unescape_u {};

            template< char D > struct unescape_action< rules::unescaped< D > > : json_pegtl::unescape::append_all {};
            // clang-format on

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
