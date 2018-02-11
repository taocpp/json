// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_TRANSFORMER_HPP
#define TAO_JSON_EVENTS_TRANSFORMER_HPP

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Consumer, template< typename... > class... Transformer >
         struct transformer;

         template< typename Consumer >
         struct transformer< Consumer >
         {
            using type = Consumer;
         };

         template< typename Consumer, template< typename... > class Head, template< typename... > class... Tail >
         struct transformer< Consumer, Head, Tail... >
         {
            using type = Head< typename transformer< Consumer, Tail... >::type >;
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
