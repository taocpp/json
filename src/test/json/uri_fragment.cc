// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/value.hh>
#include <tao/json/pointer.hh>
#include <tao/json/internal/uri_fragment.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#" ) == ""_json_pointer );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/a~1b" ) == "/a~1b"_json_pointer );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/c%25d" ) == "/c%d"_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/c%d" ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/e%5Ef" ) == "/e^f"_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/e^f" ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/g%7Ch" ) == "/g|h"_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/g|h" ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/i%5Cj" ) == "/i\\j"_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/i\\j" ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/k%22l" ) == "/k\"l"_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/k\"l" ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/%20" ) == "/ "_json_pointer );
         TEST_THROWS( internal::uri_fragment_to_pointer( "#/ " ) );
         TEST_ASSERT( internal::uri_fragment_to_pointer( "#/m~0n" ) == "/m~0n"_json_pointer );
      }

   } // json

} // tao

#include "main.hh"
