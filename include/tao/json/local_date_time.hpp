// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_LOCAL_DATE_TIME_HPP
#define TAO_JSON_LOCAL_DATE_TIME_HPP

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "local_date.hpp"
#include "local_time.hpp"

#include "external/string_view.hpp"
#include "internal/date_time_helper.hpp"

namespace tao
{
   namespace json
   {
      class local_date_time
         : internal::date_time_ops< local_date_time >
      {
         static bool validate_basics( const tao::string_view sv )
         {
            if( sv.size() < 19 ) {
               throw std::runtime_error( "invalid length: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            const char c = sv[ 10 ];
            if( ( c != 'T' ) && ( c != 't' ) && ( c != ' ' ) ) {
               throw std::runtime_error( "invalid separator(s): '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            return true;
         }

         local_date_time( const tao::string_view sv, const bool /*is_validated*/ )
            : local_date_time( local_date( sv.substr( 0, 10 ) ), local_time( sv.substr( 11 ) ) )
         {
         }

      public:
         local_date date;
         local_time time;

         local_date_time( const local_date in_date, const local_time in_time )
            : date( in_date ), time( in_time )
         {
         }

         explicit local_date_time( const tao::string_view sv )
            : local_date_time( sv, validate_basics( sv ) )
         {
         }
      };

      inline bool operator==( const local_date_time lhs, const local_date_time rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) == std::tie( rhs.date, rhs.time );
      }

      inline bool operator<( const local_date_time lhs, const local_date_time rhs ) noexcept
      {
         return std::tie( lhs.date, lhs.time ) < std::tie( rhs.date, rhs.time );
      }

      inline std::ostream& operator<<( std::ostream& os, const local_date_time v )
      {
         os << v.date << 'T' << v.time;
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
