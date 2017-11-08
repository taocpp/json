// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_PARSE_FILE_HPP

#include <string>
#include <utility>

#include "events/parse_file.hpp"
#include "events/to_value.hpp"
#include "events/transformer.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class... Transformers >
      data parse_file( const std::string& filename )
      {
         events::transformer< events::to_value, Transformers... > consumer;
         events::parse_file( consumer, filename );
         return std::move( consumer.value );
      }

   }  // namespace json

}  // namespace tao

#endif
