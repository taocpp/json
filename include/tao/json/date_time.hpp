// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_DATE_TIME_HPP
#define TAO_JSON_DATE_TIME_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <tuple>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct date_time_ops
         {
            friend bool operator!=( const T& lhs, const T& rhs ) noexcept
            {
               return !( lhs == rhs );
            };

            friend bool operator>( const T& lhs, const T& rhs ) noexcept
            {
               return rhs < lhs;
            };

            friend bool operator<=( const T& lhs, const T& rhs ) noexcept
            {
               return !( lhs > rhs );
            };

            friend bool operator>=( const T& lhs, const T& rhs ) noexcept
            {
               return !( lhs < rhs );
            };
         };

      }  // namespace internal

      struct local_date_t
         : private internal::date_time_ops< local_date_t >
      {
         std::uint16_t year = 0;
         std::uint8_t month = 0;
         std::uint8_t day = 0;
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
         std::int8_t offset_minute = 0;
      };

      inline bool operator==( const local_date_t& lhs, const local_date_t& rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) == std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline bool operator==( const local_time_t& lhs, const local_time_t& rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) == std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline bool operator==( const local_date_time_t& lhs, const local_date_time_t& rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) == std::tie( rhs.date, rhs.time );
      }

      inline bool operator==( const offset_date_time_t& lhs, const offset_date_time_t& rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) == std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline bool operator<( const local_date_t& lhs, const local_date_t& rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) < std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline bool operator<( const local_time_t& lhs, const local_time_t& rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) < std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline bool operator<( const local_date_time_t& lhs, const local_date_time_t& rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) < std::tie( rhs.date, rhs.time );
      }

      inline bool operator<( const offset_date_time_t& lhs, const offset_date_time_t& rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) < std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date_t& v )
      {
         const auto f = os.fill( '0' );
         os << std::setw( 4 ) << v.year << '-' << std::setw( 2 ) << v.month << '-' << std::setw( 2 ) << v.day;
         os.fill( f );
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const local_time_t& v )
      {
         const auto f = os.fill( '0' );
         os << std::setw( 2 ) << v.hour << ':' << std::setw( 2 ) << v.minute << ':' << std::setw( 2 ) << v.second;
         if( v.nanodigits != 0U ) {
            os << '.';
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
         }
         os.fill( f );
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date_time_t& v )
      {
         os << v.date << 'T' << v.time;
         return os;
      }

      inline std::ostream& operator<<( std::ostream& os, const offset_date_time_t& v )
      {
         os << v.date_time;
         if( ( v.offset_hour == 0 ) && ( v.offset_minute == 0 ) ) {
            os << 'Z';
         }
         else {
            const auto f = os.fill( '0' );
            if( v.offset_hour > 0 ) {
               os << '+' << std::setw( 2 ) << v.offset_hour << ':' << std::setw( 2 ) << v.offset_minute;
            }
            else {
               os << '-' << std::setw( 2 ) << -v.offset_hour << ':' << std::setw( 2 ) << v.offset_minute;
            }
            os.fill( f );
         }
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
