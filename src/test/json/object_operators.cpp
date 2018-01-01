// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         value v = empty_object;
         TEST_ASSERT( v.get_object().empty() );
         v += {};
         TEST_ASSERT( v.get_object().empty() );
         v += { { "foo", 1 } };
         TEST_ASSERT( v.get_object().size() == 1 );
         v += { { "bar", 2 }, { "baz", 3 } };
         TEST_ASSERT( v.get_object().size() == 3 );
         TEST_THROWS( v += { { "foo", 42 } } );
         TEST_THROWS( v -= { "what?" } );
         v -= {};
         TEST_ASSERT( v.get_object().size() == 3 );
         v -= { "bar" };
         TEST_THROWS( v -= { "bar" } );
         TEST_ASSERT( v.get_object().size() == 2 );
         v -= { "foo", "baz" };
         TEST_ASSERT( v.get_object().empty() );
         TEST_THROWS( v -= { "foo" } );
         TEST_THROWS( v -= { "bar" } );
         TEST_THROWS( v -= { "baz" } );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
