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
         template< std::size_t I, typename Tuple >
         decltype( auto ) get( Tuple& t ) noexcept( noexcept( std::get< I >( t ) ) )
         {
            return std::get< I >( t );
         }

         template< std::size_t I, typename Tuple >
         decltype( auto ) cget( const Tuple& t ) noexcept( noexcept( std::get< I >( t ) ) )
         {
            return std::get< I >( t );
         }

         template< typename Tuple, typename Indices >
         struct tuple_traits;

         template< typename Tuple, std::size_t... Is >
         struct tuple_traits< Tuple, std::index_sequence< Is... > >
         {
            template< std::size_t I >
            using helper_t = binding::element2< &cget< I, Tuple >, &get< I, Tuple > >;

            using type = binding::array< helper_t< Is >... >;
         };

      }  // namespace internal

      template< typename... Ts >
      struct tuple_traits
         : public internal::tuple_traits< std::tuple< Ts... >, std::index_sequence_for< Ts... > >::type
      {
      };

   }  // namespace json

}  // namespace tao

#endif
