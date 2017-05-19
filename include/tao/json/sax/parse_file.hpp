// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HPP

#include <utility>

#include "../internal/action.hpp"
#include "../internal/control.hpp"
#include "../internal/grammar.hpp"

#include "../external/pegtl/file_input.hpp"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX producer to parse a file containing a JSON string representation
         template< typename T, typename Consumer >
         void parse_file( T&& filename, Consumer& consumer )
         {
            json_pegtl::file_input< json_pegtl::tracking_mode::LAZY > in( std::forward< T >( filename ) );
            json_pegtl::parse< internal::grammar, internal::action, internal::control >( in, consumer );
         }

      }  // sax

   }  // json

}  // tao

#endif
