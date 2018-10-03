// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_DATE_TIME_HELPER_HPP
#define TAO_JSON_INTERNAL_DATE_TIME_HELPER_HPP

#include <cstdint>
#include <ostream>
#include <stdexcept>

#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct date_time_ops
         {
            friend bool operator!=( const T lhs, const T rhs ) noexcept  // NOLINT
            {
               return !( lhs == rhs );
            };

            friend bool operator>( const T lhs, const T rhs ) noexcept  // NOLINT
            {
               return rhs < lhs;
            };

            friend bool operator<=( const T lhs, const T rhs ) noexcept  // NOLINT
            {
               return !( lhs > rhs );
            };

            friend bool operator>=( const T lhs, const T rhs ) noexcept  // NOLINT
            {
               return !( lhs < rhs );
            };
         };

         template< std::size_t I >
         constexpr std::uint8_t get( const tao::string_view sv )
         {
            return ( ( sv[ I ] >= '0' ) && ( sv[ I ] <= '9' ) ) ? ( sv[ I ] - '0' ) : throw std::runtime_error( "invalid digit: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
         }

         template< std::size_t I >
         constexpr std::uint8_t get_two( const tao::string_view sv )
         {
            return get< I >( sv ) * 10 + get< I + 1 >( sv );
         }

         template< std::size_t I >
         constexpr std::uint16_t get_four( const tao::string_view sv )
         {
            return get_two< I >( sv ) * 100 + get_two< I + 2 >( sv );
         }

         template< typename T >
         void write_two( std::ostream& os, const T v )
         {
            os.put( static_cast< char >( '0' + v / 10 ) );
            os.put( static_cast< char >( '0' + v % 10 ) );
         }

         template< typename T >
         void write_four( std::ostream& os, const T v )
         {
            write_two( os, v / 100 );
            write_two( os, v % 100 );
         }

         constexpr bool is_leap_year( const std::uint16_t year ) noexcept
         {
            return ( year % 4 == 0 ) && ( ( year % 100 != 0 ) || ( year % 400 == 0 ) );
         }

         constexpr std::uint8_t days_of_month( const std::uint16_t year, const std::uint8_t month ) noexcept
         {
            return ( month == 2 ) ? ( is_leap_year( year ) ? 29 : 28 ) : ( ( ( month == 4 ) || ( month == 6 ) || ( month == 9 ) || ( month == 11 ) ) ? 30 : 31 );
         }

         constexpr std::uint8_t nanodigits_from_nanosecond( const std::uint32_t nanosecond ) noexcept
         {
            return ( nanosecond == 0 ) ? 0 : ( nanosecond % 1000 != 0 ) ? 9 : ( nanosecond % 1000000 != 0 ) ? 6 : 3;
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
