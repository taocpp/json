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

         template< typename, typename, typename = void >
         struct has_as_one : std::false_type
         {
         };

         template< typename T, typename V >
         struct has_as_one< T, V, decltype( T::as( std::declval< const V& >() ), void() ) > : std::true_type
         {
         };

         template< typename, typename, typename, typename = void >
         struct has_as_two : std::false_type
         {
         };

         template< typename T, typename V, typename U >
         struct has_as_two< T, V, U, decltype( T::as( std::declval< const V& >(), std::declval< U& >() ), void() ) > : std::true_type
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

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
