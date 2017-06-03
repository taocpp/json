// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JSON5_UNESCAPE_ACTION_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JSON5_UNESCAPE_ACTION_HPP

#include "../../external/pegtl/contrib/unescape.hpp"
#include "../../external/pegtl/nothing.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace json5
         {
            // clang-format off
            template< typename Rule > struct unescape_action : json_pegtl::nothing< Rule > {};

            template<> struct unescape_action< rules::hexcode > : json_pegtl::unescape::unescape_x {};
            template<> struct unescape_action< rules::unicode > : json_pegtl::unescape::unescape_j {};
            template<> struct unescape_action< rules::escaped_char > : json_pegtl::unescape::unescape_c< rules::escaped_char, '\b', '\f', '\n', '\r', '\t', '\v' > {};
            template<> struct unescape_action< rules::escaped_any > : json_pegtl::unescape::append_all {};

            template<> struct unescape_action< rules::unescaped< '"' > > : json_pegtl::unescape::append_all {};
            template<> struct unescape_action< rules::unescaped< '\'' > > : json_pegtl::unescape::append_all {};
            // clang-format on

         }  // namespace json5

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif