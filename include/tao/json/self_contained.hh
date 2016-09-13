// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SELF_CONTAINED_HH
#define TAOCPP_JSON_INCLUDE_SELF_CONTAINED_HH

#include "value.hh"

namespace tao
{
   namespace json
   {
      // recursively checks for the existence if POINTER nodes,
      // returns true is no POINTER nodes were found.
      template< template< typename ... > class Traits >
      bool is_self_contained( basic_value< Traits > & v ) noexcept
      {
         switch ( v.type() ) {
            case json::type::NULL_:
            case json::type::BOOL:
            case json::type::SIGNED:
            case json::type::UNSIGNED:
            case json::type::DOUBLE:
            case json::type::STRING:
               return true;
            case json::type::ARRAY:
               for ( auto & e : v.unsafe_get_array() ) {
                  if ( ! is_self_contained( e ) ) {
                     return false;
                  }
               }
               return true;
            case json::type::OBJECT:
               for ( auto & e : v.unsafe_get_object() ) {
                  if ( ! is_self_contained( e.second ) ) {
                     return false;
                  }
               }
               return true;
            case json::type::POINTER:
               return false;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      // removes all POINTER nodes, recursively, by copying their pointed-to content
      // or replacing a nullptr POINTER node with a null node.
      template< template< typename ... > class Traits >
      void make_self_contained( basic_value< Traits > & v )
      {
         switch ( v.type() ) {
            case json::type::NULL_:
            case json::type::BOOL:
            case json::type::SIGNED:
            case json::type::UNSIGNED:
            case json::type::DOUBLE:
            case json::type::STRING:
               return;
            case json::type::ARRAY:
               for ( auto & e : v.unsafe_get_array() ) {
                  make_self_contained( e );
               }
               return;
            case json::type::OBJECT:
               for ( auto & e : v.unsafe_get_object() ) {
                  make_self_contained( e.second );
               }
               return;
            case json::type::POINTER:
               if ( const auto * p = v.unsafe_get_pointer() ) {
                  v = * p;
                  make_self_contained( v );
               }
               else {
                  v = null;
               }
               return;
         }
         throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
      }

  } // json

} // tao

#endif
