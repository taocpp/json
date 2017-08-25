// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP

#include <string>

#include "../value.hpp"

#include "../events/to_value.hpp"

#include "../events/msgpack/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class... Transformers, typename... Ts >
         data from_string( Ts&&... ts )
         {
            events::transformer< events::to_value, Transformers... > consumer;
            events::msgpack::from_string( consumer, std::forward< Ts >( ts )... );
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

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
