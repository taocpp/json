// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_PATCH_HPP
#define TAO_JSON_PATCH_HPP

#include <stdexcept>
#include <utility>

#include "pointer.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits, typename Base >
      void patch_inplace( basic_value< Traits, Base >& v, const basic_value< Traits, Base >& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( v.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );  // NOLINT
               }
            }
            else if( op == "remove" ) {
               v.erase( path_pointer );
            }
            else if( op == "add" ) {
               v.insert( path_pointer, entry.at( "value" ) );
            }
            else if( op == "replace" ) {
               v.at( path_pointer ) = entry.at( "value" );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto t = std::move( v.at( from ) );
               v.erase( from );
               v.insert( path_pointer, std::move( t ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               v.insert( path_pointer, v.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );  // NOLINT
            }
         }
      }

      template< template< typename... > class Traits, typename Base >
      void patch_inplace( basic_value< Traits, Base >& v, basic_value< Traits, Base >&& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( v.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );  // NOLINT
               }
            }
            else if( op == "remove" ) {
               v.erase( path_pointer );
            }
            else if( op == "add" ) {
               v.insert( path_pointer, std::move( entry.at( "value" ) ) );
            }
            else if( op == "replace" ) {
               v.at( path_pointer ) = std::move( entry.at( "value" ) );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto t = std::move( v.at( from ) );
               v.erase( from );
               v.insert( path_pointer, std::move( t ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               v.insert( path_pointer, v.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );  // NOLINT
            }
         }
      }

      template< template< typename... > class Traits, typename Base >
      basic_value< Traits, Base > patch( basic_value< Traits, Base > v, const basic_value< Traits, Base >& patch )
      {
         patch_inplace( v, patch );
         return v;
      }

      template< template< typename... > class Traits, typename Base >
      basic_value< Traits, Base > patch( basic_value< Traits, Base > v, basic_value< Traits, Base >&& patch )
      {
         patch_inplace( v, std::move( patch ) );
         return v;
      }

   }  // namespace json

}  // namespace tao

#endif
