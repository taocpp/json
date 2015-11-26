// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SRC_TEST_MAIN_HH
#define TAOCPP_JSON_INCLUDE_SRC_TEST_MAIN_HH

#include <cstdlib>

int main( int, char ** argv )
{
   tao::json::unit_test();

   if ( tao::json::failed ) {
      std::cerr << "json: unit test " << argv[ 0 ] << " failed " << tao::json::failed << std::endl;
   }
   return ( tao::json::failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif
