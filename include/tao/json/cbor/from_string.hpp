// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "../events/cbor/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class... Transformers, typename... Ts >
         data from_string( Ts&&... ts )
         {
            events::transformer< events::to_value, Transformers... > consumer;
            events::cbor::from_string( consumer, std::forward< Ts >( ts )... );
            return std::move( consumer.value );
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
