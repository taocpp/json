// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_APPLY_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_APPLY_HPP

#include "ref.hpp"
#include "transformer.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< template< typename... > class... Transformer, typename Consumer >
         transformer< ref< Consumer >, Transformer... > apply( Consumer& c ) noexcept( noexcept( transformer< ref< Consumer >, Transformer... >( c ) ) )
         {
            return transformer< ref< Consumer >, Transformer... >( c );
         }

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
