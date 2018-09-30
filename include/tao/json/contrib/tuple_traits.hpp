// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_TUPLE_TRAITS_HPP
#define TAO_JSON_CONTRIB_TUPLE_TRAITS_HPP

#include <tuple>
#include <utility>

#include <tao/json/binding.hpp>

namespace tao
{
   namespace json
   {
      template< typename T >
      struct tuple_traits;

      namespace internal
      {
         template< typename Tuple, typename Indices >
         struct tuple_traits;

         template< typename Tuple, std::size_t... Is >
         struct tuple_traits< Tuple, TAO_JSON_PEGTL_NAMESPACE::internal::index_sequence< Is... > >
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

      template< typename U, typename V >
      struct tuple_traits< std::pair< U, V > >
         : public binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                                  TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >
      {
      };

      template< typename... Ts >
      struct tuple_traits< std::tuple< Ts... > >
         : public internal::tuple_traits< std::tuple< Ts... >, TAO_JSON_PEGTL_NAMESPACE::internal::index_sequence_for< Ts... > >::type
      {
      };

   }  // namespace json

}  // namespace tao

#endif
