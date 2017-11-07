// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JAXN_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_JAXN_PARSE_FILE_HPP

#include <string>
#include <utility>

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "../events/jaxn/parse_file.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class... Transformers >
         data parse_file( const std::string& filename )
         {
            events::transformer< events::to_value, Transformers... > consumer;
            events::jaxn::parse_file( consumer, filename );
            return std::move( consumer.value );
         }

         template< template< typename... > class... Transformers >
         void parse_file( data& output, const std::string& filename )
         {
            output = parse_file< Transformers... >( filename );
         }

         template< template< typename... > class... Transformers, template< typename... > class Traits >
         void parse_file( basic_value< Traits >& output, const std::string& filename )
         {
            output = parse_file< Transformers... >( filename );
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
