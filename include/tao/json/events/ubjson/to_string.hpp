// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON_UBJSON_TO_STRING_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON_UBJSON_TO_STRING_HPP

#include <sstream>
#include <string>

#include "to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace ubjson
         {
            struct to_string
               : public to_stream
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

         }  // namespace ubjson

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
