// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_PARSE_FILE_HPP
#define TAO_JSON_JAXN_PARSE_FILE_HPP

#include <string>
#include <utility>

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "events/parse_file.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class Traits, typename Base, template< typename... > class... Transformers >
         basic_value< Traits, Base > basic_parse_file( const std::string& filename )
         {
            json::events::transformer< json::events::to_basic_value< Traits, Base >, Transformers... > consumer;
            events::parse_file( consumer, filename );
            return std::move( consumer.value );
         }

         template< template< typename... > class Traits, template< typename... > class... Transformers >
         basic_value< Traits > basic_parse_file( const std::string& filename )
         {
            return basic_parse_file< Traits, json::internal::empty_base, Transformers... >( filename );
         }

         template< template< typename... > class... Transformers >
         value parse_file( const std::string& filename )
         {
            return basic_parse_file< traits, Transformers... >( filename );
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
