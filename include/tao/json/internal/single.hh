// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_SINGLE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_SINGLE_HH

#include <utility>

#include "pair.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct single
         {
            mutable T e;

            template< typename U >
            single( U && v ) : e( std::forward< U >( v ) ) {}

            single( std::initializer_list< pair< T > > && l ) : e( std::move( l ) ) {}
            single( const std::initializer_list< pair< T > > & l ) : e( l ) {}
            single( std::initializer_list< pair< T > > & l ) : e( l ) {}
         };

      } // internal

   } // json

} // tao

#endif
