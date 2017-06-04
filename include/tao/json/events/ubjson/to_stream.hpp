// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON_UBJSON_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON_UBJSON_TO_STREAM_HPP

#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>

#include "../../byte.hpp"
#include "../../internal/temporary.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace ubjson
         {
            class to_stream
            {
            private:
               std::ostream& os;

            public:
               explicit to_stream( std::ostream& os ) noexcept
                  : os( os )
               {
               }

               void null()
               {
                  os.put( 'Z' );
               }

               void boolean( const bool v )
               {
                  os.put( v ? 'T' : 'F' );
               }

               void number( const std::int64_t v )
               {
                  if( ( v >= -128 ) && ( v <= 127 ) ) {
                     os.put( 'i' );
                     const std::int8_t x = v;
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( ( v >= -32768 ) && ( v <= 32767 ) ) {
                     os.put( 'I' );
                     const std::uint16_t x = internal::h_to_be( std::uint16_t( v ) );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( ( v >= -2147483648 ) && ( v <= 2147483647 ) ) {
                     os.put( 'l' );
                     const std::uint32_t x = internal::h_to_be( std::uint32_t( v ) );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else {
                     os.put( 'L' );
                     const std::uint64_t x = internal::h_to_be( std::uint64_t( v ) );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
               }

               void number( const std::uint64_t v )
               {
                  if( v <= 255 ) {
                     os.put( 'U' );
                     const std::uint8_t x = v;
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( v <= 32767 ) {
                     os.put( 'I' );
                     const std::uint16_t x = internal::h_to_be( std::uint16_t( v ) );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( v <= 2147483647ul ) {
                     os.put( 'l' );
                     const std::uint32_t x = internal::h_to_be( std::uint32_t( v ) );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( v <= 9223372036854775807ull ) {
                     os.put( 'L' );
                     const std::uint64_t x = internal::h_to_be( v );
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else {
                     os.put( 'H' );
                     os.put( 'U' );
                     os.put( char( 19 ) + char( v >= 10000000000000000000ull ) );
                     os << v;
                  }
               }

               void number( const double v )
               {
                  os.put( 'D' );
                  const auto x = internal::h_to_be( v );
                  os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
               }

               void string( const std::string& v )
               {
                  if( ( v.size() == 1 ) && ( ( v[ 0 ] & 0x80 ) == 0 ) ) {
                     os.put( 'C' );
                     os.put( v[ 0 ] );
                  }
                  else {
                     os.put( 'S' );
                     number( std::uint64_t( v.size() ) );
                     os.write( v.data(), v.size() );
                  }
               }

               void binary( const std::vector< byte >& v )
               {
                  // NOTE: UBJSON encodes binary data as 'strongly typed array of uint8 values'.
                  os.write( "[$U#", 4 );
                  number( std::uint64_t( v.size() ) );
                  os.write( reinterpret_cast< const char* >( v.data() ), v.size() );
               }

               void begin_array()
               {
                  os.put( '[' );
               }

               void begin_array( const std::size_t size )
               {
                  os.write( "[#", 2 );
                  number( std::uint64_t( size ) );
               }

               void element() noexcept
               {
               }

               void end_array()
               {
                  os.put( ']' );
               }

               void end_array( const std::size_t ) noexcept
               {
               }

               void begin_object()
               {
                  os.put( '{' );
               }

               void begin_object( const std::size_t size )
               {
                  os.write( "{#", 2 );
                  number( std::uint64_t( size ) );
               }

               void key( const std::string& v )
               {
                  number( std::uint64_t( v.size() ) );
                  os.write( v.data(), v.size() );
               }

               void member() noexcept
               {
               }

               void end_object()
               {
                  os.put( '}' );
               }

               void end_object( const std::size_t ) noexcept
               {
               }
            };

         }  // namespace ubjson

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
