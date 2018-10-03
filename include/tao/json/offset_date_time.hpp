// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_OFFSET_DATE_TIME_HPP
#define TAO_JSON_OFFSET_DATE_TIME_HPP

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "local_date_time.hpp"

#include "external/string_view.hpp"
#include "internal/date_time_helper.hpp"

namespace tao
{
   namespace json
   {
      class offset_date_time
         : internal::date_time_ops< offset_date_time >
      {
         static bool validate_offset_basics( const tao::string_view sv )
         {
            if( sv.size() == 1 ) {
               if( ( sv[ 0 ] != 'Z' ) && ( sv[ 0 ] != 'z' ) ) {
                  throw std::runtime_error( "invalid offset character: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
               }
               return true;
            }
            if( sv.size() != 6 ) {
               throw std::runtime_error( "invalid offset length: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            if( ( sv[ 0 ] != '+' ) && ( sv[ 0 ] != '-' ) ) {
               throw std::runtime_error( "invalid prefix: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            if( sv[ 3 ] != ':' ) {
               throw std::runtime_error( "invalid separator: '" + std::string( sv.begin(), sv.end() ) + "'" );  // NOLINT
            }
            return true;
         }

         void validate() const
         {
            if( offset_hour < 0 ) {
               if( offset_hour < -23 ) {
                  throw std::runtime_error( "invalid offset hour '" + std::to_string( offset_hour ) + "'" );  // NOLINT
               }
               if( offset_minute > 0 ) {
                  throw std::runtime_error( "invalid offset minute, inconsistent signedness '" + std::to_string( offset_minute ) + "'" );  // NOLINT
               }
               if( offset_minute < -59 ) {
                  throw std::runtime_error( "invalid offset minute '" + std::to_string( offset_minute ) + "'" );  // NOLINT
               }
            }
            else {
               if( offset_hour > 23 ) {
                  throw std::runtime_error( "invalid offset hour '" + std::to_string( offset_hour ) + "'" );  // NOLINT
               }
               if( offset_minute < 0 ) {
                  throw std::runtime_error( "invalid offset minute, inconsistent signedness '" + std::to_string( offset_minute ) + "'" );  // NOLINT
               }
               if( offset_minute > 59 ) {
                  throw std::runtime_error( "invalid offset minute '" + std::to_string( offset_minute ) + "'" );  // NOLINT
               }
            }
         }

         offset_date_time( const tao::string_view sv_date_time, const tao::string_view sv_offset, const bool /*is_validated*/ )
            : date_time( local_date_time( sv_date_time ) ),
              offset_hour( ( sv_offset.size() == 1 ) ? 0 : internal::get_two< 1 >( sv_offset ) ),
              offset_minute( ( sv_offset.size() == 1 ) ? 0 : internal::get_two< 4 >( sv_offset ) )
         {
            if( sv_offset[ 0 ] == '-' ) {
               offset_hour *= -1;
               offset_minute *= -1;
            }
         }

         offset_date_time( const tao::string_view sv, const tao::string_view::size_type p )
            : offset_date_time( sv.substr( 0, p ), sv.substr( p ) )
         {
         }

      public:
         local_date_time date_time;
         std::int8_t offset_hour = 0;
         std::int8_t offset_minute = 0;  // note: if hour is negative, minute is negative as well (if not 0)

         offset_date_time( const local_date_time in_date_time, const std::int8_t in_offset_hour, const std::int8_t in_offset_minutes )
            : date_time( in_date_time ), offset_hour( in_offset_hour ), offset_minute( in_offset_minutes )
         {
            validate();
         }

         offset_date_time( const tao::string_view sv_date_time, const tao::string_view sv_offset )
            : offset_date_time( sv_date_time, sv_offset, validate_offset_basics( sv_offset ) )
         {
         }

         explicit offset_date_time( const tao::string_view sv )
            : offset_date_time( sv, sv.find_first_of( "Zz+-", 19 ) )
         {
         }

         bool has_offset() const noexcept
         {
            return ( offset_hour != 0 ) || ( offset_minute != 0);
         }
      };

      inline bool operator==( const offset_date_time lhs, const offset_date_time rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) == std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline bool operator<( const offset_date_time lhs, const offset_date_time rhs ) noexcept
      {
         return std::tie( lhs.date_time, lhs.offset_hour, lhs.offset_minute ) < std::tie( rhs.date_time, rhs.offset_hour, rhs.offset_minute );
      }

      inline std::ostream& operator<<( std::ostream& os, const offset_date_time v )
      {
         os << v.date_time;
         if( !v.has_offset() ) {
            os << 'Z';
            return os;
         }
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
         return os;
      }

   }  // namespace json

}  // namespace tao

#endif
