// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SRC_TEST_MAIN_HPP
#define TAOCPP_JSON_INCLUDE_SRC_TEST_MAIN_HPP

#include <cstdlib>

int main( int, char** argv )
{
   tao::json::unit_test();

   if( tao::json::failed ) {
      std::cerr << "json: unit test " << argv[ 0 ] << " failed " << tao::json::failed << std::endl;
   }
   return ( tao::json::failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif
