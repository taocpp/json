// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_TRANSFORMER_HPP
#define TAO_JSON_EVENTS_TRANSFORMER_HPP

#include <type_traits>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename... > class >
         struct invalid_transformer
         {
         };

         template< template< typename... > class T >
         struct report_invalid_transformer
         {
            // if this static assert is triggered there is a high chance that 'T' is
            // a traits class template and you intended to call a method starting with "basic_*",
            // e.g. basic_parse_file< my_traits >( ... ) instead of parse_file< my_traits >( ... ).
            static_assert( sizeof( invalid_transformer< T > ) == 0, "T is not a valid transformer" );
         };

         template< typename B, template< typename... > class T, typename = void >
         struct check_transformer
            : report_invalid_transformer< T >
         {
            using type = B;
         };

         template< typename B, template< typename... > class T >
         struct check_transformer< B,
                                   T,
                                   decltype( std::declval< T< B > >().null(),
                                             std::declval< T< B > >().boolean( true ),
                                             std::declval< T< B > >().number( double( 0.0 ) ),
                                             std::declval< T< B > >().string( "" ),
                                             std::declval< T< B > >().element(),
                                             std::declval< T< B > >().member(),
                                             void() ) >
         {
            using type = T< B >;
         };

         template< typename Consumer, template< typename... > class... Transformer >
         struct transformer;

         template< typename Consumer >
         struct transformer< Consumer >
         {
            using type = Consumer;
         };

         template< typename Consumer, template< typename... > class Head, template< typename... > class... Tail >
         struct transformer< Consumer, Head, Tail... >
            : check_transformer< typename transformer< Consumer, Tail... >::type, Head >
         {
         };

      }  // namespace internal

      namespace events
      {
         template< typename Consumer, template< typename... > class... Transformer >
         using transformer = typename internal::transformer< Consumer, Transformer... >::type;

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
