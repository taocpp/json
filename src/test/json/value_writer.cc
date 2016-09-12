// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/internal/value_writer.hh>

namespace tao
{
   namespace json
   {
      void test( const std::string & v )
      {
         std::ostringstream oss;
         tao_json_pegtl::input input( 1, 0, v.data(), v.data() + v.size(), __PRETTY_FUNCTION__ );
         tao_json_pegtl::parse_input< internal::grammar, internal::action, internal::control >( input, internal::value_writer( oss ) );
         TEST_ASSERT( oss.str() == v );
      }

      void unit_test()
      {
         test( "[null,true,false,42,43.0,\"foo\",[1,2,3],{\"a\":\"b\",\"c\":\"d\"}]" );
      }

   } // json

} // tao

#include "main.hh"

