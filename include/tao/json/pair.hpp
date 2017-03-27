// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PAIR_HPP
#define TAOCPP_JSON_INCLUDE_PAIR_HPP

#include <string>
#include <type_traits>
#include <utility>

namespace tao
{
   namespace json
   {
      template< template< typename... > class >
      class basic_value;

      template< template< typename... > class Traits >
      struct pair
      {
         mutable std::string key;
         mutable basic_value< Traits > value;

         template< typename U >
         pair( U&& v )
            : key( Traits< typename std::decay< U >::type >::default_key ),
              value( std::forward< U >( v ) )
         {
         }

         pair( std::string&& k, basic_value< Traits >&& v )
            : key( std::move( k ) ), value( std::move( v ) )
         {
         }

         pair( std::string&& k, const basic_value< Traits >& v )
            : key( std::move( k ) ), value( v )
         {
         }

         pair( const std::string& k, basic_value< Traits >&& v )
            : key( k ), value( std::move( v ) )
         {
         }

         pair( const std::string& k, const basic_value< Traits >& v )
            : key( k ), value( v )
         {
         }

         pair( const char* k, basic_value< Traits >&& v )
            : key( k ), value( std::move( v ) )
         {
         }

         pair( const char* k, const basic_value< Traits >& v )
            : key( k ), value( v )
         {
         }
      };

   }  // json

}  // tao

#endif
