// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_SINGLE_HPP
#define TAO_JSON_INTERNAL_SINGLE_HPP

#include <utility>

#include "../forward.hpp"

#include "pair.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename... > class Traits, typename Base >
         struct single
         {
            mutable basic_value< Traits, Base > value;

            template< typename U >
            single( U&& v )  // NOLINT
               : value( std::forward< U >( v ) )
            {
            }

            single( std::initializer_list< pair< Traits, Base > >&& l )
               : value( std::move( l ) )
            {
            }

            single( const std::initializer_list< pair< Traits, Base > >& l )
               : value( l )
            {
            }

            single( std::initializer_list< pair< Traits, Base > >& l )
               : value( l )
            {
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
