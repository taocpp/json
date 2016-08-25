// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <iostream>

#include <tao/json.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
        const value a = { { "a", { { "foo", 1 } } } };
        const value q = { { "q", { { "bar", 2 } } } };

        TEST_ASSERT( patch( {}, value::array( {} ) ) == null );
        TEST_ASSERT( patch( a, value::array( {} ) ) == a );
        TEST_ASSERT( patch( q, value::array( {} ) ) == q );

        TEST_ASSERT( patch( {}, value::array( { { { "op", "test" }, { "path", "" }, { "value", null } } } ) ) == null );
        TEST_THROWS( patch( {}, value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "test" }, { "path", "/a" }, { "value", { { "foo", 1 } } } } } ) ) == a );
        TEST_ASSERT( patch( a, value::array( { { { "op", "test" }, { "path", "/a/foo" }, { "value", 1 } } } ) ) == a );

        TEST_THROWS( patch( a, value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "test" }, { "path", "/a" }, { "value", 42 } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == value( empty_object ) ); // TODO: allow comparison against empty_object directly
        TEST_ASSERT( patch( a, value::array( { { { "op", "remove" }, { "path", "/a/foo" } } } ) ) == value( { { "a", empty_object } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "remove" }, { "path", "/q" } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 } } }, { "b", 42 } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 }, { "b", 42 } } } } ) );
        TEST_THROWS( patch( q, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );

        // TODO: Way more tests...
      }

   } // json

} // tao

#include "main.hh"
