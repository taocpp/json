// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_UBJSON_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_UBJSON_TO_STRING_HPP

#include <string>

#include "../value.hpp"

#include "../events/from_value.hpp"
#include "../events/non_finite_to_null.hpp"
#include "../events/transformer.hpp"

#include "../events/ubjson/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< template< typename... > class... Transformers, template< typename... > class Traits >
         std::string to_string( const basic_value< Traits >& v )
         {
            events::transformer< events::ubjson::to_string, Transformers..., events::non_finite_to_null > consumer;
            events::from_value( consumer, v );
            return consumer.value();
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
