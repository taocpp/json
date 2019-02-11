// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_BINARY_STATE_HPP
#define TAO_JSON_JAXN_INTERNAL_BINARY_STATE_HPP

#include <cstddef>
#include <utility>
#include <vector>

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
               template< typename Input, typename... States >
               explicit binary_state( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
               {
               }

               binary_state( const binary_state& ) = delete;
               binary_state( binary_state&& ) = delete;

               ~binary_state() = default;

               void operator=( const binary_state& ) = delete;
               void operator=( binary_state&& ) = delete;

               template< typename Input, typename Consumer >
               void success( const Input& /*unused*/, Consumer& consumer )
               {
                  consumer.binary( std::move( value ) );
               }

               std::vector< std::byte > value;
            };

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
