// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_AS_HPP
#define TAO_JSON_AS_HPP

#include "forward.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, template< typename... > class Traits, typename Base >
      void as( const basic_value< Traits, Base >& v, T& t )
      {
         v.as( t );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      T as( const basic_value< Traits, Base >& v )
      {
         return v.template as< T >();
      }

   }  // namespace json

}  // namespace tao

#endif
