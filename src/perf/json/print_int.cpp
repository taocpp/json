// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <tao/json.hpp>

#include "bench_mark.hpp"

namespace df
{
   namespace
   {
      inline char* lt100( const std::uint32_t n, char* p ) noexcept
      {
         assert( n < 100 );
         const auto h = n / 10;
         *p++ = '0' + h;
         *p++ = '0' + ( n - 10 * h );
         return p;
      }

      inline char* lt10000( const std::uint32_t n, char* p ) noexcept
      {
         assert( n < 10000 );
         const auto h = n / 100;
         p = lt100( h, p );
         return lt100( n - 100 * h, p );
      }

      inline char* lt100000000( const std::uint32_t n, char* p ) noexcept
      {
         assert( n < 100000000 );
         const auto h = n / 10000;
         p = lt10000( h, p );
         return lt10000( n - 10000 * h, p );
      }

   }  // namespace

   inline char* u32toa( const std::uint32_t n, char* p ) noexcept
   {
      if( n < 100 ) {
         if( n < 10 ) {
            *p++ = '0' + n;
            return p;
         }
         return lt100( n, p );
      }
      if( n < 10000 ) {
         const auto h = n / 100;
         p = u32toa( h, p );
         return lt100( n - 100 * h, p );
      }
      const auto h = n / 10000;
      p = u32toa( h, p );
      return lt10000( n - 10000 * h, p );
   }

   inline char* i32toa( const std::int32_t i, char* p ) noexcept
   {
      const std::uint32_t u = i < 0 ? ( *p++ = '-', 0 - std::uint32_t( i ) ) : i;
      return u32toa( u, p );
   }

   inline char* u64toa( const std::uint64_t n, char* p ) noexcept
   {
      if( std::uint32_t( n >> 32 ) == 0 ) {
         return u32toa( n, p );
      }

      std::uint64_t a = n / 100000000;
      if( std::uint32_t( a >> 32 ) == 0 ) {
         p = u32toa( a, p );
      }
      else {
         const auto h = a / 100000000;
         p = u32toa( h, p );
         p = lt100000000( a - 100000000 * h, p );
      }
      return lt100000000( n - 100000000 * a, p );
   }

   inline char* i64toa( const std::int64_t i, char* p ) noexcept
   {
      const std::uint64_t u = i < 0 ? ( *p++ = '-', 0 - std::uint64_t( i ) ) : i;
      return u64toa( u, p );
   }

}  // namespace df

int main( int argc, char** /*unused*/ )
{
   char b[ 28 ];

   const unsigned f = ( argc == 2 ) ? 0 : 1;

   std::cout << std::string( b, df::u32toa( f * 123456789, b ) - b ) << std::endl;
   std::cout << std::string( b, df::u32toa( f * 1234567890, b ) - b ) << std::endl;
   std::cout << std::string( b, df::u64toa( f * 12345678987654321UL, b ) - b ) << std::endl;
   std::cout << std::string( b, df::u64toa( f * 9223372036854775807UL, b ) - b ) << std::endl;

   // tao::bench::mark( "pd", "0", [&]() { tao::json::jeaiii::u32toa( 0, b ); } );
   // tao::bench::mark( "pd", "1", [&]() { tao::json::jeaiii::u32toa( 1, b ); } );
   tao::bench::mark( "pd", "123456789", [&]() { tao::json::jeaiii::u32toa( f * 123456789, b ); } );
   tao::bench::mark( "pd", "1234567890", [&]() { tao::json::jeaiii::u32toa( f * 1234567890, b ); } );
   tao::bench::mark( "pd", "12345678987654321", [&]() { tao::json::jeaiii::u64toa( f * 12345678987654321L, b ); } );
   tao::bench::mark( "pd", "9223372036854775807", [&]() { tao::json::jeaiii::u64toa( f * 9223372036854775807L, b ); } );

   // tao::bench::mark( "pd", "0", [&]() { df::u32toa( 0, b ); } );
   // tao::bench::mark( "pd", "1", [&]() { df::u32toa( 1, b ); } );
   tao::bench::mark( "pd", "123456789", [&]() { df::u32toa( f * 123456789, b ); } );
   tao::bench::mark( "pd", "1234567890", [&]() { df::u32toa( f * 1234567890, b ); } );
   tao::bench::mark( "pd", "12345678987654321", [&]() { df::u64toa( f * 12345678987654321L, b ); } );
   tao::bench::mark( "pd", "9223372036854775807", [&]() { df::u64toa( f * 9223372036854775807L, b ); } );

   return 0;
}
