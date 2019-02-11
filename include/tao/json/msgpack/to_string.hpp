// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_TO_STRING_HPP
#define TAO_JSON_MSGPACK_TO_STRING_HPP

#include <string>

#include "../value.hpp"

#include "../events/from_value.hpp"
#include "../events/transformer.hpp"

#include "events/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class... Transformers, template< typename... > class Traits >
         std::string to_string( const basic_value< Traits >& v )
         {
            json::events::transformer< events::to_string, Transformers... > consumer;
            json::events::from_value( consumer, v );
            return consumer.value();
         }

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
