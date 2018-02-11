// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_REFERENCE_HPP
#define TAO_JSON_REFERENCE_HPP

#include "internal/uri_fragment.hpp"

#include "pointer.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // JSON Reference, see draft ("work in progress") RFC at
         // https://tools.ietf.org/html/draft-pbryan-zyp-json-ref-03

         // NOTE: Currently, only URI fragments are supported.
         // Remote references are ignored, i.e., left untouched.

         // JSON References are replaced with a RAW_PTR,
         // which might lead to infinite loops if you try
         // to traverse the value. Make sure you understand
         // the consequences and handle the resulting value
         // accordingly!

         // Self-references will throw an exception, as well as
         // references into JSON Reference additional members
         // (which shall be ignored as per the specification).

         template< template< typename... > class Traits, typename Base >
         void resolve_references( basic_value< Traits, Base >& r, basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::UNINITIALIZED:
                  return;
               case type::DISCARDED:
                  throw std::logic_error( "attempt to use a discarded value" );  // NOLINT
               case type::DESTROYED:
                  throw std::logic_error( "attempt to use a destroyed value" );  // NOLINT
               case type::NULL_:
               case type::BOOLEAN:
               case type::SIGNED:
               case type::UNSIGNED:
               case type::DOUBLE:
               case type::STRING:
               case type::STRING_VIEW:
               case type::BINARY:
               case type::BINARY_VIEW:
                  return;
               case type::ARRAY:
                  for( auto& e : v.unsafe_get_array() ) {
                     resolve_references( r, e );
                  }
                  return;
               case type::OBJECT:
                  for( auto& e : v.unsafe_get_object() ) {
                     resolve_references( r, e.second );
                  }
                  if( const auto* ref = v.find( "$ref" ) ) {
                     ref = ref->skip_raw_ptr();
                     if( ref->is_string_type() ) {
                        const tao::string_view s = ref->unsafe_get_string_type();
                        if( !s.empty() && s[ 0 ] == '#' ) {
                           const pointer ptr = internal::uri_fragment_to_pointer( s );
                           const auto* p = &r;
                           auto it = ptr.begin();
                           while( it != ptr.end() ) {
                              switch( p->type() ) {
                                 case type::ARRAY:
                                    p = p->at( it->index() ).skip_raw_ptr();
                                    break;
                                 case type::OBJECT:
                                    if( const auto* t = p->find( "$ref" ) ) {
                                       if( t->is_string_type() ) {
                                          throw std::runtime_error( "invalid JSON Reference: referencing additional data members is invalid" );  // NOLINT
                                       }
                                    }
                                    p = p->at( it->key() ).skip_raw_ptr();
                                    break;
                                 default:
                                    throw invalid_type( ptr.begin(), std::next( it ) );  // NOLINT
                              }
                              ++it;
                           }
                           if( p == &v ) {
                              throw std::runtime_error( "JSON Reference: invalid self reference" );  // NOLINT
                           }
                           v.assign_raw_ptr( p );
                           resolve_references( r, v );
                        }
                        else {
                           // Ignore remote references for now...
                           // throw std::runtime_error( "JSON Reference: unsupported or invalid URI: " + s ); // NOLINT
                        }
                     }
                  }
                  return;
               case type::RAW_PTR:
                  return;
               case type::OPAQUE_PTR:
                  return;
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // NOLINT, LCOV_EXCL_LINE
         }

      }  // namespace internal

      template< template< typename... > class Traits, typename Base >
      void resolve_references( basic_value< Traits, Base >& r )
      {
         internal::resolve_references( r, r );
      }

   }  // namespace json

}  // namespace tao

#endif
