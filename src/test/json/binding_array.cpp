// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/binding.hpp>

namespace tao
{
   namespace json
   {
      using type_1 = std::pair< int, std::string >;  // traits< std::pair< ... > > is implemented with binding::array and binding::element.
      using type_2 = std::tuple< int, std::string, bool >;  // traits< std::tuple< ... > > is implemented with binding::array and binding::element2.

      class type_3
      {
      public:
         int get_int() const noexcept
         {
            return 7;
         }

         const std::string& get_string() const noexcept
         {
            return m_string;
         }

      private:
         std::string m_string = "dangerous";
      };

      template<>
      struct traits< type_3 >
         : binding::array< TAO_JSON_BIND_ELEMENT( &type_3::get_int ),
                           TAO_JSON_BIND_ELEMENT( &type_3::get_string ) >
      {
      };

      struct type_4
      {
         int i = 0;
      };

      template<>
      struct traits< type_4 >
         : binding::array< TAO_JSON_BIND_ELEMENT( &type_4::i ),
                           binding::element_b< true >,
                           binding::element_u< 90 >,
                           binding::element_i< -5 >,
                           binding::element_s< json_pegtl::string< 'a', 'b', 'c' > > >
      {
      };

      void unit_test()
      {
         {
            const value v = value::array( { 142, "hallo" } );
            const auto a = v.as< type_1 >();
            TEST_ASSERT( a.first == 142 );
            TEST_ASSERT( a.second == "hallo" );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142, "hallo", 143 } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142 } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { "ciao", "hallo" } );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { 142, 143 } );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { 142, "hallo", true } );
            const auto a = v.as< type_2 >();
            TEST_ASSERT( std::get< 0 >( a ) == 142 );
            TEST_ASSERT( std::get< 1 >( a ) == "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) == true );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { "ciao", "hallo", true } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142, "hallo" } );
            type_1 a;
            TEST_ASSERT( a.first != 142 );
            TEST_ASSERT( a.second != "hallo" );
            v.to( a );
            TEST_ASSERT( a.first == 142 );
            TEST_ASSERT( a.second == "hallo" );
            type_2 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, "hallo", true } );
            type_2 a;
            TEST_ASSERT( std::get< 0 >( a ) != 142 );
            TEST_ASSERT( std::get< 1 >( a ) != "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) != true );
            v.to( a );
            TEST_ASSERT( std::get< 0 >( a ) == 142 );
            TEST_ASSERT( std::get< 1 >( a ) == "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) == true );
            type_1 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, 143 } );
            type_1 a;
            TEST_THROWS( v.to( a ) );
            type_2 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, "hallo", 144 } );
            type_2 a;
            TEST_THROWS( v.to( a ) );
            type_1 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const type_1 a = { 19, "ciao" };
            const value v = a;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 2 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 19 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< std::string >() == "ciao" );

            TEST_ASSERT( a == v );
            TEST_ASSERT( v == a );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != a ) );
            const type_1 b = { 20, "ciao" };
            const type_1 c = { 19, "hola" };
            TEST_ASSERT( b != c );
            TEST_ASSERT( c != b );
            TEST_ASSERT( a != b );
            TEST_ASSERT( b != a );
            TEST_ASSERT( a != c );
            TEST_ASSERT( c != a );
            TEST_ASSERT( !( b == c ) );
            TEST_ASSERT( !( c == b ) );
            TEST_ASSERT( !( a == b ) );
            TEST_ASSERT( !( b == a ) );
            TEST_ASSERT( !( a == c ) );
            TEST_ASSERT( !( c == a ) );

            const value w = produce::to_value( a );
            TEST_ASSERT( w == v );
            TEST_ASSERT( w == a );
            TEST_ASSERT( a == v );
         }
         {
            const type_2 a = { 20, "hola", false };
            const value v = a;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 3 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 20 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< std::string >() == "hola" );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].is_boolean() );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].as< bool >() == false );

            TEST_ASSERT( a == v );
            TEST_ASSERT( v == a );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != a ) );
            const type_2 b = { 19, "hola", false };
            const type_2 c = { 20, "hola", true };
            TEST_ASSERT( b != c );
            TEST_ASSERT( c != b );
            TEST_ASSERT( a != b );
            TEST_ASSERT( b != a );
            TEST_ASSERT( a != c );
            TEST_ASSERT( c != a );
            TEST_ASSERT( !( b == c ) );
            TEST_ASSERT( !( c == b ) );
            TEST_ASSERT( !( a == b ) );
            TEST_ASSERT( !( b == a ) );
            TEST_ASSERT( !( a == c ) );
            TEST_ASSERT( !( c == a ) );

            const value w = produce::to_value( a );
            TEST_ASSERT( w == v );
            TEST_ASSERT( w == a );
            TEST_ASSERT( a == v );
         }
         {
            parts_parser p( "[1,\"foo\"]", __FUNCTION__ );
            const auto a = consume< type_1 >( p );
            TEST_ASSERT( a.first == 1 );
            TEST_ASSERT( a.second == "foo" );
         }
         {
            parts_parser p( "1", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "[1]", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "[1,\"foo\",2]", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "\"foo\"", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "{}", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            type_3 a;
            const value v = a;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 2 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 7 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< std::string >() == "dangerous" );
            const auto w = produce::to_value( a );
            TEST_ASSERT( v == w );
            TEST_ASSERT( a == v );
            TEST_ASSERT( w == a );
         }
         {
            const auto v = value::array( { 1, true, 90, -5, "abc" } );
            const auto a = v.as< type_4 >();
            TEST_ASSERT( a.i == 1 );
            type_4 b;
            TEST_ASSERT( b.i == 0 );
            v.to( b );
            TEST_ASSERT( b.i == 1 );
            TEST_ASSERT( a == v );
            TEST_ASSERT( v == b );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != b ) );
         }
         {
            const auto v = value::array( { 200, true, 90, -5, "abc" } );
            const auto a = v.as< type_4 >();
            TEST_ASSERT( a.i == 200 );
            type_4 b;
            TEST_ASSERT( b.i == 0 );
            v.to( b );
            TEST_ASSERT( b.i == 200 );
            TEST_ASSERT( a == v );
            TEST_ASSERT( v == b );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != b ) );
         }
         {
            const auto v = value::array( { 1, false, 90, -5, "abc" } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            const auto v = value::array( { 1, true, 91, -5, "abc" } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            const auto v = value::array( { 1, true, 90, -6, "abc" } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            const auto v = value::array( { 1, true, 90, -5, "abcd" } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            const auto v = value::array( { 1, 1, 90, -5, "abc" } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            const auto v = value::array( { 1, true, 90, -5, "abc", 2 } );
            TEST_THROWS( v.as< type_4 >() );
            type_4 a;
            TEST_THROWS( v.to( a ) );
         }
         {
            type_4 a;
            a.i = 80;
            const value v = a;
            TEST_ASSERT( a == v );
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 5 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 80 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_boolean() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< bool >() == true );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].as< int >() == 90 );
            TEST_ASSERT( v.unsafe_get_array()[ 3 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 3 ].as< int >() == -5 );
            TEST_ASSERT( v.unsafe_get_array()[ 4 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 4 ].as< std::string >() == "abc" );
            const value w = produce::to_value( a );
            TEST_ASSERT( w == a );
            TEST_ASSERT( v == w );
         }
         {
            parts_parser p( "[7,true,90,-5,\"abc\"]", __FUNCTION__ );
            const auto a = consume< type_4 >( p );
            TEST_ASSERT( a.i == 7 );
         }
         {
            parts_parser p( "[7,true,90,-5,\"abc\",null]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,true,90,-5]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,false,90,-5,\"abc\"]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,true,91,-5,\"abc\"]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,true,90,-6,\"abc\"]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,true,90,-5,\"abcd\"]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[7,true,90,-5,90]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "[]", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
         {
            parts_parser p( "null", __FUNCTION__ );
            TEST_THROWS( consume< type_4 >( p ) );
         }
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
