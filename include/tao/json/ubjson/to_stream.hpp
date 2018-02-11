// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_TO_STREAM_HPP
#define TAO_JSON_UBJSON_TO_STREAM_HPP

#include <ostream>

#include "../value.hpp"

#include "../events/from_value.hpp"
#include "../events/non_finite_to_null.hpp"
#include "../events/transformer.hpp"

#include "events/to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace ubjson
      {
         template< template< typename... > class... Transformers, template< typename... > class Traits, typename Base >
         void to_stream( std::ostream& os, const basic_value< Traits, Base >& v )
         {
            json::events::transformer< ubjson::events::to_stream, Transformers..., json::events::non_finite_to_null > consumer( os );
            json::events::from_value( consumer, v );
         }

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
