// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_BINARY_STATE_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_BINARY_STATE_HPP

#include <utility>
#include <vector>

#include "../../byte.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace jaxn
         {
            struct binary_state
            {
               binary_state() = default;
               binary_state( const binary_state& ) = delete;
               void operator=( const binary_state& ) = delete;

               template< typename Consumer >
               void success( Consumer& consumer )
               {
                  consumer.binary( std::move( value ) );
               }

               std::vector< byte > value;
            };

         }  // namespace jaxn

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
