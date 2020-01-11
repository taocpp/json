// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/value.hpp>

namespace tao::json
{
   template< class... Fs > struct overloaded : Fs... { using Fs::operator()...; };
   template< class... Fs > overloaded( Fs... ) -> overloaded< Fs... >;

   void unit_test()
   {
      int i = 0;
      value v = 4;

      v.visit( overloaded
               {
                  [&]( const std::int64_t a ) { i = a; },
                  []( auto&& t ){ std::cerr << typeid( t ).name() << std::endl; }
               } );

      TEST_ASSERT( i == 4 );

      std::string s;
      const value w = "dalek";

      w.visit( overloaded
               {
                  [&]( const std::string& a ) { s = a; },
                  []( auto&& t ){ std::cerr << typeid( t ).name() << std::endl; }
               } );

      TEST_ASSERT( s == "dalek" );
   }

}  // namespace tao::json

#include "main.hpp"
