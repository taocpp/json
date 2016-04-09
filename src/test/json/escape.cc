// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         const std::string s1 = "\" \\b\\f\\n\\r\\t\\\\\\\" \"";
         TEST_ASSERT( from_string( s1 ).get_string() == " \b\f\n\r\t\\\" " );
         TEST_ASSERT( to_string( from_string( s1 ) ) == s1 );
         const std::string s2 = "\" \\u007f \"";
         TEST_ASSERT( from_string( s2 ).get_string() == " \x7f " );
         TEST_ASSERT( to_string( from_string( s2 ) ) == s2 );
      }

   } // json

} // tao

#include "main.hh"
