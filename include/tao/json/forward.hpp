// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_FORWARD_HPP
#define TAO_JSON_FORWARD_HPP

#include <cstddef>
#include <vector>

namespace tao
{
   using binary = std::vector< std::byte >;

   namespace json
   {
      namespace events
      {
         class virtual_base;

      }  // namespace events

      namespace internal
      {
         template< template< typename... > class Traits >
         struct single;

         template< template< typename... > class Traits >
         struct pair;

      }  // namespace internal

      template< typename T, typename = void >
      struct traits
      {
      };

      template< template< typename... > class Traits >
      class basic_value;

      using producer_t = void ( * )( events::virtual_base&, const void* );

   }  // namespace json

}  // namespace tao

#endif
