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
            void null() noexcept
            {
            }

            void boolean( const bool ) noexcept
            {
            }

            void number( const std::int64_t ) noexcept
            {
            }

            void number( const std::uint64_t ) noexcept
            {
            }

            void number( const double ) noexcept
            {
            }

            void string( const tao::string_view ) noexcept
            {
            }

            void binary( const tao::byte_view ) noexcept
            {
            }

            void begin_array( const std::size_t = 0 ) noexcept
            {
            }

            void element() noexcept
            {
            }

            void end_array( const std::size_t = 0 ) noexcept
            {
            }

            void begin_object( const std::size_t = 0 ) noexcept
            {
            }

            void key( const tao::string_view ) noexcept
            {
            }

            void member() noexcept
            {
            }

            void end_object( const std::size_t = 0 ) noexcept
            {
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
