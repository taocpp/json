// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_PAIR_HPP
#define TAO_JSON_INTERNAL_PAIR_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "../forward.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename... > class Traits, typename Base >
         struct pair
         {
            mutable std::string key;
            mutable basic_value< Traits, Base > value;

            template< typename U >
            pair( U&& v )  // NOLINT
               : key( Traits< typename std::decay< U >::type >::default_key ),
                 value( std::forward< U >( v ) )
            {
            }

            template< typename U >
            pair( U&& k, basic_value< Traits, Base >&& v )
               : key( std::forward< U >( k ) ), value( std::move( v ) )
            {
            }

            template< typename U >
            pair( U&& k, const basic_value< Traits, Base >& v )
               : key( std::forward< U >( k ) ), value( v )
            {
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
