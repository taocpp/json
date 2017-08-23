// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_DATA_UNION_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_DATA_UNION_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../byte_view.hpp"

#include "../external/byte.hpp"
#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      class data;

      namespace internal
      {
         union data_union
         {
            data_union() noexcept
            {
            }

            data_union( const data_union& ) = delete;
            void operator=( const data_union& ) = delete;

            ~data_union() noexcept
            {
            }

            bool b;

            std::int64_t i;
            std::uint64_t u;
            double d;

            std::string s;
            tao::string_view sv;

            std::vector< tao::byte > x;
            tao::byte_view xv;

            std::vector< data > a;

            std::map< std::string, data > o;

            const data* p;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
