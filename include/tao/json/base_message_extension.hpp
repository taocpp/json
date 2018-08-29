// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BASE_MESSAGE_EXTENSION_HPP
#define TAO_JSON_BASE_MESSAGE_EXTENSION_HPP

#include <string>

namespace tao
{
   namespace json
   {
      template< typename T >
      std::string base_message_extension( const T& )
      {
         return typeid( T ).name();
      }

   }  // namespace json

}  // namespace tao

#endif
