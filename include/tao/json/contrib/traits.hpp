// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_TRAITS_HPP
#define TAO_JSON_CONTRIB_TRAITS_HPP

#include <tao/json/traits.hpp>

#include <tao/json/contrib/shared_ptr_traits.hpp>
#include <tao/json/contrib/unique_ptr_traits.hpp>

namespace tao
{
   namespace json
   {
      template< typename T >
      struct traits< std::shared_ptr< T > >
         : public shared_ptr_traits< T >
      {
      };

      template< typename T >
      struct traits< std::unique_ptr< T > >
         : public unique_ptr_traits< T >
      {
      };

   }  // namespace json

}  // namespace tao

#endif
