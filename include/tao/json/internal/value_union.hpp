// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_UNION_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_UNION_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Value >
         union value_union
         {
            value_union()
            {
            }

            value_union( const value_union& ) = delete;
            void operator=( const value_union& ) = delete;

            ~value_union()
            {
            }

            bool b;
            std::int64_t i;
            std::uint64_t u;
            double d;

            std::string s;
            std::vector< Value > a;
            std::map< std::string, Value > o;

            const Value* p;
         };

      }  // internal

   }  // json

}  // tao

#endif
