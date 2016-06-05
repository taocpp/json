// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_OBJECT_OPERATORS_HH
#define TAOCPP_JSON_INCLUDE_OBJECT_OPERATORS_HH

#include "value.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      basic_value< Traits > & operator+= ( basic_value< Traits > & v, std::initializer_list< pair< Traits > > && l )
      {
         v.insert( std::move( l ) );
         return v;
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > & operator+= ( basic_value< Traits > & v, const std::initializer_list< pair< Traits > > & l )
      {
         v.insert( l );
         return v;
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > & operator-= ( basic_value< Traits > & v, const std::string & k )
      {
         if ( v.get_object().erase( k ) == 0 ) {
            throw std::runtime_error( "key not found: " + k );
         }
         return v;
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > & operator-= ( basic_value< Traits > & v, std::initializer_list< std::string > l )
      {
         auto & o = v.get_object();
         for ( const auto & k : l ) {
            if ( o.erase( k ) == 0 ) {
               throw std::runtime_error( "key not found: " + k );
            }
         }
         return v;
      }

   } // json

} // tao

#endif
