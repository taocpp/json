// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CBOR_EVENTS_TO_STREAM_HPP
#define TAO_JSON_CBOR_EVENTS_TO_STREAM_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include "../internal/major.hpp"

#include "../../byte_view.hpp"
#include "../../external/string_view.hpp"
#include "../../internal/endian.hpp"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4310 )
#endif

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         namespace events
         {
            class to_stream
            {
            private:
               std::ostream& os;

            public:
               explicit to_stream( std::ostream& in_os ) noexcept
                  : os( in_os )
               {
               }

               void null()
               {
                  os.put( char( std::uint8_t( internal::major::OTHER ) + 22 ) );
               }

               void boolean( const bool v )
               {
                  os.put( char( std::uint8_t( internal::major::OTHER ) + 20 + std::uint8_t( v ) ) );
               }

               void number( const internal::major m, const std::uint64_t v )
               {
                  if( v < 24 ) {
                     os.put( char( std::uint8_t( m ) + v ) );
                  }
                  else if( v < 256 ) {
                     os.put( char( std::uint8_t( m ) + 24 ) );
                     os.put( char( v ) );
                  }
                  else if( v < 65536 ) {
                     os.put( char( std::uint8_t( m ) + 25 ) );
                     const std::uint16_t x = json::internal::h_to_be( std::uint16_t( v ) );
                     os.write( static_cast< const char* >( static_cast< const void* >( &x ) ), sizeof( x ) );
                  }
                  else if( v < 4294967296ull ) {
                     os.put( char( std::uint8_t( m ) + 26 ) );
                     const std::uint32_t x = json::internal::h_to_be( std::uint32_t( v ) );
                     os.write( static_cast< const char* >( static_cast< const void* >( &x ) ), sizeof( x ) );
                  }
                  else {
                     os.put( char( std::uint8_t( m ) + 27 ) );
                     const std::uint64_t x = json::internal::h_to_be( v );
                     os.write( static_cast< const char* >( static_cast< const void* >( &x ) ), sizeof( x ) );
                  }
               }

               void number( const std::int64_t v )
               {
                  if( v >= 0 ) {
                     number( internal::major::UNSIGNED, v );
                  }
                  else {
                     number( internal::major::NEGATIVE, -( v + 1 ) );
                  }
               }

               void number( const std::uint64_t v )
               {
                  number( internal::major::UNSIGNED, v );
               }

               void number( const double v )
               {
                  std::uint64_t n;
                  std::memcpy( &n, &v, sizeof( n ) );
                  n = json::internal::h_to_be( n );
                  os.put( char( std::uint8_t( internal::major::OTHER ) + 27 ) );
                  os.write( static_cast< const char* >( static_cast< const void* >( &n ) ), sizeof( n ) );
               }

               void string( const tao::string_view v )
               {
                  number( internal::major::STRING, v.size() );
                  os.write( v.data(), v.size() );
               }

               void binary( const tao::byte_view v )
               {
                  number( internal::major::BINARY, v.size() );
                  os.write( static_cast< const char* >( static_cast< const void* >( v.data() ) ), v.size() );
               }

               void begin_array()
               {
                  os.put( char( std::uint8_t( internal::major::ARRAY ) + internal::minor_mask ) );
               }

               void begin_array( const std::size_t size )
               {
                  number( internal::major::ARRAY, size );
               }

               void element() noexcept
               {
               }

               void end_array()
               {
                  os.put( char( 0xff ) );
               }

               void end_array( const std::size_t /*unused*/ ) noexcept
               {
               }

               void begin_object()
               {
                  os.put( char( std::uint8_t( internal::major::OBJECT ) + internal::minor_mask ) );
               }

               void begin_object( const std::size_t size )
               {
                  number( internal::major::OBJECT, size );
               }

               void key( const tao::string_view v )
               {
                  string( v );
               }

               void member() noexcept
               {
               }

               void end_object()
               {
                  os.put( char( 0xff ) );
               }

               void end_object( const std::size_t /*unused*/ ) noexcept
               {
               }
            };

         }  // namespace events

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
