// Copyright (c) 2018-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_EVENTS_FROM_FILE_HPP
#define TAO_JSON_UBJSON_EVENTS_FROM_FILE_HPP

#include "tao/pegtl/file_input.hpp"
#include "tao/pegtl/parse.hpp"

#include "../../internal/filesystem.hpp"

#include "../internal/grammar.hpp"

namespace tao::json::ubjson::events
{
   // Events producer to parse a file containing a UBJSON string representation.

   template< typename Consumer >
   void from_file( Consumer& consumer, const json::internal::filesystem::path& path )
   {
      pegtl::file_input< pegtl::tracking_mode::lazy > in( path );
      pegtl::parse< internal::grammar >( in, consumer );
   }

}  // namespace tao::json::ubjson::events

#endif
