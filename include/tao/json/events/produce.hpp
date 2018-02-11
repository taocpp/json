// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_PRODUCE_HPP
#define TAO_JSON_EVENTS_PRODUCE_HPP

#include <type_traits>
#include <utility>

#include "../forward.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< template< typename... > class Traits = traits, typename Consumer, typename T >
         void produce( Consumer& c, T&& t )
         {
            Traits< typename std::decay< T >::type >::template produce< Traits >( c, std::forward< T >( t ) );
         }

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
