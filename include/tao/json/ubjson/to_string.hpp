// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_TO_STRING_HPP
#define TAO_JSON_UBJSON_TO_STRING_HPP

#include <string>

#include "../value.hpp"

#include "../events/from_value.hpp"
#include "../events/non_finite_to_null.hpp"
#include "../events/transformer.hpp"

#include "events/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< template< typename... > class... Transformers, template< typename... > class Traits, typename Base >
         std::string to_string( const basic_value< Traits, Base >& v )
         {
            json::events::transformer< ubjson::events::to_string, Transformers..., json::events::non_finite_to_null > consumer;
            json::events::from_value( consumer, v );
            return consumer.value();
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
