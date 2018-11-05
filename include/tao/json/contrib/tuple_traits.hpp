// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_TUPLE_TRAITS_HPP
#define TAO_JSON_CONTRIB_TUPLE_TRAITS_HPP

#include <tuple>
#include <utility>

#include "../binding.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< std::size_t I, typename... Ts >
         const std::tuple_element_t< I, std::tuple< Ts... > >& cget( const std::tuple< Ts... >& t ) noexcept
         {
            return std::get< I >( t );
         }

         template< std::size_t I, typename... Ts >
         std::tuple_element_t< I, std::tuple< Ts... > >& get( std::tuple< Ts... >& t ) noexcept
         {
            return std::get< I >( t );
         }

         template< typename Indices, typename... Ts >
         struct tuple_traits;

         template< std::size_t... Is, typename... Ts >
         struct tuple_traits< std::index_sequence< Is... >, Ts... >
         {
            template< std::size_t I >
            using helper_t = binding::element2< static_cast< std::tuple_element_t< I, const std::tuple< Ts... > >& (*)(const std::tuple< Ts... >&)noexcept >( &cget< I > ), static_cast< std::tuple_element_t< I, std::tuple< Ts... > >& (*)(std::tuple< Ts... >&)noexcept >( &get< I > ) >;

            using type = binding::array< helper_t< Is >... >;
         };

      }  // namespace internal

      template< typename... Ts >
      struct tuple_traits
         : internal::tuple_traits< std::index_sequence_for< Ts... >, Ts... >::type
      {
      };

   }  // namespace json

}  // namespace tao

#endif
