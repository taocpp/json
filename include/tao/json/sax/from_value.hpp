// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_FROM_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_SAX_FROM_VALUE_HPP

#include <stdexcept>

#include "../value.hpp"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX producer to generate events from a JSON value
         template< template< typename... > class Traits, typename Consumer >
         void from_value( const basic_value< Traits >& v, Consumer& consumer )
         {
            switch( v.type() ) {
               case type::UNINITIALIZED:
                  throw std::logic_error( "unable to produce events from uninitialized values" );
               case type::DISCARDED:
                  throw std::logic_error( "unable to produce events from discarded values" );
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
               case type::ARRAY:
                  consumer.begin_array();
                  for( const auto& e : v.unsafe_get_array() ) {
                     sax::from_value( e, consumer );
                     consumer.element();
                  }
                  consumer.end_array();
                  return;
               case type::OBJECT:
                  consumer.begin_object();
                  for( const auto& e : v.unsafe_get_object() ) {
                     consumer.key( e.first );
                     sax::from_value( e.second, consumer );
                     consumer.member();
                  }
                  consumer.end_object();
                  return;
               case type::RAW_PTR:
                  if( const basic_value< Traits >* p = v.unsafe_get_raw_ptr() ) {
                     sax::from_value( *p, consumer );
                  }
                  else {
                     consumer.null();
                  }
                  return;
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
         }

         // SAX producer to generate events from an rvalue JSON value
         // note: strings from the source might be moved in the consumer
         template< template< typename... > class Traits, typename Consumer >
         void from_value( basic_value< Traits >&& v, Consumer& consumer )
         {
            switch( v.type() ) {
               case type::UNINITIALIZED:
                  throw std::logic_error( "unable to produce events from uninitialized values" );
               case type::DISCARDED:
                  throw std::logic_error( "unable to produce events from discarded values" );
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
               case type::ARRAY:
                  consumer.begin_array();
                  for( auto&& e : v.unsafe_get_array() ) {
                     sax::from_value( std::move( e ), consumer );
                     consumer.element();
                  }
                  consumer.end_array();
                  return;
               case type::OBJECT:
                  consumer.begin_object();
                  for( auto&& e : v.unsafe_get_object() ) {
                     consumer.key( std::move( e.first ) );
                     sax::from_value( std::move( e.second ), consumer );
                     consumer.member();
                  }
                  consumer.end_object();
                  return;
               case type::RAW_PTR:
                  if( const basic_value< Traits >* p = v.unsafe_get_raw_ptr() ) {
                     sax::from_value( *p, consumer );
                  }
                  else {
                     consumer.null();
                  }
                  return;
            }
            throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
         }

      }  // sax

   }  // json

}  // tao

#endif
