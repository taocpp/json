// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TEMPORARY_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_TEMPORARY_HPP

#include <cstdint>
#include <cstring>

#if defined( _WIN32 ) && !defined( __MINGW32__ )
#include <stdlib.h>  // TODO: Or is intrin.h the 'more correct' header?
#endif

// TODO: Better endian detection?
// TODO: Where to put this header?

namespace tao
{
   namespace json
   {
      namespace internal
      {

#if defined( _WIN32 ) && !defined( __MINGW32__ )

         template< unsigned S >
         struct bswap;

         template<>
         struct bswap< 1 >
         {
            static std::uint8_t convert( const std::uint8_t n ) noexcept
            {
               return n;
            }
         };

         template<>
         struct bswap< 2 >
         {
            static std::uint16_t convert( const std::uint16_t n ) noexcept
            {
               return _byteswap_ushort( n );
            }
         };

         template<>
         struct bswap< 4 >
         {
            static float convert( float n ) noexcept
            {
               std::uint32_t u;
               std::memcpy( &u, &n, 4 );
               u = convert( u );
               std::memcpy( &n, &u, 4 );
               return n;
            }

            static std::uint32_t convert( const std::uint32_t n ) noexcept
            {
               return _byteswap_ulong( n );
            }
         };

         template<>
         struct bswap< 8 >
         {
            static double convert( double n ) noexcept
            {
               std::uint64_t u;
               std::memcpy( &u, &n, 8 );
               u = convert( u );
               std::memcpy( &n, &u, 8 );
               return n;
            }

            static std::uint64_t convert( const std::uint64_t n ) noexcept
            {
               return _byteswap_uint64( n );
            }
         };

#elif not defined( __BYTE_ORDER__ )

#error TODO!

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

         template< unsigned S >
         struct bswap
         {
            template< typename T >
            static T convert( const T n ) noexcept
            {
               return n;
            }
         };

#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

         template< unsigned S >
         struct bswap;

         template<>
         struct bswap< 1 >
         {
            static std::uint8_t convert( const std::uint8_t n ) noexcept
            {
               return n;
            }
         };

         template<>
         struct bswap< 2 >
         {
            static std::uint16_t convert( const std::uint16_t n ) noexcept
            {
               return __builtin_bswap16( n );
            }
         };

         template<>
         struct bswap< 4 >
         {
            static float convert( float n ) noexcept
            {
               std::uint32_t u;
               std::memcpy( &u, &n, 4 );
               u = convert( u );
               std::memcpy( &n, &u, 4 );
               return n;
            }

            static std::uint32_t convert( const std::uint32_t n ) noexcept
            {
               return __builtin_bswap32( n );
            }
         };

         template<>
         struct bswap< 8 >
         {
            static double convert( double n ) noexcept
            {
               std::uint64_t u;
               std::memcpy( &u, &n, 8 );
               u = convert( u );
               std::memcpy( &n, &u, 8 );
               return n;
            }

            static std::uint64_t convert( const std::uint64_t n ) noexcept
            {
               return __builtin_bswap64( n );
            }
         };

#else

#error Unknown host byte order!

#endif

         template< typename N >
         N h_to_be( const N n ) noexcept
         {
            return N( bswap< sizeof( N ) >::convert( n ) );
         }

         template< typename N >
         N be_to_h( const N n ) noexcept
         {
            return h_to_be( n );
         }

         template< typename N >
         N be_to_h( const void* p ) noexcept
         {
            N n;
            std::memcpy( &n, p, sizeof( n ) );
            return internal::be_to_h( n );
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
