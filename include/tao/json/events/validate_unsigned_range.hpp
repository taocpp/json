// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_UNSIGNED_RANGE_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_UNSIGNED_RANGE_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer, std::uint64_t Min, std::uint64_t Max >
         struct validate_unsigned_range
            : public Consumer
         {
            static_assert( Max >= Min, "Max may not be less than Min" );

            using Consumer::Consumer;

            void number( const std::int64_t v ) noexcept( noexcept( std::declval< Consumer >().number( v ) ) )
            {
               Consumer::number( v );
            }

            void number( const std::uint64_t v )
            {
               if( ( v < Min ) || ( v > Max ) ) {
                  throw std::runtime_error( "unsigned range violated: " + std::to_string( v ) );
               }
               Consumer::number( v );
            }

            void number( const double v ) noexcept( noexcept( std::declval< Consumer >().number( v ) ) )
            {
               Consumer::number( v );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
