// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON5_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON5_PARSE_FILE_HPP

#include <utility>

#include "../../internal/json5/action.hpp"
#include "../../internal/json5/control.hpp"
#include "../../internal/json5/grammar.hpp"

#include "../../external/pegtl/file_input.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace json5
         {
            // Events producer to parse a file containing a JSON5 string representation.

            template< typename T, typename Consumer >
            void parse_file( T&& filename, Consumer& consumer )
            {
               json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
               json_pegtl::parse< internal::json5::grammar, internal::json5::action, internal::json5::control >( in, consumer );
            }

         }  // namespace json5

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
