// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CBOR_FROM_BINARY_HPP
#define TAO_JSON_CBOR_FROM_BINARY_HPP

#include <utility>

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "events/from_binary.hpp"

namespace tao::json::cbor
{
   template< template< typename... > class Traits, template< typename... > class... Transformers, typename... Ts >
   [[nodiscard]] basic_value< Traits > basic_from_binary( Ts&&... ts )
   {
      json::events::transformer< json::events::to_basic_value< Traits >, Transformers... > consumer;
      events::from_binary( consumer, std::forward< Ts >( ts )... );
      return std::move( consumer.value );
   }

   template< template< typename... > class... Transformers, typename... Ts >
   [[nodiscard]] value from_binary( Ts&&... ts )
   {
      return basic_from_binary< traits, Transformers... >( std::forward< Ts >( ts )... );
   }

}  // namespace tao::json::cbor

#endif
