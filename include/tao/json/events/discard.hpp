// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_DISCARD_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_DISCARD_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#include "../byte_view.hpp"

#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer that discards events.

         struct discard
         {
            void null() const noexcept
            {
            }

            void boolean( const bool ) const noexcept
            {
            }

            void number( const std::int64_t ) const noexcept
            {
            }

            void number( const std::uint64_t ) const noexcept
            {
            }

            void number( const double ) const noexcept
            {
            }

            void string( const tao::string_view ) const noexcept
            {
            }

            void binary( const tao::byte_view ) const noexcept
            {
            }

            void begin_array( const std::size_t = 0 ) const noexcept
            {
            }

            void element() const noexcept
            {
            }

            void end_array( const std::size_t = 0 ) const noexcept
            {
            }

            void begin_object( const std::size_t = 0 ) const noexcept
            {
            }

            void key( const tao::string_view ) const noexcept
            {
            }

            void member() const noexcept
            {
            }

            void end_object( const std::size_t = 0 ) const noexcept
            {
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
