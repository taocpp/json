// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/patch.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      data cpatch( const data& v, const data& patch )
      {
         return json::patch( v, patch );
      }

      void unit_test()
      {
         const custom_value n = null;
         const custom_value one = 1u;
         const custom_value two = 2;
         const custom_value ea = empty_array;
         const custom_value eo = empty_object;

         const custom_value a = { { "a", { { "foo", one } } } };
         const custom_value b = { { "b", custom_value::array( { one, two, 3, 4 } ) } };

         const custom_value q = { { "q", { { "bar", two } } } };

         TEST_ASSERT( patch( n, ea ) == n );
         TEST_ASSERT( patch( ea, ea ) == ea );
         TEST_ASSERT( patch( eo, ea ) == eo );
         TEST_ASSERT( patch( a, ea ) == a );
         TEST_ASSERT( patch( b, ea ) == b );
         TEST_ASSERT( patch( q, ea ) == q );

         TEST_THROWS( patch( n, custom_value::array( { { { "op", "foo" }, { "path", "" } } } ) ) );

         TEST_ASSERT( patch( n, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", null } } } ) ) == null );
         TEST_THROWS( patch( n, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "test" }, { "path", "/a" }, { "value", { { "foo", 1 } } } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "test" }, { "path", "/a/foo" }, { "value", 1 } } } ) ) == a );

         TEST_THROWS( patch( a, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "test" }, { "path", "/a" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == empty_object );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == eo );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a/foo" } } } ) ) == custom_value( { { "a", empty_object } } ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "remove" }, { "path", "/q" } } } ) ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "remove" }, { "path", "" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == empty_object );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == eo );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/4" } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/a" } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );
         TEST_THROWS( patch( q, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );

         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "add" }, { "path", "/b" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", 42 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 1 }, { "b", 42 } } } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "add" }, { "path", "" }, { "value", 42 } } } ) ) == 42 );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 42, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 42, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 42, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 42, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/-" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 42 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( q, custom_value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "replace" }, { "path", "" }, { "value", 42 } } } ) ) == 42 );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a" }, { "value", 42 } } } ) ) == custom_value( { { "a", 42 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 42 } } } } ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "replace" }, { "path", "/b" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 42, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 42, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 42, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 42 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/-" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
         TEST_THROWS( patch( q, custom_value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) );

         TEST_THROWS( patch( a, custom_value::array( { { { "op", "move" }, { "from", "" }, { "path", "" } } } ) ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "" } } } ) ) == custom_value( { { "foo", 1 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a" } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a/foo" } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/b" } } } ) ) == custom_value( { { "b", { { "foo", 1 } } } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/b" } } } ) ) == custom_value( { { "a", empty_object }, { "b", 1 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 } } ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a/foo" } } } ) ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "move" }, { "from", "/b" }, { "path", "/a" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/0" } } } ) ) == b );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/1" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 1, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 3, 1, 2, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 4, 1 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 4, 3 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/b/-" } } } ) ) == b );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/4" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/-" }, { "path", "/b/-" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 }, { "b", custom_value::array( { 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 3 }, { "b", custom_value::array( { 1, 2, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 4 }, { "b", custom_value::array( { 1, 2, 3 } ) } } ) );

         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "" } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "" } } } ) ) == custom_value( { { "foo", 1 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a" } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/a/foo" } } } ) ) == a );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/b" } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", { { "foo", 1 } } } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/b" } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", 1 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a/foo" } } } ) ) == custom_value( { { "a", { { "foo", { { "foo", 1 } } } } } } ) );
         TEST_ASSERT( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a/bar" } } } ) ) == custom_value( { { "a", { { "foo", 1 }, { "bar", { { "foo", 1 } } } } } } ) );
         TEST_THROWS( patch( a, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/a" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 1, 4 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/4" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 1 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 3, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 3, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 3, 4 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/4" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 3 } ) } } ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/4" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/-" }, { "path", "/b/-" } } } ) ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { custom_value::array( { 1, 2, 3, 4 } ), 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, custom_value::array( { 1, 2, 3, 4 } ), 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, custom_value::array( { 1, 2, 3, 4 } ) } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { { { "b", custom_value::array( { 1, 2, 3, 4 } ) } }, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, { { "b", custom_value::array( { 1, 2, 3, 4 } ) } }, 3, 4 } ) } } ) );
         TEST_ASSERT( patch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, { { "b", custom_value::array( { 1, 2, 3, 4 } ) } } } ) } } ) );

         TEST_ASSERT( cpatch( n, ea ) == n );
         TEST_ASSERT( cpatch( ea, ea ) == ea );
         TEST_ASSERT( cpatch( eo, ea ) == eo );
         TEST_ASSERT( cpatch( a, ea ) == a );
         TEST_ASSERT( cpatch( b, ea ) == b );
         TEST_ASSERT( cpatch( q, ea ) == q );

         TEST_THROWS( cpatch( n, custom_value::array( { { { "op", "foo" }, { "path", "" } } } ) ) );

         TEST_ASSERT( cpatch( n, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", null } } } ) ) == null );
         TEST_THROWS( cpatch( n, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "test" }, { "path", "/a" }, { "value", { { "foo", 1 } } } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "test" }, { "path", "/a/foo" }, { "value", 1 } } } ) ) == a );

         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "test" }, { "path", "/a" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == empty_object );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == eo );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "remove" }, { "path", "/a/foo" } } } ) ) == custom_value( { { "a", empty_object } } ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "remove" }, { "path", "/q" } } } ) ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "remove" }, { "path", "" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == empty_object );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == eo );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/4" } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/b/a" } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );
         TEST_THROWS( cpatch( q, custom_value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );

         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "add" }, { "path", "/b" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", 42 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 1 }, { "b", 42 } } } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "add" }, { "path", "" }, { "value", 42 } } } ) ) == 42 );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 42, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 42, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 42, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 42, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/-" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 42 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "add" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( q, custom_value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );

         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "replace" }, { "path", "" }, { "value", 42 } } } ) ) == 42 );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a" }, { "value", 42 } } } ) ) == custom_value( { { "a", 42 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) == custom_value( { { "a", { { "foo", 42 } } } } ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "replace" }, { "path", "/b" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "replace" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 42, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 42, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 42, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 42 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/-" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "replace" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
         TEST_THROWS( cpatch( q, custom_value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) );

         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "" }, { "path", "" } } } ) ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "" } } } ) ) == custom_value( { { "foo", 1 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a" } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a/foo" } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/b" } } } ) ) == custom_value( { { "b", { { "foo", 1 } } } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/b" } } } ) ) == custom_value( { { "a", empty_object }, { "b", 1 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 } } ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a/foo" } } } ) ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "move" }, { "from", "/b" }, { "path", "/a" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/0" } } } ) ) == b );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/1" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 1, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 3, 1, 2, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 2, 3, 4, 1 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 4, 3 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/b/-" } } } ) ) == b );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/4" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/-" }, { "path", "/b/-" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 }, { "b", custom_value::array( { 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 3 }, { "b", custom_value::array( { 1, 2, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 4 }, { "b", custom_value::array( { 1, 2, 3 } ) } } ) );

         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "" } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "" } } } ) ) == custom_value( { { "foo", 1 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a" } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/a/foo" } } } ) ) == a );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/b" } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", { { "foo", 1 } } } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/b" } } } ) ) == custom_value( { { "a", { { "foo", 1 } } }, { "b", 1 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a/foo" }, { "path", "/a" } } } ) ) == custom_value( { { "a", 1 } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a/foo" } } } ) ) == custom_value( { { "a", { { "foo", { { "foo", 1 } } } } } } ) );
         TEST_ASSERT( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/a" }, { "path", "/a/bar" } } } ) ) == custom_value( { { "a", { { "foo", 1 }, { "bar", { { "foo", 1 } } } } } } ) );
         TEST_THROWS( cpatch( a, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/a" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 1, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 1, 4 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/4" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/0" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 1 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { 3, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/1" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 3, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/3" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 3, 4 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/4" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/2" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, 3 } ) } } ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/4" }, { "path", "/b/-" } } } ) ) );
         TEST_THROWS( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b/-" }, { "path", "/b/-" } } } ) ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { custom_value::array( { 1, 2, 3, 4 } ), 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, custom_value::array( { 1, 2, 3, 4 } ), 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "/b" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, custom_value::array( { 1, 2, 3, 4 } ) } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/0" } } } ) ) == custom_value( { { "b", custom_value::array( { { { "b", custom_value::array( { 1, 2, 3, 4 } ) } }, 1, 2, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/2" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, { { "b", custom_value::array( { 1, 2, 3, 4 } ) } }, 3, 4 } ) } } ) );
         TEST_ASSERT( cpatch( b, custom_value::array( { { { "op", "copy" }, { "from", "" }, { "path", "/b/-" } } } ) ) == custom_value( { { "b", custom_value::array( { 1, 2, 3, 4, { { "b", custom_value::array( { 1, 2, 3, 4 } ) } } } ) } } ) );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
