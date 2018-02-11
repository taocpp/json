// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_DISCARD_HPP
#define TAO_JSON_EVENTS_DISCARD_HPP

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

            void boolean( const bool /*unused*/ ) noexcept
            {
            }

            void number( const std::int64_t /*unused*/ ) noexcept
            {
            }

            void number( const std::uint64_t /*unused*/ ) noexcept
            {
            }

            void number( const double /*unused*/ ) noexcept
            {
            }

            void string( const tao::string_view /*unused*/ ) noexcept
            {
            }

            void binary( const tao::byte_view /*unused*/ ) noexcept
            {
            }

            void begin_array( const std::size_t /*unused*/ = 0 ) noexcept
            {
            }

            void element() noexcept
            {
            }

            void end_array( const std::size_t /*unused*/ = 0 ) noexcept
            {
            }

            void begin_object( const std::size_t /*unused*/ = 0 ) noexcept
            {
            }

            void key( const tao::string_view /*unused*/ ) noexcept
            {
            }

            void member() noexcept
            {
            }

            void end_object( const std::size_t /*unused*/ = 0 ) noexcept
            {
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
