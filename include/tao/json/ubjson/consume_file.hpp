// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_CONSUME_FILE_HPP
#define TAO_JSON_UBJSON_CONSUME_FILE_HPP

#include <utility>

#include "../external/pegtl/file_input.hpp"

#include "../consume.hpp"
#include "../forward.hpp"

#include "parts_parser.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< typename T, template< typename... > class Traits = traits, typename F >
         T consume_file( F&& filename )
         {
            ubjson::basic_parts_parser< 1 << 24, utf8_mode::check, json_pegtl::file_input< json_pegtl::tracking_mode::lazy > > pp( std::forward< F >( filename ) );
            return json::consume< T, Traits >( pp );
         }

         template< template< typename... > class Traits = traits, typename F, typename T >
         T consume_file( F&& filename, T& t )
         {
            ubjson::basic_parts_parser< 1 << 24, utf8_mode::check, json_pegtl::file_input< json_pegtl::tracking_mode::lazy > > pp( std::forward< F >( filename ) );
            return json::consume< Traits >( pp, t );
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
