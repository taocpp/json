// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_FROM_STRING_HPP

#include "../traits.hpp"

#include "../events/to_value.hpp"

#include "../events/msgpack/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class Traits = traits >
         basic_value< Traits > from_string( const std::string& text )
         {
            events::to_basic_value< Traits > consumer;
            events::msgpack::from_string( text, consumer, __PRETTY_FUNCTION__ );  // TODO: Source.
            return std::move( consumer.value );
         }

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
