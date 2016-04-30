// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_FLATTEN_HH
#define TAOCPP_JSON_INCLUDE_FLATTEN_HH

#include "value.hh"

namespace tao
{
   namespace json
   {
      // removes all POINTER nodes, recursively, by copying their pointed-to content
      template< template< typename ... > class Traits >
      void flatten( basic_value< Traits > & v )
      {
         switch ( v.type() ) {
            case json::type::NULL_:
            case json::type::BOOL_:
            case json::type::SIGNED_INTEGER:
            case json::type::UNSIGNED_INTEGER:
            case json::type::DOUBLE:
            case json::type::STRING:
               return;
            case json::type::ARRAY:
               for ( auto & e : v.unsafe_get_array() ) {
                  flatten( e );
               }
               return;
            case json::type::OBJECT:
               for ( auto & e : v.unsafe_get_object() ) {
                  flatten( e.second );
               }
               return;
            case json::type::POINTER:
               if( const auto * p = v.unsafe_get_pointer() ) {
                  v = * p;
                  flatten( v );
               }
               else {
                  v = null;
               }
               return;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

  } // json

} // tao

#endif
