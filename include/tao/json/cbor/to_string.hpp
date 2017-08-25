// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_TO_STRING_HPP

#include <string>

#include "../data.hpp"

#include "../events/from_value.hpp"
#include "../events/transformer.hpp"

#include "../events/cbor/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class... Transformers >
         std::string to_string( const data& v )
         {
            events::transformer< events::cbor::to_string, Transformers... > consumer;
            events::from_value( consumer, v );
            return consumer.value();
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
