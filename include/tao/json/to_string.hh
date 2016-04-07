// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
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
      inline std::string to_string( const value & v )
      {
         std::ostringstream o;
         internal::to_stream( o, v );
         return o.str();
      }

      inline std::string to_string( const value & v, const unsigned indent, const unsigned current = 0 )
      {
         std::ostringstream o;
         internal::to_stream( o, v, indent, current );
         return o.str();
      }

   } // json

} // tao

#endif
