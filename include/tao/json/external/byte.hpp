// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EXTERNAL_BYTE_HPP
#define TAO_JSON_EXTERNAL_BYTE_HPP

#ifndef TAO_JSON_USE_STD_BYTE
#if __cplusplus >= 201703L
#define TAO_JSON_USE_STD_BYTE
#endif
#endif

#if defined( TAO_JSON_USE_STD_BYTE )
#include <cstddef>
#else
#include <type_traits>
#endif

namespace tao
{

#if defined( TAO_JSON_USE_STD_BYTE )
   using std::byte;
   using std::to_integer;
#else

   // this is C++17's std::byte, including support functions.
   // note that prior to C++17, due to the lack of the
   // relaxed enum class initialization rules,
   // the byte{42}-syntax does not work.
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

   inline byte& operator|=( byte& l, byte r ) noexcept
   {
      return l = byte( static_cast< unsigned char >( l ) | static_cast< unsigned char >( r ) );
   }

   constexpr byte operator|( byte l, byte r ) noexcept
   {
      return byte( static_cast< unsigned char >( l ) | static_cast< unsigned char >( r ) );
   }

   inline byte& operator&=( byte& l, byte r ) noexcept
   {
      return l = byte( static_cast< unsigned char >( l ) & static_cast< unsigned char >( r ) );
   }

   constexpr byte operator&( byte l, byte r ) noexcept
   {
      return byte( static_cast< unsigned char >( l ) & static_cast< unsigned char >( r ) );
   }

   inline byte& operator^=( byte& l, byte r ) noexcept
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

   template< class IntType >
   constexpr typename std::enable_if< std::is_integral< IntType >::value, IntType >::type
   to_integer( byte b ) noexcept
   {
      return IntType( b );
   }

#endif

}  // namespace tao

#endif
