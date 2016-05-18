// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PAIR_HH
#define TAOCPP_JSON_INCLUDE_PAIR_HH

#include <string>
#include <utility>
#include <type_traits>

#include "traits.hh"

namespace tao
{
   namespace json
   {
      template< typename T >
      struct pair
      {
         mutable std::string key;
         mutable T value;

         template< typename U >
         pair( U && v ) : key( traits< typename std::decay< U >::type >::default_key ), value( std::forward< U >( v ) ) {}

         pair( std::string && k, T && v ) : key( std::move( k ) ), value( std::move( v ) ) {}
         pair( std::string && k, const T & v ) : key( std::move( k ) ), value( v ) {}
         pair( const std::string & k, T && v ) : key( k ), value( std::move( v ) ) {}
         pair( const std::string & k, const T & v ) : key( k ), value( v ) {}
         pair( const char * k, T && v ) : key( k ), value( std::move( v ) ) {}
         pair( const char * k, const T & v ) : key( k ), value( v ) {}
      };

   } // json

} // tao

#endif
