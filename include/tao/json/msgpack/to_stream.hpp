// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_TO_STREAM_HPP

#include <ostream>

#include "../events/from_value.hpp"

#include "../events/msgpack/to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class Traits >
         void to_stream( std::ostream& os, const basic_value< Traits >& v )
         {
            events::msgpack::to_stream consumer( os );
            events::from_value( v, consumer );
         }

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
