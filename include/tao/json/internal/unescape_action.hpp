// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_UNESCAPE_ACTION_HPP
#define TAO_JSON_INTERNAL_UNESCAPE_ACTION_HPP

#include "../external/pegtl/contrib/unescape.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // clang-format off
         template< typename Rule > struct unescape_action {};

         template<> struct unescape_action< rules::escaped_unicode > : pegtl::unescape::unescape_j {};
         template<> struct unescape_action< rules::escaped_char > : pegtl::unescape::unescape_c< rules::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
         template<> struct unescape_action< rules::unescaped > : pegtl::unescape::append_all {};
         // clang-format on

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
