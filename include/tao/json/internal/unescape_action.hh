// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_UNESCAPE_ACTION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_UNESCAPE_ACTION_HH

#include "../external/pegtl/nothing.hh"
#include "../external/pegtl/contrib/unescape.hh"

#include "grammar.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule > struct unescape_action : tao_json_pegtl::nothing< Rule > {};

         template<> struct unescape_action< rules::unicode > : tao_json_pegtl::unescape::unescape_j {};
         template<> struct unescape_action< rules::escaped_char > : tao_json_pegtl::unescape::unescape_c< rules::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
         template<> struct unescape_action< rules::unescaped > : tao_json_pegtl::unescape::append_all {};

      } // internal

   } // json

} // tao

#endif
