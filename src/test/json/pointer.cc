// Copyright (c) 2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         const value v { { "foo", 1 } };
         const value v2 { { "bar", &v } };  // Store a non-owning const pointer to v.
         TEST_ASSERT( to_string( v ) == "{\"foo\":1}" );
         TEST_ASSERT( to_string( v2 ) == "{\"bar\":{\"foo\":1}}" );
         TEST_ASSERT( to_string( v2[ "bar" ].type() ) == "pointer" );
      }

   } // json

} // tao

#include "main.hh"
