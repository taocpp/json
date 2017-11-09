// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_UBJSON_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_UBJSON_FROM_STRING_HPP

#include <utility>

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "../events/ubjson/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< template< typename... > class Traits, template< typename... > class... Transformers, typename... Ts >
         basic_value< Traits > basic_from_string( Ts&&... ts )
         {
            events::transformer< events::to_basic_value< Traits >, Transformers... > consumer;
            events::ubjson::from_string( consumer, std::forward< Ts >( ts )... );
            return std::move( consumer.value );
         }

         template< template< typename... > class... Transformers, typename... Ts >
         value from_string( Ts&&... ts )
         {
            return basic_from_string< traits, Transformers... >( std::forward< Ts >( ts )... );
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
