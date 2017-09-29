// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SELF_CONTAINED_HPP
#define TAOCPP_JSON_INCLUDE_SELF_CONTAINED_HPP

#include "data.hpp"

namespace tao
{
   namespace json
   {
      // recursively checks for the existence if RAW_PTR nodes,
      // returns true is no RAW_PTR nodes were found.
      inline bool is_self_contained( const data& v ) noexcept
      {
         switch( v.type() ) {
            case type::UNINITIALIZED:
               return true;

            // LCOV_EXCL_START
            case type::DISCARDED:
               assert( v.type() != type::DISCARDED );
               return false;

            case type::DESTROYED:
               assert( v.type() != type::DESTROYED );
               return false;
            // LCOV_EXCL_STOP

            case type::NULL_:
            case type::BOOLEAN:
            case type::SIGNED:
            case type::UNSIGNED:
            case type::DOUBLE:
            case type::STRING:
            case type::BINARY:
               return true;

            case type::STRING_VIEW:
            case type::BINARY_VIEW:
               return false;

            case type::ARRAY:
               for( auto& e : v.unsafe_get_array() ) {
                  if( !is_self_contained( e ) ) {
                     return false;
                  }
               }
               return true;

            case type::OBJECT:
               for( auto& e : v.unsafe_get_object() ) {
                  if( !is_self_contained( e.second ) ) {
                     return false;
                  }
               }
               return true;

            case type::RAW_PTR:
               return false;
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

      // removes all RAW_PTR nodes, recursively, by copying their pointed-to content
      // or replacing a nullptr RAW_PTR node with a null node.
      // replaces STRING_VIEW and BINARY_VIEW with copies.
      inline void make_self_contained( data& v )
      {
         switch( v.type() ) {
            case type::UNINITIALIZED:
               return;

            case type::DISCARDED:
               throw std::logic_error( "attempt to use a discarded value" );

            case type::DESTROYED:
               throw std::logic_error( "attempt to use a destroyed value" );

            case type::NULL_:
            case type::BOOLEAN:
            case type::SIGNED:
            case type::UNSIGNED:
            case type::DOUBLE:
            case type::STRING:
            case type::BINARY:
               return;

            case type::STRING_VIEW: {
               const auto sv = v.unsafe_get_string_view();
               v.unsafe_emplace_string( sv.data(), sv.size() );
               return;
            }

            case type::BINARY_VIEW: {
               const auto xv = v.unsafe_get_binary_view();
               v.unsafe_emplace_binary( xv.begin(), xv.end() );
               return;
            }

            case type::ARRAY:
               for( auto& e : v.unsafe_get_array() ) {
                  make_self_contained( e );
               }
               return;

            case type::OBJECT:
               for( auto& e : v.unsafe_get_object() ) {
                  make_self_contained( e.second );
               }
               return;

            case type::RAW_PTR:
               if( const auto* p = v.unsafe_get_raw_ptr() ) {
                  v = *p;
                  make_self_contained( v );
               }
               else {
                  v.unsafe_assign_null();
               }
               return;
         }
         throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
      }

   }  // namespace json

}  // namespace tao

#endif
