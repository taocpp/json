// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_STATISTICS_HPP
#define TAO_JSON_EVENTS_STATISTICS_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "../byte_view.hpp"

#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         struct statistics
         {
            std::size_t null_count = 0;
            std::size_t true_count = 0;
            std::size_t false_count = 0;

            std::size_t signed_count = 0;
            std::size_t unsigned_count = 0;
            std::size_t double_count = 0;

            std::size_t string_count = 0;
            std::size_t string_lengths = 0;
            std::size_t key_count = 0;
            std::size_t key_lengths = 0;
            std::size_t binary_count = 0;
            std::size_t binary_lengths = 0;

            std::size_t array_count = 0;
            std::size_t array_elements = 0;
            std::size_t object_count = 0;
            std::size_t object_members = 0;

            void null()
            {
               ++null_count;
            }

            void boolean( const bool v )
            {
               ++( v ? true_count : false_count );
            }

            void number( const std::int64_t /*unused*/ )
            {
               ++signed_count;
            }

            void number( const std::uint64_t /*unused*/ )
            {
               ++unsigned_count;
            }

            void number( const double /*unused*/ )
            {
               ++double_count;
            }

            void string( const tao::string_view v )
            {
               ++string_count;
               string_lengths += v.size();
            }

            void binary( const tao::byte_view v )
            {
               ++binary_count;
               binary_lengths += v.size();
            }

            void begin_array( const std::size_t /*unused*/ = 0 )
            {
               ++array_count;
            }

            void element()
            {
               ++array_elements;
            }

            void end_array( const std::size_t /*unused*/ = 0 )
            {
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               ++object_count;
            }

            void key( const tao::string_view v )
            {
               ++key_count;
               key_lengths += v.size();
            }

            void member()
            {
               ++object_members;
            }

            void end_object( const std::size_t /*unused*/ = 0 )
            {
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
