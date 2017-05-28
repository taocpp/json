// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_INTERNAL_TEMPORARY_HPP
#define TAOCPP_JSON_INCLUDE_JSON_INTERNAL_TEMPORARY_HPP

#include <cassert>
#include <cstdint>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // TODO: Where to put this header?
         // TODO: Support float and double.
         // TODO: Support big-endian platforms (degrade to a nop).
         // TODO: Support other compilers (VS: intrin.h's _byteswap_ushort etc.)
         // TODO: Support other platforms (don't perform unaligned memory access).

         template< unsigned S > struct bswap;

         template<> struct bswap< 1 >
         {
            static std::uint8_t convert( const std::uint8_t n ) noexcept
            {
               return n;
            }
         };

         template<> struct bswap< 2 >
         {
            static std::uint16_t convert( const std::uint16_t n ) noexcept
            {
               return __builtin_bswap16( n );
            }
         };

         template<> struct bswap< 4 >
         {
            static std::uint32_t convert( const std::uint32_t n ) noexcept
            {
               return __builtin_bswap32( n );
            }
         };

         template<> struct bswap< 8 >
         {
            static std::uint64_t convert( const std::uint64_t n ) noexcept
            {
               return __builtin_bswap64( n );
            }
         };

         template< typename N >
         N h_to_be( const N n )
         {
            return N( bswap< sizeof( N ) >::convert( n ) );
         }

         template< typename N >
         N be_to_h( const N n )
         {
            return h_to_be( n );
         }

         template< typename N >
         N be_to_h( const void* p )
         {
            return internal::be_to_h( *reinterpret_cast< const N* >( p ) );
         }

      } // internal

   } // namespace json

} // namespace tao

#endif
