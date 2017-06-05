// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_UBJSON_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_UBJSON_TO_STREAM_HPP

#include <ostream>

#include "../events/from_value.hpp"

#include "../events/non_finite_to_null.hpp"
#include "../events/ubjson/to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< template< typename... > class Traits >
         void to_stream( std::ostream& os, const basic_value< Traits >& v )
         {
            events::non_finite_to_null< events::ubjson::to_stream > consumer( os );
            events::from_value( v, consumer );
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
