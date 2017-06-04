// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON_MSGPACK_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON_MSGPACK_TO_STREAM_HPP

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
         namespace msgpack
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
                  os.put( char( 0xc0 ) );
               }

               void boolean( const bool v )
               {
                  os.put( char( 0xc2 ) + char( v ) );
               }

               template< typename Integer >
               void number_impl( const unsigned char tag, const std::uint64_t v )
               {
                  os.put( char( tag ) );
                  const Integer x = internal::h_to_be( Integer( v ) );
                  os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
               }

               void number( const std::int64_t v )
               {
                  if( ( v >= -32 ) && ( v <= -1 ) ) {
                     const std::int8_t x = v;
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( ( v >= -128 ) && ( v <= 127 ) ) {
                     number_impl< std::uint8_t >( 0xd0, v );
                  }
                  else if( ( v >= -32768 ) && ( v <= 32767 ) ) {
                     number_impl< std::uint16_t >( 0xd1, v );
                  }
                  else if( ( v >= -2147483648 ) && ( v <= 2147483647 ) ) {
                     number_impl< std::uint32_t >( 0xd2, v );
                  }
                  else {
                     number_impl< std::uint64_t >( 0xd3, v );
                  }
               }

               void number( const std::uint64_t v )
               {
                  if( v <= 127 ) {
                     const std::uint8_t x = v;
                     os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
                  }
                  else if( v <= 255 ) {
                     number_impl< std::uint8_t >( 0xcc, v );
                  }
                  else if( v <= 65535 ) {
                     number_impl< std::uint16_t >( 0xcd, v );
                  }
                  else if( v <= 4294967295ul ) {
                     number_impl< std::uint32_t >( 0xce, v );
                  }
                  else {
                     number_impl< std::uint64_t >( 0xcf, v );
                  }
               }

               void number( const double v )
               {
                  os.put( char( 0xcb ) );
                  const auto x = internal::h_to_be( v );
                  os.write( reinterpret_cast< const char* >( &x ), sizeof( x ) );
               }

               void string( const std::string& v )
               {
                  if( v.size() <= 31 ) {
                     os.put( char( v.size() + 0xa0 ) );
                  }
                  else if( v.size() <= 255 ) {
                     number_impl< std::uint8_t >( 0xd9, v.size() );
                  }
                  else if( v.size() <= 65535 ) {
                     number_impl< std::uint16_t >( 0xda, v.size() );
                  }
                  else if( v.size() <= 4294967295ul ) {
                     number_impl< std::uint32_t >( 0xdb, v.size() );
                  }
                  else {
                     throw std::runtime_error( "string too long for msgpack" );
                  }
                  os.write( v.data(), v.size() );
               }

               void binary( const std::vector< byte >& v )
               {
                  if( v.size() <= 255 ) {
                     number_impl< std::uint8_t >( 0xc4, v.size() );
                  }
                  else if( v.size() <= 65535 ) {
                     number_impl< std::uint16_t >( 0xc5, v.size() );
                  }
                  else if( v.size() <= 4294967295ul ) {
                     number_impl< std::uint32_t >( 0xc6, v.size() );
                  }
                  else {
                     throw std::runtime_error( "binary too long for msgpack" );
                  }
                  os.write( reinterpret_cast< const char* >( v.data() ), v.size() );
               }

               void begin_array()
               {
                  throw std::runtime_error( "msgpack requires array size" );  // TODO: Fix this by counting and buffering?
               }

               void begin_array( const std::size_t size )
               {
                  if( size <= 15 ) {
                     os.put( char( 0x90 + size ) );
                  }
                  else if( size <= 65535 ) {
                     number_impl< std::uint16_t >( 0xdc, size );
                  }
                  else if( size <= 4294967295ul ) {
                     number_impl< std::uint32_t >( 0xdd, size );
                  }
                  else {
                     throw std::runtime_error( "array too large for msgpack" );
                  }
               }

               void element() noexcept
               {
               }

               void end_array()
               {
                  assert( false );
               }

               void end_array( const std::size_t ) noexcept
               {
               }

               void begin_object()
               {
                  throw std::runtime_error( "msgpack requires object size" );  // TODO: Fix this by counting and buffering?
               }

               void begin_object( const std::size_t size )
               {
                  if( size <= 15 ) {
                     os.put( char( 0x80 + size ) );
                  }
                  else if( size <= 65535 ) {
                     number_impl< std::uint16_t >( 0xde, size );
                  }
                  else if( size <= 4294967295ul ) {
                     number_impl< std::uint32_t >( 0xdf, size );
                  }
                  else {
                     throw std::runtime_error( "array too large for msgpack" );
                  }
               }

               void key( const std::string& v )
               {
                  string( v );
               }

               void member() noexcept
               {
               }

               void end_object()
               {
                  assert( false );
               }

               void end_object( const std::size_t ) noexcept
               {
               }
            };

         }  // namespace msgpack

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
