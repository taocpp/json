// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_CBOR_MAJOR_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_CBOR_MAJOR_HPP

#include <cstdint>

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace cbor
         {
            enum class major : std::uint8_t
            {
               UNSIGNED = 0,
               NEGATIVE = 0x20,
               BINARY = 0x40,
               STRING = 0x60,
               ARRAY = 0x80,
               OBJECT = 0xa0,
               TAG = 0xc0,
               OTHER = 0xe0
            };

            static constexpr std::uint8_t major_mask = 0xe0;
            static constexpr std::uint8_t minor_mask = 0x1f;

         }  // namespace cbor

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
