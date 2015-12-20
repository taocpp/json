// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( sizeof( type ) == 1 );

         TEST_ASSERT( to_string( type::NULL_ ) == "null" );
         TEST_ASSERT( to_string( type::BOOL_ ) == "bool" );
         TEST_ASSERT( to_string( type::INT64 ) == "int64" );
         TEST_ASSERT( to_string( type::DOUBLE ) == "double" );

         TEST_ASSERT( to_string( type::STRING ) == "string" );
         TEST_ASSERT( to_string( type::ARRAY ) == "array" );
         TEST_ASSERT( to_string( type::OBJECT ) == "object" );

         TEST_ASSERT( to_string( type( 42 ) ) == "unknown" );

         TEST_ASSERT( needs_destroy( type::NULL_ ) == false );
         TEST_ASSERT( needs_destroy( type::BOOL_ ) == false );
         TEST_ASSERT( needs_destroy( type::INT64 ) == false );
         TEST_ASSERT( needs_destroy( type::DOUBLE ) == false );

         TEST_ASSERT( needs_destroy( type::STRING ) == true );
         TEST_ASSERT( needs_destroy( type::ARRAY ) == true );
         TEST_ASSERT( needs_destroy( type::OBJECT ) == true );
      }

   } // json

} // tao

#include "main.hh"
