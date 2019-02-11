// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/value.hpp>

#include <tao/json/contrib/traits.hpp>

namespace tao
{
   namespace json
   {
      template< typename T >
      struct my_traits
         : public traits< T >
      {
      };

      template<>
      struct my_traits< void >
      {
         template< typename >
         using public_base = internal::empty_base;

         static constexpr const bool enable_implicit_constructor = false;
      };

      type test( const basic_value< my_traits >& v )
      {
         return v.type();
      }

      void unit_test()
      {
         TEST_ASSERT( test( null ) == type::NULL_ );
         TEST_ASSERT( test( empty_string ) == type::STRING );
         TEST_ASSERT( test( empty_binary ) == type::BINARY );
         TEST_ASSERT( test( empty_array ) == type::ARRAY );
         TEST_ASSERT( test( empty_object ) == type::OBJECT );

         basic_value< my_traits > v = null;
         TEST_ASSERT( v.is_null() );
         v = empty_string;
         TEST_ASSERT( v.is_string() );
         v = empty_binary;
         TEST_ASSERT( v.is_binary() );
         v = empty_array;
         TEST_ASSERT( v.is_array() );
         v = empty_object;
         TEST_ASSERT( v.is_object() );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
