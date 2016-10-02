// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_REFERENCE_HH
#define TAOCPP_JSON_INCLUDE_REFERENCE_HH

#include "value.hh"
#include "pointer.hh"
#include "internal/uri_fragment.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // currently, only URI fragments are supported
         template< template< typename ... > class Traits >
         void resolve_references( basic_value< Traits > & r, basic_value< Traits > & v )
         {
            switch ( v.type() ) {
               case json::type::NULL_:
               case json::type::BOOLEAN:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
               case json::type::STRING:
                  return;
               case json::type::ARRAY:
                  for ( auto & e : v.unsafe_get_array() ) {
                     resolve_references( r, e );
                  }
                  return;
               case json::type::OBJECT:
                  if ( const auto * ref = v.skip_raw_ptr()->find( "$ref" ) ) {
                     if ( ref->is_string() ) {
                        const std::string & s = ref->unsafe_get_string();
                        if ( ! s.empty() && s[ 0 ] == '#' ) {
                           v = r.at( internal::uri_fragment_to_pointer( s ) ).skip_raw_ptr();
                           resolve_references( r, v );
                           return;
                        }
                     }
                  }
                  for ( auto & e : v.unsafe_get_object() ) {
                     resolve_references( r, e.second );
                  }
                  return;
               case json::type::RAW_PTR:
                  return;
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
         }

      } // internal

      template< template< typename ... > class Traits >
      void resolve_references( basic_value< Traits > & r )
      {
         resolve_references( r, r );
      }

  } // json

} // tao

#endif
