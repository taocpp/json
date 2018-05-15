// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_VALUE_UNION_HPP
#define TAO_JSON_INTERNAL_VALUE_UNION_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../binary_view.hpp"
#include "../date_time.hpp"
#include "../forward.hpp"

#include "../external/byte.hpp"
#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct opaque_ptr_t
         {
            const void* data;
            producer_t producer;
         };

         template< typename T >
         union value_union
         {
            value_union() noexcept  // NOLINT
            {
            }

            value_union( const value_union& ) = delete;
            value_union( value_union&& ) = delete;

            ~value_union() noexcept  // NOLINT
            {
            }

            void operator=( const value_union& ) = delete;
            void operator=( value_union&& ) = delete;

            bool b;

            std::int64_t i;
            std::uint64_t u;
            double d;

            std::string s;
            tao::string_view sv;

            std::vector< tao::byte > x;
            tao::binary_view xv;

            local_date_t ld;         // local date
            local_time_t lt;         // local time
            local_date_time_t ldt;   // local date-time
            offset_date_time_t odt;  // offset-date-time

            std::vector< T > a;

            std::map< std::string, T > o;

            const T* p;
            opaque_ptr_t q;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
