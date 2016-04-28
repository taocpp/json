// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_PAIR_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_PAIR_HH

#include <string>
#include <utility>
#include <type_traits>

#include "../default_key.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct pair
         {
            mutable std::pair< std::string, T > e;

            template< typename U >
            pair( U && v ) : e( default_key< typename std::decay< U >::type >::value, std::forward< U >( v ) ) {}

            pair( std::string && k, T && v ) : e( std::move( k ), std::move( v ) ) {}
            pair( std::string && k, const T & v ) : e( std::move( k ), v ) {}
            pair( const std::string & k, T && v ) : e( k, std::move( v ) ) {}
            pair( const std::string & k, const T & v ) : e( k, v ) {}
            pair( const char * k, T && v ) : e( k, std::move( v ) ) {}
            pair( const char * k, const T & v ) : e( k, v ) {}
         };

      } // internal

   } // json

} // tao

#endif
