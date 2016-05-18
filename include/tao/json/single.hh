// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SINGLE_HH
#define TAOCPP_JSON_INCLUDE_SINGLE_HH

#include <utility>

#include "pair.hh"

namespace tao
{
   namespace json
   {
      template< typename T >
      struct single
      {
         mutable T value;

         template< typename U >
         single( U && v ) : value( std::forward< U >( v ) ) {}

         single( std::initializer_list< pair< T > > && l ) : value( std::move( l ) ) {}
         single( const std::initializer_list< pair< T > > & l ) : value( l ) {}
         single( std::initializer_list< pair< T > > & l ) : value( l ) {}
      };

   } // json

} // tao

#endif
