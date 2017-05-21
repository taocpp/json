// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_TO_STRING_HPP

#include <sstream>

#include "to_stream.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      std::string to_string( const basic_value< Traits >& v )
      {
         std::ostringstream o;
         json::to_stream( o, v );
         return o.str();
      }

      template< template< typename... > class Traits >
      std::string to_string( const basic_value< Traits >& v, const unsigned indent )
      {
         std::ostringstream o;
         json::to_stream( o, v, indent );
         return o.str();
      }

   }  // namespace json

}  // namespace tao

#endif
