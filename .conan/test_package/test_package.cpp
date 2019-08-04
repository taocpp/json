// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <cstdlib>
#include <iostream>

#include <tao/json.hpp>

int main()
{
   tao::json::value v;
   v.try_emplace( "a", 1.0 );
   v.try_emplace( "b", 2.0 );
   tao::json::to_stream( std::cout, v );
   return EXIT_SUCCESS;
}
