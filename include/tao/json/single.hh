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
      template< template< typename ... > class >
      class basic_value;

      template< template< typename ... > class Traits >
      struct single
      {
         mutable basic_value< Traits > value;

         template< typename U >
         single( U && v ) : value( std::forward< U >( v ) ) {}

         single( std::initializer_list< pair< Traits > > && l ) : value( std::move( l ) ) {}
         single( const std::initializer_list< pair< Traits > > & l ) : value( l ) {}
         single( std::initializer_list< pair< Traits > > & l ) : value( l ) {}
      };

   } // json

} // tao

#endif
