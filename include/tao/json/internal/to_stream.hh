// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH

#include <ostream>

#include "../external/double.hh"
#include "../value.hh"

#include "escape.hh"
#include "value_writer.hh"
#include "write.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const basic_value< Traits > & v )
         {
            internal::value_writer writer( o );
            internal::write( v, writer );
         }

         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const basic_value< Traits > & v, const unsigned indent, const unsigned current = 0 );

         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const std::vector< basic_value< Traits > > & v, const unsigned indent, unsigned current = 0 )
         {
            o << '[';
            if ( ! v.empty() ) {
               current += indent;
               const std::string padding = std::string( current, ' ' );
               o << '\n';
               o << padding;
               internal::to_stream( o, v[ 0 ], indent, current );
               for ( std::size_t i = 1; i < v.size(); ++i ) {
                  o << ",\n";
                  o << padding;
                  internal::to_stream( o, v[ i ], indent, current );
               }
               o << '\n';
               o.write( padding.data(), current - indent );
            }
            o << ']';
         }

         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const std::map< std::string, basic_value< Traits > > & v, const unsigned indent, unsigned current = 0 )
         {
            o << '{';
            if ( ! v.empty() ) {
               current += indent;
               const std::string padding = std::string( current, ' ' );
               o << '\n';
               o << padding;
               internal::escape( o, v.begin()->first );
               o << ": ";
               internal::to_stream( o, v.begin()->second, indent, current );
               for ( auto i = ++v.begin(); i != v.end(); ++i ) {
                  o << ",\n";
                  o << padding;
                  internal::escape( o, i->first );
                  o << ": ";
                  internal::to_stream( o, i->second, indent, current );
               }
               o << '\n';
               o.write( padding.data(), current - indent );
            }
            o << '}';
         }

         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const basic_value< Traits > & v, const unsigned indent, const unsigned current )
         {
            switch ( v.type() ) {
               case type::NULL_:
                  o.write( "null", 4 );
                  return;
               case type::BOOL:
                  if ( v.unsafe_get_bool() ) {
                     o.write( "true", 4 );
                  }
                  else {
                     o.write( "false", 5 );
                  }
                  return;
               case type::SIGNED:
                  o << v.unsafe_get_signed();
                  return;
               case type::UNSIGNED:
                  o << v.unsafe_get_unsigned();
                  return;
               case type::DOUBLE:
                  // Assumes std::isfinite( v.unsafe_get_double() ).
                  json_double_conversion::Dtostr( o, v.unsafe_get_double() );
                  return;
               case type::STRING:
                  internal::escape( o, v.unsafe_get_string() );
                  return;
               case type::ARRAY:
                  internal::to_stream( o, v.unsafe_get_array(), indent, current );
                  return;
               case type::OBJECT:
                  internal::to_stream( o, v.unsafe_get_object(), indent, current );
                  return;
               case type::POINTER:
                  if ( const basic_value< Traits > * p = v.unsafe_get_pointer() ) {
                     internal::to_stream( o, * p, indent, current );
                  }
                  else {
                     o.write( "null", 4 );
                  }
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

      } // internal

   } // json

} // tao

#endif
