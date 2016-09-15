// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         value v { { "foo", 1 } };
         const value cv { { "fuu", 2 } };

         const value v2 { { "bar", &v }, { "baz", nullptr } };  // Store a non-owning const pointer to v.
         const value cv2 { { "baz", &cv } };  // Store a non-owning const pointer to cv.

         TEST_ASSERT( to_string( v ) == "{\"foo\":1}" );
         TEST_ASSERT( to_string( cv ) == "{\"fuu\":2}" );

         TEST_ASSERT( to_string( v2 ) == "{\"bar\":{\"foo\":1},\"baz\":null}" );
         TEST_ASSERT( to_string( v2.at( "bar" ).type() ) == std::string( "pointer" ) );
         TEST_ASSERT( to_string( v2.at( "baz" ).type() ) == std::string( "pointer" ) );
         TEST_ASSERT( v2.at( "baz" ).empty() );

         TEST_ASSERT( to_string( cv2 ) == "{\"baz\":{\"fuu\":2}}" );
         TEST_ASSERT( to_string( cv2.at( "baz" ).type() ) == std::string( "pointer" ) );
         TEST_ASSERT( !cv2.at( "baz" ).empty() );
      }

   } // json

} // tao

#include "main.hh"
