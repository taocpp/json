// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SRC_TEST_UNHEX_HPP
#define TAOCPP_JSON_INCLUDE_SRC_TEST_UNHEX_HPP

#include <cassert>
#include <string>

namespace tao
{
   namespace json
   {
      inline char test_unhex( const char c )
      {
         if( ( '0' <= c ) && ( c <= '9' ) ) {
            return c - '0';
         }
         if( ( 'a' <= c ) && ( c <= 'f' ) ) {
            return c - 'a' + 10;
         }
         if( ( 'A' <= c ) && ( c <= 'F' ) ) {
            return c - 'A' + 10;
         }
         // LCOV_EXCL_START
         assert( false );
         return 0;
         // LCOV_EXCL_STOP
      }

      inline std::string test_unhex( const std::string& data )
      {
         assert( !( data.size() & 1 ) );
         std::string result;
         result.reserve( data.size() / 2 );
         for( std::string::size_type i = 0; i < data.size(); i += 2 ) {
            result += ( test_unhex( data[ i ] ) << 4 ) + test_unhex( data[ i + 1 ] );
         }
         return result;
      }

   }  // namespace json

}  // namespace tao

#endif
