// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_SELF_CONTAINED_HPP
#define TAO_JSON_SELF_CONTAINED_HPP

#include "events/to_value.hpp"
#include "events/virtual_ref.hpp"

#include "value.hpp"

namespace tao
{
   namespace json
   {
      // recursively checks for the existence
      // of RAW_PTR or OPAQUE_PTR nodes, STRING_VIEW or BINARY_VIEW,
      // returns true is no such nodes were found.

      template< template< typename... > class Traits, typename Base >
      bool is_self_contained( const basic_value< Traits, Base >& v ) noexcept
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

            case type::OPAQUE_PTR:
               return false;
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

      // removes all RAW_PTR and OPAQIE nodes, recursively,
      // by copying/generating their content,
      // replaces STRING_VIEW and BINARY_VIEW with copies.

      template< template< typename... > class Traits, typename Base >
      void make_self_contained( basic_value< Traits, Base >& v )
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
               v = *v.unsafe_get_raw_ptr();
               make_self_contained( v );
               return;

            case type::OPAQUE_PTR: {
               const auto& q = v.unsafe_get_opaque_ptr();
               events::to_basic_value< Traits, Base > consumer;
               events::virtual_ref< events::to_basic_value< Traits, Base > > ref( consumer );
               q.producer( ref, q.data );
               consumer.value.base() = v.base();  // TODO: Enable move -- or not retain base here?
               v = std::move( consumer.value );
               return;
            }
         }
         throw std::logic_error( "invalid value for tao::json::type" );  // NOLINT, LCOV_EXCL_LINE
      }

   }  // namespace json

}  // namespace tao

#endif
