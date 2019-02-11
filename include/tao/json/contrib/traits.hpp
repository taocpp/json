// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_TRAITS_HPP
#define TAO_JSON_CONTRIB_TRAITS_HPP

#include "../traits.hpp"

#include "pair_traits.hpp"
#include "tuple_traits.hpp"

#include "pointer_traits.hpp"

#include "array_traits.hpp"
#include "deque_traits.hpp"
#include "list_traits.hpp"
#include "set_traits.hpp"
#include "unordered_set_traits.hpp"
#include "vector_traits.hpp"

#include "map_traits.hpp"
#include "unordered_map_traits.hpp"

#include "multimap_traits.hpp"
#include "multiset_traits.hpp"

#include "shared_ptr_traits.hpp"
#include "unique_ptr_traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename U, typename V >
      struct traits< std::pair< U, V > >
         : public pair_traits< U, V >
      {
      };

      template< typename... Ts >
      struct traits< std::tuple< Ts... > >
         : public tuple_traits< Ts... >
      {
      };

      template<>
      struct traits< token >
         : public token_traits
      {
      };

      template<>
      struct traits< pointer >
         : public pointer_traits
      {
      };

      template< typename T, std::size_t N >
      struct traits< std::array< T, N > >
         : public array_traits< T, N >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::deque< T, Ts... > >
         : public deque_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::list< T, Ts... > >
         : public list_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::set< T, Ts... > >
         : public set_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::unordered_set< T, Ts... > >
         : public unordered_set_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::vector< T, Ts... > >
         : public vector_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::map< std::string, T, Ts... > >
         : public map_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::unordered_map< std::string, T, Ts... > >
         : public unordered_map_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::multiset< T, Ts... > >
         : public multiset_traits< T, Ts... >
      {
      };

      template< typename T, typename... Ts >
      struct traits< std::multimap< std::string, T, Ts... > >
         : public multimap_traits< T, Ts... >
      {
      };

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
