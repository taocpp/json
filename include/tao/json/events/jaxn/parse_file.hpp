// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JAXN_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JAXN_PARSE_FILE_HPP

#include <utility>

#include "../../internal/jaxn/action.hpp"
#include "../../internal/jaxn/control.hpp"
#include "../../internal/jaxn/grammar.hpp"

#include "../../external/pegtl/file_input.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace jaxn
         {
            // Events producer to parse a file containing a JAXN string representation.

            template< typename T, typename Consumer >
            void parse_file( T&& filename, Consumer& consumer )
            {
               json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
               json_pegtl::parse< internal::jaxn::grammar, internal::jaxn::action, internal::jaxn::control >( in, consumer );
            }

         }  // namespace jaxn

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
