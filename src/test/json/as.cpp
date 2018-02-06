// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/stream.hpp>
#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      struct one
      {
         int i;
      };

      struct two
      {
         int i;
      };

      struct both
      {
         int i;
      };

      template<>
      struct traits< one >
      {
         template< typename T >
         static one as( const T& /*unused*/ )
         {
            return one{ 1 };
         }
      };

      template<>
      struct traits< two >
      {
         template< typename T >
         static void as( const T& /*unused*/, two& t )
         {
            t = two{ 2 };
         }
      };

      template<>
      struct traits< both >
      {
         template< typename T >
         static both as( const T& /*unused*/ )
         {
            return both{ 3 };
         }

         template< typename T >
         static void as( const T& /*unused*/, both& t )
         {
            t = both{ 4 };
         }
      };

      void unit_test()
      {
         value v = 1;

         TEST_ASSERT( v.as< one >().i == 1 );
         {
            one i = { 0 };
            v.as( i );
            TEST_ASSERT( i.i == 1 );
         }
         TEST_ASSERT( v.as< two >().i == 2 );
         {
            two i = { 0 };
            v.as( i );
            TEST_ASSERT( i.i == 2 );
         }
         TEST_ASSERT( v.as< both >().i == 3 );
         {
            both i = { 0 };
            v.as( i );
            TEST_ASSERT( i.i == 4 );
         }
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
