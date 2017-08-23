// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_STORE_VIEWS_HPP
#define TAOCPP_JSON_INCLUDE_TRAITS_STORE_VIEWS_HPP

#include "traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, typename = void >
      struct traits_store_views
         : traits< T >
      {
      };

      template<>
      struct traits_store_views< tao::string_view >
         : traits< tao::string_view >
      {
         static void assign( data& v, const tao::string_view sv )
         {
            v.unsafe_assign_string_view( sv );
         }
      };

   }  // namespace json

}  // namespace tao

#endif
