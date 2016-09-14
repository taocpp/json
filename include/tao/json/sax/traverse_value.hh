// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_TRAVERSE_VALUE_HH
#define TAOCPP_JSON_INCLUDE_SAX_TRAVERSE_VALUE_HH

#include <stdexcept>

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         template< template< typename ... > class Traits, typename Handler >
         void traverse_value( const basic_value< Traits > & v, Handler & handler )
         {
            switch( v.type() ) {
               case type::NULL_:
                  handler.null();
                  break;
               case type::BOOL:
                  handler.boolean( v.unsafe_get_bool() );
                  break;
               case type::SIGNED:
                  handler.number( v.unsafe_get_signed() );
                  break;
               case type::UNSIGNED:
                  handler.number( v.unsafe_get_unsigned() );
                  break;
               case type::DOUBLE:
                  handler.number( v.unsafe_get_double() );
                  break;
               case type::STRING:
                  handler.string( v.unsafe_get_string() );
                  break;
               case type::ARRAY:
                  handler.begin_array();
                  for( const auto & e : v.unsafe_get_array() ) {
                     sax::traverse_value( e, handler );
                     handler.element();
                  }
                  handler.end_array();
                  break;
               case type::OBJECT:
                  handler.begin_object();
                  for( const auto & e : v.unsafe_get_object() ) {
                     handler.key( e.first );
                     sax::traverse_value( e.second, handler );
                     handler.value();
                  }
                  handler.end_object();
                  break;
               case type::POINTER:
                  if ( const basic_value< Traits > * p = v.unsafe_get_pointer() ) {
                     sax::traverse_value( * p, handler );
                  }
                  else {
                     handler.null();
                  }
                  break;
               default:
                  throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
            }
         }

         template< template< typename ... > class Traits, typename Handler >
         void traverse_value( basic_value< Traits > && v, Handler & handler )
         {
            switch( v.type() ) {
               case type::NULL_:
                  handler.null();
                  break;
               case type::BOOL:
                  handler.boolean( v.unsafe_get_bool() );
                  break;
               case type::SIGNED:
                  handler.number( v.unsafe_get_signed() );
                  break;
               case type::UNSIGNED:
                  handler.number( v.unsafe_get_unsigned() );
                  break;
               case type::DOUBLE:
                  handler.number( v.unsafe_get_double() );
                  break;
               case type::STRING:
                  handler.string( std::move( v.unsafe_get_string() ) );
                  break;
               case type::ARRAY:
                  handler.begin_array();
                  for( const auto & e : v.unsafe_get_array() ) {
                     sax::traverse_value( e, handler );
                     handler.element();
                  }
                  handler.end_array();
                  break;
               case type::OBJECT:
                  handler.begin_object();
                  for( const auto & e : v.unsafe_get_object() ) {
                     handler.key( e.first );
                     sax::traverse_value( e.second, handler );
                     handler.value();
                  }
                  handler.end_object();
                  break;
               case type::POINTER:
                  if ( const basic_value< Traits > * p = v.unsafe_get_pointer() ) {
                     sax::traverse_value( * p, handler );
                  }
                  else {
                     handler.null();
                  }
                  break;
               default:
                  throw std::logic_error( "invalid value for tao::json::type" );  // LCOV_EXCL_LINE
            }
         }

      } // sax

   } // json

} // tao

#endif
