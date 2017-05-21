// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PATCH_HPP
#define TAOCPP_JSON_INCLUDE_PATCH_HPP

#include <stdexcept>
#include <utility>

#include "pointer.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      void patch_inplace( basic_value< Traits >& value, const basic_value< Traits >& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( value.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );
               }
            }
            else if( op == "remove" ) {
               value.erase( path_pointer );
            }
            else if( op == "add" ) {
               value.insert( path_pointer, entry.at( "value" ) );
            }
            else if( op == "replace" ) {
               value.at( path_pointer ) = entry.at( "value" );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto v = std::move( value.at( from ) );
               value.erase( from );
               value.insert( path_pointer, std::move( v ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               value.insert( path_pointer, value.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );
            }
         }
      }

      template< template< typename... > class Traits >
      void patch_inplace( basic_value< Traits >& value, basic_value< Traits >&& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( value.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );
               }
            }
            else if( op == "remove" ) {
               value.erase( path_pointer );
            }
            else if( op == "add" ) {
               value.insert( path_pointer, std::move( entry.at( "value" ) ) );
            }
            else if( op == "replace" ) {
               value.at( path_pointer ) = std::move( entry.at( "value" ) );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto v = std::move( value.at( from ) );
               value.erase( from );
               value.insert( path_pointer, std::move( v ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               value.insert( path_pointer, value.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );
            }
         }
      }

      template< template< typename... > class Traits >
      basic_value< Traits > patch( basic_value< Traits > value, const basic_value< Traits >& patch )
      {
         patch_inplace( value, patch );
         return std::move( value );
      }

      template< template< typename... > class Traits >
      basic_value< Traits > patch( basic_value< Traits > value, basic_value< Traits >&& patch )
      {
         patch_inplace( value, std::move( patch ) );
         return std::move( value );
      }

   }  // namespace json

}  // namespace tao

#endif
