// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_TO_STREAM_HPP

#include <ostream>

#include "../events/from_value.hpp"

#include "../events/cbor/to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class Traits >
         void to_stream( std::ostream& os, const basic_value< Traits >& v )
         {
            events::cbor::to_stream consumer( os );
            events::from_value( v, consumer );
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
