// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CONSUME_HPP
#define TAOCPP_JSON_INCLUDE_CONSUME_HPP

#include <type_traits>

#include "traits.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
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

      template< typename T, template< typename... > class Traits = traits, typename Producer >
      typename std::enable_if< internal::has_consume_one< Traits, Producer, T >::value, T >::type consume( Producer& producer )
      {
         return Traits< T >::template consume< Traits >( producer );
      }

      template< typename T, template< typename... > class Traits = traits, typename Producer >
      typename std::enable_if< !internal::has_consume_one< Traits, Producer, T >::value, T >::type consume( Producer& producer )
      {
         T t;
         Traits< T >::template consume< Traits >( producer, t );
         return t;
      }

      template< template< typename... > class Traits = traits, typename Producer, typename T >
      typename std::enable_if< internal::has_consume_two< Traits, Producer, T >::value, void >::type consume( Producer& producer, T& t )
      {
         Traits< T >::template consume< Traits >( producer, t );
      }

      template< template< typename... > class Traits = traits, typename Producer, typename T >
      typename std::enable_if< !internal::has_consume_two< Traits, Producer, T >::value, void >::type consume( Producer& producer, T& t )
      {
         t = Traits< T >::template consume< Traits >( producer );
      }

   }  // namespace json

}  // namespace tao

#endif
