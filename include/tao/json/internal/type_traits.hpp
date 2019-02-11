// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_TYPE_TRAITS_HPP
#define TAO_JSON_INTERNAL_TYPE_TRAITS_HPP

#include <type_traits>
#include <typeinfo>

#include "../forward.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename... Ts >
         struct type_list
         {
            static constexpr std::size_t size = sizeof...( Ts );
         };

         template< typename... >
         struct merge_type_lists_t;

         template< typename... Ts >
         struct merge_type_lists_t< type_list< Ts... > >
         {
            using list = type_list< Ts... >;
         };

         template< typename... Vs, typename... Ws, typename... Rs >
         struct merge_type_lists_t< type_list< Vs... >, type_list< Ws... >, Rs... >
            : public merge_type_lists_t< type_list< Vs..., Ws... >, Rs... >
         {
         };

         template< typename... Ts >
         using merge_type_lists = typename merge_type_lists_t< Ts... >::list;

         struct type_info_less
         {
            bool operator()( const std::type_info* l, const std::type_info* r ) const
            {
               return l->before( *r );
            }
         };

         template< typename, typename... >
         inline constexpr bool has_as_impl = false;

         template< typename T, typename... Args >
         inline constexpr bool has_as_impl< decltype( T::as( std::declval< Args >()... ), void() ), T, Args... > = true;

         template< typename T, typename V, typename... With >
         inline constexpr bool has_as = has_as_impl< void, T, const V&, With&... >;

         template< typename, typename... >
         inline constexpr bool has_to_impl = false;

         template< typename T, typename... Args >
         inline constexpr bool has_to_impl< decltype( T::to( std::declval< Args >()... ), void() ), T, Args... > = true;

         template< typename T, typename V, typename... With >
         inline constexpr bool has_to = has_to_impl< void, T, const V&, With&... >;

         template< template< typename... > class, typename, typename, typename = void >
         inline constexpr bool has_consume_one = false;

         template< template< typename... > class Traits, typename P, typename U >
         inline constexpr bool has_consume_one< Traits, P, U, decltype( Traits< U >::template consume< Traits >( std::declval< P& >() ), void() ) > = true;

         template< template< typename... > class, typename, typename, typename = void >
         inline constexpr bool has_consume_two = false;

         template< template< typename... > class Traits, typename P, typename U >
         inline constexpr bool has_consume_two< Traits, P, U, decltype( Traits< U >::template consume< Traits >( std::declval< P& >(), std::declval< U& >() ), void() ) > = true;

         template< template< typename... > class Traits, typename T, typename = void >
         inline constexpr bool has_is_nothing = false;

         template< template< typename... > class Traits, typename T >
         inline constexpr bool has_is_nothing< Traits, T, decltype( Traits< T >::template is_nothing< Traits >( std::declval< const T& >() ), void() ) > = true;

         template< template< typename... > class Traits, typename T >
         std::enable_if_t< has_is_nothing< Traits, T >, bool > is_nothing( const T& t ) noexcept( noexcept( Traits< T >::template is_nothing< Traits >( t ) ) )
         {
            return Traits< T >::template is_nothing< Traits >( t );
         }

         template< template< typename... > class Traits, typename T >
         std::enable_if_t< !has_is_nothing< Traits, T >, bool > is_nothing( const T& /*unused*/ ) noexcept
         {
            return false;
         }

         template< template< typename... > class Traits, typename T, typename = void >
         inline constexpr bool has_enable_implicit_constructor = false;

         template< template< typename... > class Traits, typename T >
         inline constexpr bool has_enable_implicit_constructor< Traits, T, decltype( Traits< T >::enable_implicit_constructor, void() ) > = true;

         template< template< typename... > class Traits, typename T >
         inline constexpr bool enable_implicit_constructor = Traits< std::conditional_t< has_enable_implicit_constructor< Traits, T >, T, void > >::enable_implicit_constructor;

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
