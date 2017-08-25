// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "../value.hpp"

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

         template< template< typename... > class Traits, template< typename... > class... Transformers, typename... Ts >
         basic_custom_value< Traits > basic_custom_from_string( Ts&&... ts )
         {
            return from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

         template< template< typename... > class... Transformers, typename... Ts >
         custom_value custom_from_string( Ts&&... ts )
         {
            return from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

         template< template< typename... > class... Transformers, template< typename... > class Traits, typename... Ts >
         void from_string( basic_custom_value< Traits >& output, Ts&&... ts )
         {
            output = from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
