// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_INCLUDE_INTERNAL_IDENTITY_HPP
#define TAOCPP_INCLUDE_INTERNAL_IDENTITY_HPP

#include <type_traits>

namespace tao
{
   namespace internal
   {
      template< typename T >
      using identity = typename std::decay< T >::type;

   }  // namespace internal

}  // namespace tao

#endif
