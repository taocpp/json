// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_UNION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_UNION_HH

#include <map>
#include <vector>
#include <string>

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
            { }

            value_union( const value_union & ) = delete;
            void operator= ( const value_union & ) = delete;

            ~value_union()
            { }

            bool b;
            int64_t i;
            double d;
            void * v;

            std::string s;
            std::vector< Value > a;
            std::map< std::string, Value > o;
         };

      } // internal

   } // json

} // tao

#endif
