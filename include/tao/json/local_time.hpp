// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_LOCAL_TIME_HPP
#define TAO_JSON_LOCAL_TIME_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "external/string_view.hpp"
#include "internal/date_time_helper.hpp"

namespace tao
{
   namespace json
   {
      class local_time
         : internal::date_time_ops< local_time >
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
            if( nanodigits > 9 ) {
               throw std::runtime_error( "invalid nanodigits '" + std::to_string( nanodigits ) + "'" );  // NOLINT
            }
         }

         explicit local_time( const tao::string_view sv, const bool /*is_validated*/ )
            : hour( internal::get_two< 0 >( sv ) ),
              minute( internal::get_two< 3 >( sv ) ),
              second( internal::get_two< 6 >( sv ) )
         {
            validate();

            if( sv.size() > 8 ) {
               if( sv.size() > 18 ) {
                  nanodigits = 9;
               }
               else {
                  nanodigits = static_cast< std::uint8_t >( sv.size() - 9 );
               }
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
               }
            }
         }

      public:
         std::uint8_t hour = 0;
         std::uint8_t minute = 0;
         std::uint8_t second = 0;
         std::uint8_t nanodigits = 0;
         std::uint32_t nanosecond = 0;

         local_time( const std::uint8_t in_hour, const std::uint8_t in_minute, const std::uint8_t in_second, const std::uint32_t in_nanosecond = 0 )
            : hour( in_hour ), minute( in_minute ), second( in_second ), nanodigits( internal::nanodigits_from_nanosecond( in_nanosecond ) ), nanosecond( in_nanosecond )
         {
            validate();
         }

         local_time( const std::uint8_t in_hour, const std::uint8_t in_minute, const std::uint8_t in_second, const std::uint32_t in_nanosecond, const std::uint8_t in_nanodigits )
            : hour( in_hour ), minute( in_minute ), second( in_second ), nanodigits( in_nanodigits ), nanosecond( in_nanosecond )
         {
            validate();
         }

         explicit local_time( const tao::string_view sv )
            : local_time( sv, validate_basics( sv ) )
         {
         }
      };

      inline bool operator==( const local_time lhs, const local_time rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) == std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline bool operator<( const local_time lhs, const local_time rhs ) noexcept
      {
         return std::tie( lhs.hour, lhs.minute, lhs.second, lhs.nanosecond ) < std::tie( rhs.hour, rhs.minute, rhs.second, rhs.nanosecond );
      }

      inline std::ostream& operator<<( std::ostream& os, const local_time v )
      {
         internal::write_two( os, v.hour );
         os.put( ':' );
         internal::write_two( os, v.minute );
         os.put( ':' );
         internal::write_two( os, v.second );
         if( v.nanodigits != 0 ) {
            os << '.';
            const auto f = os.fill( '0' );
            switch( v.nanodigits ) {
               case 9:
                  os << std::setw( 9 ) << v.nanosecond;
                  break;

               case 8:
                  os << std::setw( 8 ) << ( v.nanosecond / 10 );
                  break;

               case 7:
                  os << std::setw( 7 ) << ( v.nanosecond / 100 );
                  break;

               case 6:
                  os << std::setw( 6 ) << ( v.nanosecond / 1000 );
                  break;

               case 5:
                  os << std::setw( 5 ) << ( v.nanosecond / 10000 );
                  break;

               case 4:
                  os << std::setw( 4 ) << ( v.nanosecond / 100000 );
                  break;

               case 3:
                  os << std::setw( 3 ) << ( v.nanosecond / 1000000 );
                  break;

               case 2:
                  os << std::setw( 2 ) << ( v.nanosecond / 10000000 );
                  break;

               case 1:
                  os << std::setw( 1 ) << ( v.nanosecond / 100000000 );
                  break;

               default:
                  throw std::logic_error( "invalid nanodigits: " + std::to_string( v.nanodigits ) );  // NOLINT
            }
            os.fill( f );
         }
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
