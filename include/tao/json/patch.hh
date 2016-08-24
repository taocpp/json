// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PATCH_HH
#define TAOCPP_JSON_INCLUDE_PATCH_HH

#include "value.hh"
#include "json_pointer.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      void patch_inplace( basic_value< Traits > & value, const basic_value< Traits > & patch )
      {
        for( const auto & entry : patch.get_array() ) {
          const auto & op = entry.at( "op" ).get_string();
          const json_pointer path( entry.at( "path" ).get_string() );
          if( op == "test" ) {
            if( value.at( path ) != entry.at( "value" ) ) {
              throw std::runtime_error( "test failed for: " + path.value() );
            }
          }
          else if( op == "remove" ) {
            value.erase( path );
          }
          else if( op == "add" ) {
            value.add( path ) = entry.at( "value" );
          }
          else if( op == "replace" ) {
            value.at( path ) = entry.at( "value" );
          }
          else if( op == "move" ) {
            const json_pointer from( entry.at( "from" ).get_string() );
            auto v = std::move( value.at( from ) );
            value.erase( from );
            value.add( path ) = std::move( v );
          }
          else if( op == "copy" ) {
            const json_pointer from( entry.at( "from" ).get_string() );
            auto v = value.at( from );
            value.add( path ) = std::move( v );
          }
          else {
            throw std::runtime_error( "unknown patch operation: '" + op + '\'' );
          }
        }
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > patch( const basic_value< Traits > & value, const basic_value< Traits > & patch )
      {
        basic_value< Traits > result = value;
        patch_inplace( result, patch );
        return result;
      }

   } // json

} // tao

#endif
