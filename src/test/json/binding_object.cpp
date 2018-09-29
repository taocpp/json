// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      using type_0 = std::pair< int, int >;

      template<>
      struct traits< type_0 >
         : binding::array< TAO_JSON_BIND_ELEMENT( &type_0::first ),
                           TAO_JSON_BIND_ELEMENT( &type_0::second ) >
      {
         TAO_JSON_DEFAULT_KEY( "z" );
      };

      struct type_1
      {
         int i = 8000;
         std::string s;
         tao::optional< bool > b;
         tao::optional< double > d;
         type_0 z;
      };

      template<>
      struct traits< type_1 >
         : binding::object< TAO_JSON_BIND_REQUIRED( "i", &type_1::i ),
                            TAO_JSON_BIND_OPTIONAL( "s", &type_1::s ),
                            TAO_JSON_BIND_REQUIRED( "b", &type_1::b ),
                            TAO_JSON_BIND_OPTIONAL( "d", &type_1::d ),
                            TAO_JSON_BIND_REQUIRED1( &type_1::z ) >
      {
      };

      void unit_test_1()
      {
         const value v = {
            { "i", 42 },
            { "s", "foo" },
            { "b", true },
            { "d", 43.1 },
            { "z", value::array( { 5, 6 } ) }
         };
         const auto a = v.as< type_1 >();
         TEST_ASSERT( a.i == 42 );
         TEST_ASSERT( a.s == "foo" );
         TEST_ASSERT( a.b && ( *a.b == true ) );
         TEST_ASSERT( a.d && ( *a.d == 43.1 ) );
         TEST_ASSERT( a.z.first == 5 );
         TEST_ASSERT( a.z.second == 6 );
         TEST_ASSERT( a == v );
         TEST_ASSERT( v == a );
         TEST_ASSERT( !( a != v ) );
         TEST_ASSERT( !( v != a ) );
         type_1 b;
         v.to( b );
         TEST_ASSERT( b.i == 42 );
         TEST_ASSERT( b.s == "foo" );
         TEST_ASSERT( b.b && ( *a.b == true ) );
         TEST_ASSERT( b.d && ( *a.d == 43.1 ) );
         TEST_ASSERT( b.z.first == 5 );
         TEST_ASSERT( b.z.second == 6 );
      }

      void unit_test_2()
      {
         const value v = {
            { "i", 42 },
            { "b", true },
            { "z", value::array( { 5, 6 } ) }
         };
         const auto a = v.as< type_1 >();
         TEST_ASSERT( a.i == 42 );
         TEST_ASSERT( a.s.empty() );
         TEST_ASSERT( a.b && ( *a.b == true ) );
         TEST_ASSERT( !a.d );
         TEST_ASSERT( a.z.first == 5 );
         TEST_ASSERT( a.z.second == 6 );
         value w = v;
         w[ "s" ] = "";
         w[ "d" ] = null;  // Must compare equal without this entry when using for_nothing_value::SUPPRESS.
         TEST_ASSERT( a == w );
         TEST_ASSERT( w == a );
         TEST_ASSERT( !( a != w ) );
         TEST_ASSERT( !( w != a ) );
         type_1 b;
         b.s = "hallo";
         v.to( b );
         TEST_ASSERT( b.i == 42 );
         TEST_ASSERT( b.s == "hallo" );
         TEST_ASSERT( b.b && ( *a.b == true ) );
         TEST_ASSERT( !b.d );
         TEST_ASSERT( b.z.first == 5 );
         TEST_ASSERT( b.z.second == 6 );
      }

      void unit_test_3()
      {
         type_1 a;
         a.i = 90;
         a.s = "bar";
         a.b = false;
         a.d = 44.2;
         a.z.first = 8;
         a.z.second = 9;
         const value v = a;
         TEST_ASSERT( v.is_object() );
         TEST_ASSERT( v.at( "i" ).is_integer() );
         TEST_ASSERT( v.as< int >( "i" ) == 90 );
         TEST_ASSERT( v.at( "s" ).is_string_type() );
         TEST_ASSERT( v.as< std::string >( "s" ) == "bar" );
         TEST_ASSERT( v.at( "b" ).is_boolean() );
         TEST_ASSERT( v.as< bool >( "b" ) == false );
         TEST_ASSERT( v.at( "d" ).is_double() );
         TEST_ASSERT( v.as< double >( "d" ) == 44.2 );
         TEST_ASSERT( v.at( "z" ).is_array() );
         TEST_ASSERT( v.at( "z" ).unsafe_get_array().size() == 2 );
         TEST_ASSERT( v.at( "z" ).unsafe_get_array()[ 0 ].as< int >() == 8 );
         TEST_ASSERT( v.at( "z" ).unsafe_get_array()[ 1 ].as< int >() == 9 );
         const value w = produce::to_value( a );
         TEST_ASSERT( v == w );
      }

      struct type_3
      {
         int i = 3;
      };

      struct type_4
         : public type_3
      {
         int j = 4;
      };

      template<>
      struct traits< type_3 >
         : public binding::object< TAO_JSON_BIND_REQUIRED( "i", &type_3::i ) >
      {
      };

      template<>
      struct traits< type_4 >
         : public binding::object< binding::inherit< traits< type_3 > >,
                                   TAO_JSON_BIND_REQUIRED( "j", &type_4::j ) >
      {
      };

      // void unit_test_4()
      // {
      //    const value v = from_string( "{ \"i\": 5, \"j\": 6 }" );
      //    const auto a = as< type_4 >( v );
      //    TEST_ASSERT( a.i == 5 );
      //    TEST_ASSERT( a.j == 6 );
      // }

      // TODO: Test consume...
      // TODO: Test additional keys in input.
      // TODO: Test with different for_unknown_key.
      // TODO: Test with different for_nothing_value (incl. consistency of size to consumer).
      // TODO: Test with binding::member_x constants.

      void unit_test()
      {
         unit_test_1();
         unit_test_2();
         unit_test_3();
         //         unit_test_4();  // TODO: Fix the type mismatch in the functions for the maps used in binding::object (as opposed to binding::array where they are called directly).
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
