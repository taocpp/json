// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_MSGPACK_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_MSGPACK_PARSE_FILE_HPP

#include <utility>

#include "../../external/pegtl/file_input.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace msgpack
         {
            // Events producer to parse a file containing a MSGPACK string representation.

            template< typename T, typename Consumer >
            void parse_file( Consumer& consumer, T&& filename )
            {
               json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
               json_pegtl::parse< grammar >( in, consumer );
            }

         }  // namespace msgpack

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
