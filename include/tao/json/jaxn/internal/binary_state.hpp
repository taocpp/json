// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_BINARY_STATE_HPP
#define TAO_JSON_JAXN_INTERNAL_BINARY_STATE_HPP

#include <utility>
#include <vector>

#include "../../external/byte.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            struct binary_state
            {
               binary_state() = default;

               binary_state( const binary_state& ) = delete;
               binary_state( binary_state&& ) = delete;

               ~binary_state() = default;

               void operator=( const binary_state& ) = delete;
               void operator=( binary_state&& ) = delete;

               template< typename Consumer >
               void success( Consumer& consumer )
               {
                  consumer.binary( std::move( value ) );
               }

               std::vector< tao::byte > value;
            };

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
