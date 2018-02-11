// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_UNESCAPE_ACTION_HPP
#define TAO_JSON_INTERNAL_UNESCAPE_ACTION_HPP

#include "../external/pegtl/contrib/unescape.hpp"
#include "../external/pegtl/nothing.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // clang-format off
         template< typename Rule > struct unescape_action : json_pegtl::nothing< Rule > {};

         template<> struct unescape_action< rules::escaped_unicode > : json_pegtl::unescape::unescape_j {};
         template<> struct unescape_action< rules::escaped_char > : json_pegtl::unescape::unescape_c< rules::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
         template<> struct unescape_action< rules::unescaped > : json_pegtl::unescape::append_all {};
         // clang-format on

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
