// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_EVENTS_TO_STRING_HPP
#define TAO_JSON_JAXN_EVENTS_TO_STRING_HPP

#include <sstream>
#include <string>

#include "to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace events
         {
            // Events consumer to build a JAXN string representation.

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

         }  // namespace events

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
