// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/type.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( sizeof( type ) == 1 );

         TEST_ASSERT( to_string( type::NULL_ ) == std::string( "null" ) );
         TEST_ASSERT( to_string( type::BOOLEAN ) == std::string( "boolean" ) );
         TEST_ASSERT( to_string( type::SIGNED ) == std::string( "signed" ) );
         TEST_ASSERT( to_string( type::UNSIGNED ) == std::string( "unsigned" ) );
         TEST_ASSERT( to_string( type::DOUBLE ) == std::string( "double" ) );

         TEST_ASSERT( to_string( type::STRING ) == std::string( "string" ) );
         TEST_ASSERT( to_string( type::ARRAY ) == std::string( "array" ) );
         TEST_ASSERT( to_string( type::OBJECT ) == std::string( "object" ) );

         TEST_ASSERT( to_string( type::RAW_PTR ) == std::string( "raw_ptr" ) );

         TEST_ASSERT( to_string( type( 42 ) ) == std::string( "unknown" ) );

         TEST_ASSERT( needs_destroy( type::NULL_ ) == false );
         TEST_ASSERT( needs_destroy( type::BOOLEAN ) == false );
         TEST_ASSERT( needs_destroy( type::SIGNED ) == false );
         TEST_ASSERT( needs_destroy( type::UNSIGNED ) == false );
         TEST_ASSERT( needs_destroy( type::DOUBLE ) == false );

         TEST_ASSERT( needs_destroy( type::STRING ) == true );
         TEST_ASSERT( needs_destroy( type::ARRAY ) == true );
         TEST_ASSERT( needs_destroy( type::OBJECT ) == true );

         TEST_ASSERT( needs_destroy( type::RAW_PTR ) == false );
      }

   } // json

} // tao

#include "main.hh"
