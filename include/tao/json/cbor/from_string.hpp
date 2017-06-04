// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP

#include "../traits.hpp"

#include "../events/to_value.hpp"

#include "../events/cbor/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class Traits = traits >
         basic_value< Traits > from_string( const std::string& text )
         {
            events::to_basic_value< Traits > consumer;
            events::cbor::from_string( text, consumer, __PRETTY_FUNCTION__ );  // TODO: Source.
            return std::move( consumer.value );
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
