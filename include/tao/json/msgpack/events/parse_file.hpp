// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_EVENTS_PARSE_FILE_HPP
#define TAO_JSON_MSGPACK_EVENTS_PARSE_FILE_HPP

#include <utility>

#include "../../external/pegtl/file_input.hpp"
#include "../../external/pegtl/parse.hpp"

#include "../internal/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         namespace events
         {
            // Events producer to parse a file containing a MSGPACK string representation.

            template< typename T, typename Consumer >
            void parse_file( Consumer& consumer, T&& filename )
            {
               json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
               json_pegtl::parse< msgpack::internal::grammar >( in, consumer );
            }

         }  // namespace events

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
