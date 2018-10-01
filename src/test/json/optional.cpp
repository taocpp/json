// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>

#include "test.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         {
            const tao::optional< std::string > f;
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_null() );
            const auto g = v.as< tao::optional< std::string > >();
            TEST_ASSERT( g == f );
         }
         {
            const tao::optional< std::string > f( "hallo" );
            value v = f;
            TEST_ASSERT( v == f );
            TEST_ASSERT( v.is_string() );
            TEST_ASSERT( v == "hallo" );
            TEST_ASSERT( v.get_string() == "hallo" );
            const auto g = v.as< tao::optional< std::string > >();
            TEST_ASSERT( *g == *f );
         }
         {
            const tao::optional< int > i;
            const value v = null;
            TEST_ASSERT( i == v );
         }
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
