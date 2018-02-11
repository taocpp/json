// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_PARSE_FILE_HPP
#define TAO_JSON_EVENTS_PARSE_FILE_HPP

#include <utility>

#include "../internal/action.hpp"
#include "../internal/control.hpp"
#include "../internal/grammar.hpp"

#include "../external/pegtl/file_input.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events producer to parse a file containing a JSON string representation.

         template< typename T, typename Consumer >
         void parse_file( Consumer& consumer, T&& filename )
         {
            json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
            json_pegtl::parse< internal::grammar, internal::action, internal::control >( in, consumer );
         }

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
