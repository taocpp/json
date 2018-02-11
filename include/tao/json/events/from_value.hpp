// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_FROM_VALUE_HPP
#define TAO_JSON_EVENTS_FROM_VALUE_HPP

#include <stdexcept>

#include "../basic_value.hpp"

#include "virtual_ref.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events producer to generate events from a JSON Value.

         template< typename Consumer, template< typename... > class Traits, typename Base >
         void from_value( Consumer& consumer, const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::UNINITIALIZED:
                  throw std::logic_error( "unable to produce events from uninitialized values" );  // NOLINT

               case type::DISCARDED:
                  throw std::logic_error( "unable to produce events from discarded values" );  // NOLINT

               case type::DESTROYED:
                  throw std::logic_error( "unable to produce events from destroyed values" );  // NOLINT

               case type::NULL_:
                  consumer.null();
                  return;

               case type::BOOLEAN:
                  consumer.boolean( v.unsafe_get_boolean() );
                  return;

               case type::SIGNED:
                  consumer.number( v.unsafe_get_signed() );
                  return;

               case type::UNSIGNED:
                  consumer.number( v.unsafe_get_unsigned() );
                  return;

               case type::DOUBLE:
                  consumer.number( v.unsafe_get_double() );
                  return;

               case type::STRING:
                  consumer.string( v.unsafe_get_string() );
                  return;

               case type::STRING_VIEW:
                  consumer.string( v.unsafe_get_string_view() );
                  return;

               case type::BINARY:
                  consumer.binary( v.unsafe_get_binary() );
                  return;

               case type::BINARY_VIEW:
                  consumer.binary( v.unsafe_get_binary_view() );
                  return;

               case type::ARRAY: {
                  const auto& a = v.unsafe_get_array();
                  const auto s = a.size();
                  consumer.begin_array( s );
                  for( const auto& e : a ) {
                     events::from_value( consumer, e );
                     consumer.element();
                  }
                  consumer.end_array( s );
                  return;
               }

               case type::OBJECT: {
                  const auto& o = v.unsafe_get_object();
                  const auto s = o.size();
                  consumer.begin_object( s );
                  for( const auto& e : o ) {
                     consumer.key( e.first );
                     events::from_value( consumer, e.second );
                     consumer.member();
                  }
                  consumer.end_object( s );
                  return;
               }

               case type::RAW_PTR:
                  events::from_value( consumer, *v.unsafe_get_raw_ptr() );
                  return;

               case type::OPAQUE_PTR: {
                  const auto& q = v.unsafe_get_opaque_ptr();
                  virtual_ref< Consumer > ref( consumer );
                  q.producer( ref, q.data );
                  return;
               }
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // NOLINT, LCOV_EXCL_LINE
         }

         // Events producer to generate events from an rvalue JSON value.
         // Note: Strings from the source might be moved to the consumer.

         template< typename Consumer, template< typename... > class Traits, typename Base >
         void from_value( Consumer& consumer, basic_value< Traits, Base >&& v )
         {
            switch( v.type() ) {
               case type::UNINITIALIZED:
                  throw std::logic_error( "unable to produce events from uninitialized values" );  // NOLINT

               case type::DISCARDED:
                  throw std::logic_error( "unable to produce events from discarded values" );  // NOLINT

               case type::DESTROYED:
                  throw std::logic_error( "unable to produce events from destroyed values" );  // NOLINT

               case type::NULL_:
                  consumer.null();
                  return;

               case type::BOOLEAN:
                  consumer.boolean( v.unsafe_get_boolean() );
                  return;

               case type::SIGNED:
                  consumer.number( v.unsafe_get_signed() );
                  return;

               case type::UNSIGNED:
                  consumer.number( v.unsafe_get_unsigned() );
                  return;

               case type::DOUBLE:
                  consumer.number( v.unsafe_get_double() );
                  return;

               case type::STRING:
                  consumer.string( std::move( v.unsafe_get_string() ) );
                  return;

               case type::STRING_VIEW:
                  consumer.string( v.unsafe_get_string_view() );
                  return;

               case type::BINARY:
                  consumer.binary( std::move( v.unsafe_get_binary() ) );
                  return;

               case type::BINARY_VIEW:
                  consumer.binary( v.unsafe_get_binary_view() );
                  return;

               case type::ARRAY:
                  consumer.begin_array( v.unsafe_get_array().size() );
                  for( auto&& e : v.unsafe_get_array() ) {
                     events::from_value( consumer, std::move( e ) );
                     consumer.element();
                  }
                  consumer.end_array( v.unsafe_get_array().size() );
                  return;

               case type::OBJECT:
                  consumer.begin_object( v.unsafe_get_object().size() );
                  for( auto&& e : v.unsafe_get_object() ) {
                     consumer.key( e.first );
                     events::from_value( consumer, std::move( e.second ) );
                     consumer.member();
                  }
                  consumer.end_object( v.unsafe_get_object().size() );
                  return;

               case type::RAW_PTR:
                  events::from_value( consumer, *v.unsafe_get_raw_ptr() );
                  return;

               case type::OPAQUE_PTR: {
                  const auto& q = v.unsafe_get_opaque_ptr();
                  virtual_ref< Consumer > ref( consumer );
                  q.producer( ref, q.data );
                  return;
               }
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // NOLINT, LCOV_EXCL_LINE
         }

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
