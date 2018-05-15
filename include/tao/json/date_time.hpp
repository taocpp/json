// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_DATE_TIME_HPP
#define TAO_JSON_DATE_TIME_HPP

#include <cstdint>

namespace tao
{
   namespace json
   {
      // TODO: placeholders, replace by the real types
      // clang-format off
      enum class local_date_t : std::uint64_t {};
      enum class local_time_t : std::uint64_t {};
      enum class local_date_time_t : std::uint64_t {};
      enum class offset_date_time_t : std::uint64_t {};
      // clang-format on

   }  // namespace json

}  // namespace tao

#endif
