// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_DATE_TIME_HPP
#define TAO_JSON_DATE_TIME_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <tuple>

#include "external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct date_time_ops
         {
            friend bool operator!=( const T lhs, const T rhs ) noexcept
            {
               return !( lhs == rhs );
            };

            friend bool operator>( const T lhs, const T rhs ) noexcept
            {
               return rhs < lhs;
            };

            friend bool operator<=( const T lhs, const T rhs ) noexcept
            {
               return !( lhs > rhs );
            };

            friend bool operator>=( const T lhs, const T rhs ) noexcept
            {
               return !( lhs < rhs );
            };
         };

         template< typename T >
         void write_two( std::ostream& os, const T v )
         {
            os.put( static_cast< char >( '0' + v / 10 ) );
            os.put( static_cast< char >( '0' + v % 10 ) );
         }

      }  // namespace internal

      struct local_date_t
         : private internal::date_time_ops< local_date_t >
      {
      private:
         static std::uint8_t days_of_month( const std::uint16_t year, const std::uint8_t month ) noexcept
         {
            return ( month == 2 ) ? ( ( ( year % 4 == 0 ) && ( ( year % 100 != 0 ) || ( year % 400 == 0 ) ) ) ? 29 : 28 ) : ( ( ( month == 4 ) || ( month == 6 ) || ( month == 9 ) || ( month == 11 ) ) ? 30 : 31 );
         }

         static bool validate_basics( const tao::string_view sv )
         {
            if( sv.size() != 10 ) {
               throw std::runtime_error( "invalid length: '" + std::string( sv.begin(), sv.end() ) + "'" );
            }
            if( ( sv[ 4 ] != '-' ) || ( sv[ 7 ] != '-' ) ) {
               throw std::runtime_error( "invalid separator: '" + std::string( sv.begin(), sv.end() ) + "'" );
            }
            return true;
         }

         explicit local_date_t( const tao::string_view sv, const bool /*is_validated*/ )
            : local_date_t( ( ( ( sv[ 0 ] - '0' ) * 10 + ( sv[ 1 ] - '0' ) ) * 10 + ( sv[ 2 ] - '0' ) ) * 10 + ( sv[ 3 ] - '0' ),
                            ( sv[ 5 ] - '0' ) * 10 + ( sv[ 6 ] - '0' ),
                            ( sv[ 8 ] - '0' ) * 10 + ( sv[ 9 ] - '0' ) )
         {
         }

      public:
         std::uint16_t year = 1970;
         std::uint8_t month = 1;
         std::uint8_t day = 1;

         local_date_t() = default;

         local_date_t( const std::uint16_t in_year, const std::uint8_t in_month, const std::uint8_t in_day )
            : year( in_year ), month( in_month ), day( in_day )
         {
            if( ( month == 0 ) || ( month > 12 ) ) {
               throw std::runtime_error( "invalid month '" + std::to_string( month ) + "'" );
            }
            if( ( day == 0 ) || ( day > days_of_month( year, month ) ) ) {
               throw std::runtime_error( "invalid day '" + std::to_string( day ) + "' for year '" + std::to_string( year ) + "', month '" + std::to_string( month ) + "'" );
            }
         }

         explicit local_date_t( const tao::string_view sv )
            : local_date_t( sv, validate_basics( sv ) )
         {
         }
      };

      struct local_time_t
         : private internal::date_time_ops< local_time_t >
      {
         std::uint8_t hour = 0;
         std::uint8_t minute = 0;
         std::uint8_t second = 0;
         std::uint8_t nanodigits = 0;
         std::uint32_t nanosecond = 0;
      };

      struct local_date_time_t
         : private internal::date_time_ops< local_date_time_t >
      {
         local_date_t date;
         local_time_t time;
      };

      struct offset_date_time_t
         : private internal::date_time_ops< offset_date_time_t >
      {
         local_date_time_t date_time;
         std::int8_t offset_hour = 0;
         std::int8_t offset_minute = 0;  // note: if hour is negative, minute is negative as well (if not 0)
      };

      inline bool operator==( const local_date_t lhs, const local_date_t rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) == std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline bool operator==( const local_time_t lhs, const local_time_t rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) == std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline bool operator==( const local_date_time_t lhs, const local_date_time_t rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) == std::tie( rhs.date, rhs.time );
      }

      inline bool operator==( const offset_date_time_t lhs, const offset_date_time_t rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) == std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline bool operator<( const local_date_t lhs, const local_date_t rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) < std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline bool operator<( const local_time_t lhs, const local_time_t rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) < std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline bool operator<( const local_date_time_t lhs, const local_date_time_t rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) < std::tie( rhs.date, rhs.time );
      }

      inline bool operator<( const offset_date_time_t lhs, const offset_date_time_t rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) < std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date_t v )
      {
         internal::write_two( os, v.year / 100 );
         internal::write_two( os, v.year % 100 );
         os.put( '-' );
         internal::write_two( os, v.month );
         os.put( '-' );
         internal::write_two( os, v.day );
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const local_time_t v )
      {
         internal::write_two( os, v.hour );
         os.put( ':' );
         internal::write_two( os, v.minute );
         os.put( ':' );
         internal::write_two( os, v.second );
         if( v.nanodigits != 0 ) {
            os << '.';
            const auto f = os.fill( '0' );
            // clang-format off
            switch( v.nanodigits ) {
               case 9: os << std::setw( 9 ) << v.nanosecond; break;
               case 8: os << std::setw( 8 ) << ( v.nanosecond / 10 ); break;
               case 7: os << std::setw( 7 ) << ( v.nanosecond / 100 ); break;
               case 6: os << std::setw( 6 ) << ( v.nanosecond / 1000 ); break;
               case 5: os << std::setw( 5 ) << ( v.nanosecond / 10000 ); break;
               case 4: os << std::setw( 4 ) << ( v.nanosecond / 100000 ); break;
               case 3: os << std::setw( 3 ) << ( v.nanosecond / 1000000 ); break;
               case 2: os << std::setw( 2 ) << ( v.nanosecond / 10000000 ); break;
               case 1: os << std::setw( 1 ) << ( v.nanosecond / 100000000 ); break;
               default: throw std::logic_error( "invalid nanodigits: " + std::to_string( v.nanodigits ) ); // NOLINT
            }
            // clang-format on
            os.fill( f );
         }
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date_time_t v )
      {
         os << v.date << 'T' << v.time;
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const offset_date_time_t v )
      {
         os << v.date_time;
         if( ( v.offset_hour == 0 ) && ( v.offset_minute == 0 ) ) {
            os << 'Z';
         }
         else {
            if( v.offset_hour > 0 ) {
               os.put( '+' );
               internal::write_two( os, v.offset_hour );
               os.put( ':' );
               internal::write_two( os, v.offset_minute );
            }
            else {
               os.put( '-' );
               internal::write_two( os, -v.offset_hour );
               os.put( ':' );
               internal::write_two( os, -v.offset_minute );
            }
         }
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
