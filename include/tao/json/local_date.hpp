// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_LOCAL_DATE_HPP
#define TAO_JSON_LOCAL_DATE_HPP

#include <cstdint>
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
      class local_date
         : internal::date_time_ops< local_date >
      {
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

         explicit local_date( const tao::string_view sv, const bool /*is_validated*/ )
            : local_date( internal::get_four< 0 >( sv ), internal::get_two< 5 >( sv ), internal::get_two< 8 >( sv ) )
         {
         }

      public:
         std::uint16_t year = 1970;
         std::uint8_t month = 1;
         std::uint8_t day = 1;

         local_date( const std::uint16_t in_year, const std::uint8_t in_month, const std::uint8_t in_day )
            : year( in_year ), month( in_month ), day( in_day )
         {
            if( year > 9999 ) {
               throw std::runtime_error( "invalid year '" + std::to_string( year ) + "'" );  // NOLINT
            }
            if( ( month == 0 ) || ( month > 12 ) ) {
               throw std::runtime_error( "invalid month '" + std::to_string( month ) + "'" );  // NOLINT
            }
            if( ( day == 0 ) || ( day > internal::days_of_month( year, month ) ) ) {
               throw std::runtime_error( "invalid day '" + std::to_string( day ) + "' for year '" + std::to_string( year ) + "', month '" + std::to_string( month ) + "'" );  // NOLINT
            }
         }

         explicit local_date( const tao::string_view sv )
            : local_date( sv, validate_basics( sv ) )
         {
         }
      };

      inline bool operator==( const local_date lhs, const local_date rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) == std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline bool operator<( const local_date lhs, const local_date rhs ) noexcept
      {
         return std::tie( lhs.year, lhs.month, lhs.day ) < std::tie( rhs.year, rhs.month, rhs.day );
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date v )
      {
         internal::write_two( os, v.year / 100 );
         internal::write_two( os, v.year % 100 );
         os.put( '-' );
         internal::write_two( os, v.month );
         os.put( '-' );
         internal::write_two( os, v.day );
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
