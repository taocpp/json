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
         // do not throw
         "//"_json_pointer;
         "/-"_json_pointer;

         // throw
         TEST_THROWS( "x"_json_pointer );
         TEST_THROWS( "/~"_json_pointer );
         TEST_THROWS( "/~2"_json_pointer );
         TEST_THROWS( "/~x"_json_pointer );

         // create test vector
         const std::string ezp( "o\0p", 3 );
         const std::string ezq( "o\0q", 3 );
         const value v {
            { "foo", value::array( { "bar", "baz" } ) },
            { "", 0 },
            { "a/b", 1 },
            { "c%d", 2 },
            { "e^f", 3 },
            { "g|h", 4 },
            { "i\\j", 5 },
            { "k\"l", 6 },
            { " ", 7 },
            { "m~n", 8 },
            { ezp, 9 },
            { ezq, 10 }
         };

         // check access
         TEST_ASSERT( v.at( ""_json_pointer ) == v );
         TEST_ASSERT( v.at( "/foo"_json_pointer ) == value::array( { "bar", "baz" } ) );
         TEST_ASSERT( v.at( "/foo/0"_json_pointer ) == "bar" );
         TEST_ASSERT( v.at( "/"_json_pointer ) == 0 );
         TEST_ASSERT( v.at( "/a~1b"_json_pointer ) == 1 );
         TEST_ASSERT( v.at( "/c%d"_json_pointer ) == 2 );
         TEST_ASSERT( v.at( "/e^f"_json_pointer ) == 3 );
         TEST_ASSERT( v.at( "/g|h"_json_pointer ) == 4 );
         TEST_ASSERT( v.at( "/i\\j"_json_pointer ) == 5 );
         TEST_ASSERT( v.at( "/k\"l"_json_pointer ) == 6 );
         TEST_ASSERT( v.at( "/ "_json_pointer ) == 7 );
         TEST_ASSERT( v.at( "/m~0n"_json_pointer ) == 8 );
         TEST_ASSERT( v.at( "/o\0p"_json_pointer ) == 9 );
         TEST_ASSERT( v.at( "/o\0q"_json_pointer ) == 10 );

         TEST_THROWS( v.at( "/o\0r"_json_pointer ) );

         TEST_THROWS( v.at( "/bar"_json_pointer ) );
         TEST_THROWS( v.at( "/foo/bar"_json_pointer ) );
         TEST_THROWS( v.at( "//"_json_pointer ) );
         TEST_THROWS( v.at( "/-"_json_pointer ) );
      }

   } // json

} // tao

#include "main.hh"
