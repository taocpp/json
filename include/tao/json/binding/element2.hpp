// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_ELEMENT2_HPP
#define TAO_JSON_BINDING_ELEMENT2_HPP

#include "internal/elementr.hpp"
#include "internal/elementw.hpp"

namespace tao::json::binding
{
   namespace internal
   {
      template< typename GT, GT G, typename ST, ST S >
      struct element2
         : elementr< GT, G >,
           elementw< ST, S >
      {};

   }  // namespace internal

   template< auto G, auto S >
   using element2 = internal::element2< decltype( G ), G, decltype( S ), S >;

}  // namespace tao::json::binding

#endif
