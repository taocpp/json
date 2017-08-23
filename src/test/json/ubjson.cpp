// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"
#include "test_unhex.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/to_string.hpp>

#include <tao/json/ubjson/from_string.hpp>
#include <tao/json/ubjson/to_string.hpp>

namespace tao
{
   namespace json
   {
      void ubjson_encode( const std::string& text, const std::string& data )
      {
         TEST_ASSERT( ubjson::to_string( from_string( text ) ) == test_unhex( data ) );
      }

      void ubjson_decode( const std::string& data, const std::string& text )
      {
         TEST_ASSERT( to_string( ubjson::from_string( test_unhex( data ) ) ) == to_string( from_string( text ) ) );
      }

      void unit_test()
      {
         ubjson_decode( "5a", "null" );

         ubjson_encode( "null", "5a" );
         ubjson_encode( "true", "54" );
         ubjson_encode( "false", "46" );
         ubjson_encode( "\"a\"", "4361" );
         ubjson_encode( "\"aa\"", "5355026161" );
         ubjson_encode( "[]", "5b235500" );
         ubjson_encode( "{}", "7b235500" );
         ubjson_encode( "[null]", "5b2355015a" );
         ubjson_encode( "{\"a\":true,\"b\":false}", "7b2355025501615455016246" );
         ubjson_encode( "\"000\"", "535503303030" );
         ubjson_encode( "0", "5500" );
         ubjson_encode( "1", "5501" );
         ubjson_encode( "255", "55ff" );
         ubjson_encode( "256", "490100" );
         ubjson_encode( "32767", "497fff" );
         ubjson_encode( "32768", "6c00008000" );
         ubjson_encode( "2147483647", "6c7fffffff" );
         ubjson_encode( "2147483648", "4c0000000080000000" );
         ubjson_encode( "9223372036854775807", "4c7fffffffffffffff" );
         ubjson_encode( "9223372036854775808", "48551339323233333732303336383534373735383038" );
         ubjson_encode( "-1", "69ff" );
         ubjson_encode( "-128", "6980" );
         ubjson_encode( "-129", "49ff7f" );
         ubjson_encode( "-32768", "498000" );
         ubjson_encode( "-32769", "6cffff7fff" );
         ubjson_encode( "-2147483648", "6c80000000" );
         ubjson_encode( "-2147483649", "4cffffffff7fffffff" );
         ubjson_encode( "-9223372036854775808", "4c8000000000000000" );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
