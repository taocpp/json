// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <tao/json.hpp>
#include <tao/json/external/pegtl/contrib/json.hpp>

#include "bench_mark.hpp"

// clang-format off

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      const auto r = tao::bench::mark( "pegtl", argv[ i ], [&]() {
         tao::json_pegtl::file_input< tao::json_pegtl::tracking_mode::LAZY > in( argv[ i ] );
         tao::json_pegtl::parse< tao::json_pegtl::must< tao::json_pegtl::json::text, tao::json_pegtl::eof > >( in );
      } );
      tao::bench::mark( "json", argv[ i ], [&]() {
         tao::json::events::discard consumer;
         tao::json::events::parse_file( consumer, argv[ i ] );
      }, r );
   }
   return 0;
}
