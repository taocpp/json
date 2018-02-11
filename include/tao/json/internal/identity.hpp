// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_IDENTITY_HPP
#define TAO_JSON_INTERNAL_IDENTITY_HPP

#include <type_traits>

namespace tao
{
   namespace internal
   {
      template< typename T >
      struct identity
      {
         using type = T;
      };

      template< typename T >
      using identity_t = typename identity< T >::type;

   }  // namespace internal

}  // namespace tao

#endif
