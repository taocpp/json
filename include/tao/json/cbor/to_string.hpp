// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_TO_STRING_HPP

#include <string>

#include "../events/from_value.hpp"

#include "../events/cbor/to_string.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class Traits >
         std::string to_string( const basic_value< Traits >& v )
         {
            events::cbor::to_string consumer;
            events::from_value( v, consumer );
            return consumer.value();
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
