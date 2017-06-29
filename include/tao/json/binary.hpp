// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_BINARY_HPP
#define TAOCPP_JSON_INCLUDE_BINARY_HPP

#include <vector>

#include "byte.hpp"
#include "internal/integer_sequence.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         constexpr char unhex( const char c ) noexcept
         {
            return ( c < 'A' ) ? ( c - '0' ) : ( ( c < 'a' ) ? ( c - 'A' + 10 ) : ( c - 'a' + 10 ) );
         }

         template< typename T, typename V, char... Cs, std::size_t... Is >
         constexpr T unhex( index_sequence< Is... > )
         {
            const char a[] = { unhex( Cs )... };
            return T{ V( ( a[ 2 * Is ] << 4 ) + a[ 2 * Is + 1 ] )... };
         }

         template< typename T, typename V, char C0, char C1, char... Cs >
         constexpr T unhex()
         {
            static_assert( C0 == '0', "not a hex literal" );
            static_assert( C1 == 'x' || C1 == 'X', "not a hex literal" );
            static_assert( sizeof...( Cs ) % 2 == 0, "invalid number of hexadecimal digits" );
            return unhex< T, V, Cs... >( make_index_sequence< sizeof...( Cs ) / 2 >() );
         }

      }  // namespace internal

      inline namespace literals
      {
         template< char... Cs >
         std::vector< byte > operator"" _binary()
         {
            return internal::unhex< std::vector< byte >, byte, Cs... >();
         }

      }  // namespace literals

   }  // namespace json

}  // namespace tao

#endif
