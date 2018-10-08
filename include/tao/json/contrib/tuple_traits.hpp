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
         template< typename Tuple, typename Indices >
         struct tuple_traits;

         template< typename Tuple, std::size_t... Is >
         struct tuple_traits< Tuple, std::index_sequence< Is... > >
         {
            template< std::size_t I >
            struct cf_i
            {
               using type = decltype( std::get< I >( std::declval< const Tuple& >() ) ) ( * )( const Tuple& );
            };

            template< std::size_t I >
            using cf = typename cf_i< I >::type;

            template< std::size_t I >
            struct f_i
            {
               using type = decltype( std::get< I >( std::declval< Tuple& >() ) ) ( * )( Tuple& );
            };

            template< std::size_t I >
            using f = typename f_i< I >::type;

            template< std::size_t I >
            struct e_i
            {
#ifdef _MSC_VER
               static constexpr const cf< I > cdummy = &std::get< I >;
               static constexpr const f< I > dummy = &std::get< I >;
#endif
               using type = binding::element2< cf< I >, &std::get< I >, f< I >, &std::get< I > >;
            };

            template< std::size_t I >
            using e = typename e_i< I >::type;

            using type = binding::array< e< Is >... >;
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
