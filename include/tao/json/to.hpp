// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_TO_HPP
#define TAO_JSON_TO_HPP

#include "forward.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, template< typename... > class Traits, typename Base >
      void to( const basic_value< Traits, Base >& v, T& t )
      {
         v.to( t );
      }

   }  // namespace json

}  // namespace tao

#endif
