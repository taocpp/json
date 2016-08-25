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
        const value b = { { "b", value::array( { 1, 2, 3, 4, 5 } ) } };
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
        TEST_THROWS( patch( a, value::array( { { { "op", "remove" }, { "path", "" } } } ) ) ); // TODO: clarify correctness
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == value( empty_object ) ); // TODO: allow comparison against empty_object directly
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/0" } } } ) ) == value( { { "b", value::array( { 2, 3, 4, 5 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/1" } } } ) ) == value( { { "b", value::array( { 1, 3, 4, 5 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/2" } } } ) ) == value( { { "b", value::array( { 1, 2, 4, 5 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/3" } } } ) ) == value( { { "b", value::array( { 1, 2, 3, 5 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/4" } } } ) ) == value( { { "b", value::array( { 1, 2, 3, 4 } ) } } ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/5" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/-" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/a" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );
        TEST_THROWS( patch( q, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 } } }, { "b", 42 } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 }, { "b", 42 } } } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "add" }, { "path", "" }, { "value", 42 } } } ) ) ); // TODO: clarify correctness
        TEST_THROWS( patch( q, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );

        // TODO: Way more tests...
      }

   } // json

} // tao

#include "main.hh"
