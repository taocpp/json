// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_STREAM_HH
#define TAOCPP_JSON_INCLUDE_STREAM_HH

#include <cmath>
#include <sstream>
#include <iomanip>

#include "value.hh"

#include "internal/double.hh"

namespace tao
{
   namespace json
   {
      inline std::string escape( const std::string & data )
      {
         std::string r = "\"";

         r.reserve( data.size() + 7 );

         static const char * h = "0123456789abcdef";

         const unsigned char * d = reinterpret_cast< const unsigned char * >( data.data() );

         for ( size_t i = 0; i < data.size(); ++i ) {
            switch ( const auto c = d[ i ] ) {
               case '\b':
                  r += "\\b";
                  break;
               case '\f':
                  r += "\\f";
                  break;
               case '\n':
                  r += "\\n";
                  break;
               case '\r':
                  r += "\\r";
                  break;
               case '\t':
                  r += "\\t";
                  break;
               case '\\':
                  r += "\\\\";
                  break;
               case '\"':
                  r += "\\\"";
                  break;
               default:
                  if ( ( c < 32 ) || ( c == 127 ) ) {
                     r += "\\u00";
                     r += h[ ( c & 0xf0 ) >> 4 ];
                     r += h[   c & 0x0f        ];
                     continue;
                  }
                  r += c;  // Assume valid UTF-8.
                  break;
            }
         }
         r += '"';
         return r;
      }

      inline void to_stream( std::ostream & o, const std::string & s )
      {
         o << escape( s );  // TODO: Check whether direct-to-stream is faster.
      }

      template< typename Value >
      void to_stream( std::ostream & o, const Value & v );

      template< typename Value >
      void to_stream( std::ostream & o, const Value & v, const unsigned indent, const unsigned current = 0 );

      template< typename Value >
      void to_stream( std::ostream & o, const std::vector< Value > & v )
      {
         o << '[';
         if ( ! v.empty() ) {
            to_stream( o, v[ 0 ] );
            for ( size_t i = 1; i < v.size(); ++i ) {
               o << ',';
               to_stream( o, v[ i ] );
            }
         }
         o << ']';
      }

      template< typename Value >
      void to_stream( std::ostream & o, const std::vector< Value > & v, const unsigned indent, unsigned current = 0 )
      {
         o << '[';
         current += indent;
         const std::string padding = std::string( current, ' ' );
         if ( ! v.empty() ) {
            o << '\n';
            o << padding;
            to_stream( o, v[ 0 ], indent, current );
            for ( size_t i = 1; i < v.size(); ++i ) {
               o << ",\n";
               o << padding;
               to_stream( o, v[ i ], indent, current );
            }
            o << '\n';
            o.write( padding.data(), current - indent );
         }
         o << ']';
      }

      template< typename Value >
      void to_stream( std::ostream & o, const std::map< std::string, Value > & v )
      {
         o << '{';
         if ( ! v.empty() ) {
            to_stream( o, v.begin()->first );
            o << ':';
            to_stream( o, v.begin()->second );
            for ( auto i = ++v.begin(); i != v.end(); ++i ) {
               o << ',';
               to_stream( o, i->first );
               o << ':';
               to_stream( o, i->second );
            }
         }
         o << '}';
      }

      template< typename Value >
      void to_stream( std::ostream & o, const std::map< std::string, Value > & v, const unsigned indent, unsigned current = 0 )
      {
         o << '{';
         current += indent;
         const std::string padding = std::string( current, ' ' );
         if ( ! v.empty() ) {
            o << '\n';
            o << padding;
            to_stream( o, v.begin()->first );
            o << ": ";
            to_stream( o, v.begin()->second, indent, current );
            for ( auto i = ++v.begin(); i != v.end(); ++i ) {
               o << ",\n";
               o << padding;
               to_stream( o, i->first );
               o << ": ";
               to_stream( o, i->second, indent, current );
            }
            o << '\n';
            o.write( padding.data(), current - indent );
         }
         o << '}';
      }

      template< typename Value >
      void to_stream( std::ostream & o, const Value & v )
      {
         switch ( v.type() ) {
            case type::NULL_:
               o << "null";
               return;
            case type::BOOL_:
               o << ( v.unsafe_bool() ? "true" : "false" );
               return;
            case type::INT64:
               o << v.unsafe_int64();
               return;
            case type::DOUBLE:
               if ( std::isfinite( v.unsafe_double() ) ) {
                  json_double_conversion::Dtostr( o, v.unsafe_double() );
               }
               else {
                  o << "null";  // JSON forbids values like NaN and infinity.
               }
               return;
            case type::STRING:
               to_stream( o, v.unsafe_string() );
               return;
            case type::ARRAY:
               to_stream( o, v.unsafe_array() );
               return;
            case type::OBJECT:
               to_stream( o, v.unsafe_object() );
               return;
         }
         assert( false );
      }

      template< typename Value >
      void to_stream( std::ostream & o, const Value & v, const unsigned indent, const unsigned current )
      {
         switch ( v.type() ) {
            case type::NULL_:
               o << "null";
               return;
            case type::BOOL_:
               o << ( v.unsafe_bool() ? "true" : "false" );
               return;
            case type::INT64:
               o << v.unsafe_int64();
               return;
            case type::DOUBLE:
               if ( std::isfinite( v.unsafe_double() ) ) {
                  json_double_conversion::Dtostr( o, v.unsafe_double() );
               }
               else {
                  o << "null";  // JSON forbids values like NaN and infinity.
               }
               return;
            case type::STRING:
               to_stream( o, v.unsafe_string() );
               return;
            case type::ARRAY:
               to_stream( o, v.unsafe_array(), indent, current );
               return;
            case type::OBJECT:
               to_stream( o, v.unsafe_object(), indent, current );
               return;
         }
         assert( false );
      }

      inline std::string to_string( const value & v )
      {
         std::ostringstream o;
         to_stream( o, v );
         return o.str();
      }

      inline std::string to_string( const value & v, const unsigned indent, const unsigned current = 0 )
      {
         std::ostringstream o;
         to_stream( o, v, indent, current );
         return o.str();
      }

      // Use ostream << std::setw( n ) for pretty-printing with indent n.

      inline std::ostream & operator<< ( std::ostream & o, const value & v )
      {
         const auto w = o.width();
         o.width( 0 );

         if ( w > 0 ) {
            to_stream( o, v, w );
         }
         else {
            to_stream( o, v );
         }
         return o;
      }

   } // json

} // tao

#endif
