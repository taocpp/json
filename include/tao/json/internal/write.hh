// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_WRITE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_WRITE_HH

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits, typename Handler >
         void write( const basic_value< Traits > & v, Handler & handler )
         {
            switch( v.type() ) {
               case type::NULL_:
                  handler.null();
                  break;
               case type::BOOL:
                  if ( v.unsafe_get_bool() ) {
                     handler.true_();
                  }
                  else {
                     handler.false_();
                  }
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
                  {
                     bool first = true;
                     for( const auto & e : v.unsafe_get_array() ) {
                        if( first ) {
                           first = false;
                        }
                        else {
                           handler.element_separator();
                        }
                        internal::write( e, handler );
                        handler.commit_element();
                     }
                  }
                  handler.end_array();
                  break;
               case type::OBJECT:
                  handler.begin_object();
                  {
                     bool first = true;
                     for( const auto & e : v.unsafe_get_object() ) {
                        if( first ) {
                           first = false;
                        }
                        else {
                           handler.value_separator();
                        }
                        handler.commit_key( e.first );
                        handler.name_separator();
                        internal::write( e.second, handler );
                        handler.commit_member();
                     }
                  }
                  handler.end_object();
                  break;
               case type::POINTER:
                  if ( const basic_value< Traits > * p = v.unsafe_get_pointer() ) {
                     internal::write( * p, handler );
                  }
                  else {
                     handler.null();
                  }
                  break;
               default:
                  assert( false );
            }
         }

      } // internal

   } // json

} // tao

#endif
