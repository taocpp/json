// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_DIFF_HPP
#define TAO_JSON_CONTRIB_DIFF_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "../pointer.hpp"
#include "../value.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename... Ts >
         void move_append( std::vector< Ts... >& lhs, std::vector< Ts... >& rhs )
         {
            if( lhs.empty() ) {
               lhs = std::move( rhs );
            }
            else if( !rhs.empty() ) {
               lhs.reserve( lhs.size() + rhs.size() );
               std::move( std::begin( rhs ), std::end( rhs ), std::back_inserter( lhs ) );
            }
         }

      }  // namespace internal

      template< template< typename... > class Traits >
      basic_value< Traits > diff( const basic_value< Traits >& source, const basic_value< Traits >& destination, const pointer& path = pointer() )
      {
         basic_value< Traits > result = empty_array;
         if( source == destination ) {
            return result;
         }
         if( source.type() != destination.type() ) {
            result.push_back( { { "op", "replace" },
                                { "path", to_string( path ) },
                                { "value", destination } } );
         }
         else {
            switch( source.type() ) {
               case type::ARRAY: {
                  const auto ss = source.unsafe_get_array().size();
                  const auto ds = destination.unsafe_get_array().size();
                  std::size_t i = 0;
                  while( ( i < ss ) && ( i < ds ) ) {
                     internal::move_append( result.unsafe_get_array(), diff( source.unsafe_at( i ), destination.unsafe_at( i ), path + i ).unsafe_get_array() );
                     ++i;
                  }
                  const auto s = ss + i - 1;
                  while( i < ss ) {
                     result.push_back( { { "op", "remove" },
                                         { "path", to_string( path + ( s - i ) ) } } );
                     ++i;
                  }
                  while( i < ds ) {
                     result.push_back( { { "op", "add" },
                                         { "path", to_string( path + i ) },
                                         { "value", destination.unsafe_at( i ) } } );
                     ++i;
                  }
               } break;

               case type::OBJECT: {
                  const auto& sm = source.unsafe_get_object();
                  const auto& dm = destination.unsafe_get_object();
                  auto sit = sm.begin();
                  auto dit = dm.begin();
                  while( ( sit != sm.end() ) || ( dit != dm.end() ) ) {
                     if( ( sit == sm.end() ) || ( ( dit != dm.end() ) && ( sit->first > dit->first ) ) ) {
                        result.push_back( { { "op", "add" },
                                            { "path", to_string( path + dit->first ) },
                                            { "value", dit->second } } );
                        ++dit;
                     }
                     else if( ( dit == dm.end() ) || ( ( sit != sm.end() ) && ( sit->first < dit->first ) ) ) {
                        result.push_back( { { "op", "remove" },
                                            { "path", to_string( path + sit->first ) } } );
                        ++sit;
                     }
                     else {
                        internal::move_append( result.unsafe_get_array(), diff( sit->second, dit->second, path + sit->first ).unsafe_get_array() );
                        ++sit;
                        ++dit;
                     }
                  }
               } break;

               default:
                  result.push_back( { { "op", "replace" },
                                      { "path", to_string( path ) },
                                      { "value", destination } } );
            }
         }
         return result;
      }

   }  // namespace json

}  // namespace tao

#endif
