// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP

#include <string>

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

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
