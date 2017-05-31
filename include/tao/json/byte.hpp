// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_BYTE_HPP
#define TAOCPP_JSON_INCLUDE_BYTE_HPP

#if __cplusplus >= 201703L
#define TAOCPP_JSON_HAS_STD_BYTE
#endif

#ifdef TAOCPP_JSON_HAS_STD_BYTE
#include <cstddef>
#else
#include <type_traits>
#endif

namespace tao
{
   namespace json
   {
#ifdef TAOCPP_JSON_HAS_STD_BYTE

      // if C++17's std::byte is available, use it
      using byte = std::byte;

#else

      // this is C++17's std::byte, including support functions
      enum class byte : unsigned char
      {
      };

      template< class IntType >
      typename std::enable_if< std::is_integral< IntType >::value, byte& >::type
      operator<<=( byte& b, IntType shift ) noexcept
      {
         return b = byte( static_cast< unsigned char >( b ) << shift );
      }

      template< class IntType >
      constexpr typename std::enable_if< std::is_integral< IntType >::value, byte >::type
      operator<<( byte b, IntType shift ) noexcept
      {
         return byte( static_cast< unsigned char >( b ) << shift );
      }

      template< class IntType >
      typename std::enable_if< std::is_integral< IntType >::value, byte& >::type
      operator>>=( byte& b, IntType shift ) noexcept
      {
         return b = byte( static_cast< unsigned char >( b ) >> shift );
      }

      template< class IntType >
      constexpr typename std::enable_if< std::is_integral< IntType >::value, byte >::type
      operator>>( byte b, IntType shift ) noexcept
      {
         return byte( static_cast< unsigned char >( b ) >> shift );
      }

      byte& operator|=( byte& l, byte r ) noexcept
      {
         return l = byte( static_cast< unsigned char >( l ) | static_cast< unsigned char >( r ) );
      }

      constexpr byte operator|( byte l, byte r ) noexcept
      {
         return byte( static_cast< unsigned char >( l ) | static_cast< unsigned char >( r ) );
      }

      byte& operator&=( byte& l, byte r ) noexcept
      {
         return l = byte( static_cast< unsigned char >( l ) & static_cast< unsigned char >( r ) );
      }

      constexpr byte operator&( byte l, byte r ) noexcept
      {
         return byte( static_cast< unsigned char >( l ) & static_cast< unsigned char >( r ) );
      }

      byte& operator^=( byte& l, byte r ) noexcept
      {
         return l = byte( static_cast< unsigned char >( l ) ^ static_cast< unsigned char >( r ) );
      }

      constexpr byte operator^( byte l, byte r ) noexcept
      {
         return byte( static_cast< unsigned char >( l ) ^ static_cast< unsigned char >( r ) );
      }

      constexpr byte operator~( byte b ) noexcept
      {
         return byte( ~static_cast< unsigned char >( b ) );
      }

#endif

      template< class IntType >
      constexpr typename std::enable_if< std::is_integral< IntType >::value, IntType >::type
      to_integer( byte b ) noexcept
      {
         return IntType( b );
      }

   }  // namespace json

}  // namespace tao

#endif
