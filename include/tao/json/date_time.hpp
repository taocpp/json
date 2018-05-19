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

      }  // namespace internal

      class local_date_t
         : internal::date_time_ops< local_date_t >
      {
         static std::uint8_t
         days_of_month( const std::uint16_t year, const std::uint8_t month ) noexcept
         {
            return ( month == 2 ) ? ( ( ( year % 4 == 0 ) && ( ( year % 100 != 0 ) || ( year % 400 == 0 ) ) ) ? 29 : 28 ) : ( ( ( month == 4 ) || ( month == 6 ) || ( month == 9 ) || ( month == 11 ) ) ? 30 : 31 );
         }

         static bool validate_basics( const tao::string_view sv )
         {
            if( sv.size() != 10 ) {
               throw std::runtime_error( "invalid length: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            if( ( sv[ 4 ] != '-' ) || ( sv[ 7 ] != '-' ) ) {
               throw std::runtime_error( "invalid separator(s): '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            return true;
         }

         explicit local_date_t( const tao::string_view sv, const bool /*is_validated*/ )
            : local_date_t( internal::get_four< 0 >( sv ), internal::get_two< 5 >( sv ), internal::get_two< 8 >( sv ) )
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
               throw std::runtime_error( "invalid month '" + std::to_string( month ) + "'" );  // NOLINT
            }
            if( ( day == 0 ) || ( day > days_of_month( year, month ) ) ) {
               throw std::runtime_error( "invalid day '" + std::to_string( day ) + "' for year '" + std::to_string( year ) + "', month '" + std::to_string( month ) + "'" );  // NOLINT
            }
         }

         explicit local_date_t( const tao::string_view sv )
            : local_date_t( sv, validate_basics( sv ) )
         {
         }
      };

      class local_time_t
         : internal::date_time_ops< local_time_t >
      {
         static bool validate_basics( const tao::string_view sv )
         {
            const auto s = sv.size();
            if( s < 8 || s == 9 ) {
               throw std::runtime_error( "invalid length: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            if( ( sv[ 2 ] != ':' ) || ( sv[ 5 ] != ':' ) || ( ( s > 8 ) && ( sv[ 8 ] != '.' ) ) ) {
               throw std::runtime_error( "invalid separator(s): '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            return true;
         }

         void validate() const
         {
            if( hour > 23 ) {
               throw std::runtime_error( "invalid hour '" + std::to_string( hour ) + "'" );  // NOLINT
            }
            if( minute > 59 ) {
               throw std::runtime_error( "invalid minute '" + std::to_string( minute ) + "'" );  // NOLINT
            }
            if( second > 59 ) {
               throw std::runtime_error( "invalid second '" + std::to_string( second ) + "'" );  // NOLINT
            }
            if( nanosecond > 999999999 ) {
               throw std::runtime_error( "invalid nanosecond '" + std::to_string( nanosecond ) + "'" );  // NOLINT
            }
         }

         explicit local_time_t( const tao::string_view sv, const bool /*is_validated*/ )
            : hour( internal::get_two< 0 >( sv ) ),
              minute( internal::get_two< 3 >( sv ) ),
              second( internal::get_two< 6 >( sv ) )
         {
            validate();

            if( sv.size() > 8 ) {
               nanodigits = sv.size() - 9;
               switch( nanodigits ) {
                  case 1:
                     nanosecond = 100000000 * internal::get< 9 >( sv );
                     break;

                  case 2:
                     nanosecond = 10000000 * internal::get_two< 9 >( sv );
                     break;

                  case 3:
                     nanosecond = 1000000 * ( internal::get_two< 9 >( sv ) * 10 + internal::get< 11 >( sv ) );
                     break;

                  case 4:
                     nanosecond = 100000 * internal::get_four< 9 >( sv );
                     break;

                  case 5:
                     nanosecond = 10000 * ( internal::get_four< 9 >( sv ) * 10 + internal::get< 13 >( sv ) );
                     break;

                  case 6:
                     nanosecond = 1000 * ( internal::get_four< 9 >( sv ) * 100 + internal::get_two< 13 >( sv ) );
                     break;

                  case 7:
                     nanosecond = 100 * ( internal::get_four< 9 >( sv ) * 1000 + internal::get_two< 13 >( sv ) * 10 + internal::get< 15 >( sv ) );
                     break;

                  case 8:
                     nanosecond = 10 * ( internal::get_four< 9 >( sv ) * 10000 + internal::get_four< 13 >( sv ) );
                     break;

                  default:
                     nanosecond = internal::get_four< 9 >( sv ) * 100000 + internal::get_four< 13 >( sv ) * 10 + internal::get< 17 >( sv );
                     nanodigits = 9;
               }
            }
         }

      public:
         std::uint8_t hour = 0;
         std::uint8_t minute = 0;
         std::uint8_t second = 0;
         std::uint8_t nanodigits = 0;
         std::uint32_t nanosecond = 0;

         local_time_t() = default;

         local_time_t( const std::uint8_t in_hour, const std::uint8_t in_minute, const std::uint8_t in_second, const std::uint32_t in_nanosecond = 0 )
            : hour( in_hour ), minute( in_minute ), second( in_second ), nanosecond( in_nanosecond )
         {
            validate();

            if( nanosecond != 0 ) {
               if( nanosecond % 1000 == 0 ) {
                  if( nanodigits % 1000000 == 0 ) {
                     nanodigits = 3;
                  }
                  else {
                     nanodigits = 6;
                  }
               }
               else {
                  nanodigits = 9;
               }
            }
         }

         explicit local_time_t( const tao::string_view sv )
            : local_time_t( sv, validate_basics( sv ) )
         {
         }
      };

      struct local_date_time_t
         : private internal::date_time_ops< local_date_time_t >
      {
         local_date_t date;
         local_time_t time;

         constexpr local_date_time_t() = default;

         constexpr local_date_time_t( const local_date_t in_date, const local_time_t in_time )
            : date( in_date ), time( in_time )
         {
         }

         explicit local_date_time_t( const tao::string_view sv )
            : local_date_time_t( local_date_t( sv.substr( 0, 10 ) ), local_time_t( sv.substr( 11 ) ) )
         {
         }
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
               default: throw std::logic_error( "invalid nanodigits: " + std::to_string( v.nanodigits ) );  // NOLINT
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
