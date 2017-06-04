// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_TO_STRING_HPP

#include <string>

#include "../events/from_value.hpp"

#include "../events/msgpack/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class Traits >
         std::string to_string( const basic_value< Traits >& v )
         {
            events::msgpack::to_string consumer;
            events::from_value( v, consumer );
            return consumer.value();
         }

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
