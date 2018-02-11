// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_TO_STRING_HPP
#define TAO_JSON_TO_STRING_HPP

#include <sstream>

#include "to_stream.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class... Transformers, typename... Ts >
      std::string to_string( Ts&&... ts )
      {
         std::ostringstream o;
         json::to_stream< Transformers... >( o, std::forward< Ts >( ts )... );
         return o.str();
      }

   }  // namespace json

}  // namespace tao

#endif
