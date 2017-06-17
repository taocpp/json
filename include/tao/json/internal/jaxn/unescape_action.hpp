// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_UNESCAPE_ACTION_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_UNESCAPE_ACTION_HPP

#include "../../external/pegtl/contrib/unescape.hpp"
#include "../../external/pegtl/nothing.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace jaxn
         {
            // clang-format off
            template< typename Rule > struct unescape_action : json_pegtl::nothing< Rule > {};

            template<> struct unescape_action< rules::escaped_char > : json_pegtl::unescape::unescape_c< rules::escaped_char, '"', '\'', '\\', '/', '\b', '\f', '\n', '\r', '\t', '\v', '\0' > {};
            template<> struct unescape_action< rules::escaped_unicode > : json_pegtl::unescape::unescape_j {};
            template<> struct unescape_action< rules::escaped_unicode_code_point_content > : json_pegtl::unescape::unescape_u {};

            template< char D > struct unescape_action< rules::unescaped< D > > : json_pegtl::unescape::append_all {};
            // clang-format on

         }  // namespace jaxn

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
