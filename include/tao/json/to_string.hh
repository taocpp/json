// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TO_STRING_HH
#define TAOCPP_JSON_INCLUDE_TO_STRING_HH

#include <sstream>

#include "value.hh"

#include "internal/to_stream.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      std::string to_string( const basic_value< Traits > & v )
      {
         std::ostringstream o;
         internal::to_stream( o, v );
         return o.str();
      }

      template< template< typename ... > class Traits >
      std::string to_string( const basic_value< Traits > & v, const unsigned indent )
      {
         std::ostringstream o;
         internal::to_stream( o, v, indent );
         return o.str();
      }

   } // json

} // tao

#endif
