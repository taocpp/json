// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
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
#if defined( __GNUC__ ) && ( __GNUC__ >= 7 )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

         template< typename... Ts >
         struct type_list
         {
            static constexpr std::size_t size = sizeof...( Ts );
         };

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 )
#pragma GCC diagnostic pop
#endif

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

         template< typename T >
         struct is_basic_value
            : public std::false_type
         {
         };

         template< template< typename... > class Traits, typename Base >
         struct is_basic_value< basic_value< Traits, Base > >
            : public std::true_type
         {
         };

         template< typename, typename, typename, typename = void >
         struct has_as_impl : std::false_type
         {
         };

         template< typename T, typename V, typename... With >
         struct has_as_impl< T, V, type_list< With... >, decltype( T::as( std::declval< const V& >(), std::declval< With& >()... ), void() ) > : std::true_type
         {
         };

         template< typename T, typename V, typename... With >
         struct has_as
            : public has_as_impl< T, V, type_list< With... > >
         {
         };

         template< typename, typename, typename, typename, typename = void >
         struct has_to_impl : std::false_type
         {
         };

         template< typename T, typename V, typename U, typename... With >
         struct has_to_impl< T, V, U, type_list< With... >, decltype( T::to( std::declval< const V& >(), std::declval< U& >(), std::declval< With& >()... ), void() ) > : std::true_type
         {
         };

         template< typename T, typename V, typename U, typename... With >
         struct has_to
            : public has_to_impl< T, V, U, type_list< With... > >
         {
         };

         template< template< typename... > class, typename, typename, typename = void >
         struct has_consume_one : std::false_type
         {
         };

         template< template< typename... > class Traits, typename P, typename U >
         struct has_consume_one< Traits, P, U, decltype( Traits< U >::template consume< Traits >( std::declval< P& >() ), void() ) > : std::true_type
         {
         };

         template< template< typename... > class, typename, typename, typename = void >
         struct has_consume_two : std::false_type
         {
         };

         template< template< typename... > class Traits, typename P, typename U >
         struct has_consume_two< Traits, P, U, decltype( Traits< U >::template consume< Traits >( std::declval< P& >(), std::declval< U& >() ), void() ) > : std::true_type
         {
         };

         template< template< typename... > class Traits, typename T, typename = void >
         struct has_is_nothing : std::false_type
         {
         };

         template< template< typename... > class Traits, typename T >
         struct has_is_nothing< Traits, T, decltype( Traits< T >::template is_nothing< Traits >( std::declval< const T& >() ), void() ) > : std::true_type
         {
         };

         template< template< typename... > class Traits, typename T >
         auto is_nothing( const T& t ) -> typename std::enable_if< has_is_nothing< Traits, T >::value, bool >::type
         {
            return Traits< T >::template is_nothing< Traits >( t );
         }

         template< template< typename... > class Traits, typename T >
         auto is_nothing( const T & /*unused*/ ) -> typename std::enable_if< !has_is_nothing< Traits, T >::value, bool >::type
         {
            return false;
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
