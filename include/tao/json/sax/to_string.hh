// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_TO_STRING_HH
#define TAOCPP_JSON_INCLUDE_SAX_TO_STRING_HH

#include <sstream>

#include "to_stream.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX consumer to build a JSON string representation
         struct to_string : to_stream
         {
            std::ostringstream oss;

            to_string()
               : to_stream( oss )
            {
            }

            std::string value() const
            {
               return oss.str();
            }
         };

      }  // sax

   }  // json

}  // tao

#endif
