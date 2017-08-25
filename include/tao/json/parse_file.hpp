// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_PARSE_FILE_HPP

#include <string>
#include <utility>

#include "value.hpp"

#include "events/parse_file.hpp"
#include "events/to_value.hpp"
#include "events/transformer.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class... Transformers >
      data parse_file( const std::string& filename )
      {
         events::transformer< events::to_value, Transformers... > consumer;
         events::parse_file( consumer, filename );
         return std::move( consumer.value );
      }

      template< template< typename... > class Traits, template< typename... > class... Transformers >
      basic_custom_value< Traits > basic_custom_parse_file( const std::string& filename )
      {
         return parse_file< Transformers... >( filename );
      }

      template< template< typename... > class... Transformers >
      custom_value custom_parse_file( const std::string& filename )
      {
         return parse_file< Transformers... >( filename );
      }

      template< template< typename... > class... Transformers, template< typename... > class Traits >
      void parse_file( basic_custom_value< Traits >& output, const std::string& filename )
      {
         output = parse_file< Transformers... >( filename );
      }

   }  // namespace json

}  // namespace tao

#endif
