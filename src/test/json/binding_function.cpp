// Copyright (c) 2018-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

namespace tao::json
{
   struct type_f
   {
      int private_a1 = 11;
      int private_a2 = 12;

      int private_b1 = 21;
      int private_b2 = 22;
      int private_b3 = 23;
      int private_b4 = 24;

      int private_c1 = 31;
      int private_c2 = 32;
      int private_c3 = 33;
      int private_c4 = 34;
   };

   [[nodiscard]] int get_a1( const type_f& f )
   {
      return f.private_a1 + 100;
   }

   [[nodiscard]] int get_a2( const type_f& f ) noexcept
   {
      return f.private_a2 + 100;
   }

   [[nodiscard]] int get_b1( const type_f& f )
   {
      return f.private_b1 + 200;
   }

   [[nodiscard]] int& set_b1( type_f& f )
   {
      return f.private_b1;
   }

   [[nodiscard]] int get_b2( const type_f& f ) noexcept
   {
      return f.private_b2 + 200;
   }

   [[nodiscard]] int& set_b2( type_f& f )
   {
      return f.private_b2;
   }

   [[nodiscard]] int get_b3( const type_f& f )
   {
      return f.private_b3 + 200;
   }

   [[nodiscard]] int& set_b3( type_f& f ) noexcept
   {
      return f.private_b3;
   }

   [[nodiscard]] int get_b4( const type_f& f ) noexcept
   {
      return f.private_b4 + 200;
   }

   [[nodiscard]] int& set_b4( type_f& f ) noexcept
   {
      return f.private_b4;
   }

   [[nodiscard]] int get_c1( const type_f& f ) noexcept
   {
      return f.private_c1 + 300;
   }

   void set_c1( type_f& f, const int c ) noexcept
   {
      f.private_c1 = c;
   }

   [[nodiscard]] int get_c2( const type_f& f )
   {
      return f.private_c2 + 300;
   }

   void set_c2( type_f& f, const int& c ) noexcept
   {
      f.private_c2 = c;
   }

   [[nodiscard]] int get_c3( const type_f& f ) noexcept
   {
      return f.private_c3 + 300;
   }

   void set_c3( type_f& f, int&& c ) noexcept
   {
      f.private_c3 = c;
   }

   [[nodiscard]] int get_c4( const type_f& f )
   {
      return f.private_c4 + 300;
   }

   void set_c4( type_f& f, const int& c )
   {
      f.private_c4 = c;
   }

   template<>
   struct traits< type_f >
      : binding::object< binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a1" ), &get_a1 >,
                         binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a2" ), &get_a2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b1" ), &get_b1, &set_b1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b2" ), &get_b2, &set_b2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b3" ), &get_b3, &set_b3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b4" ), &get_b4, &set_b4 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c1" ), &get_c1, &set_c1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c2" ), &get_c2, &set_c2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c3" ), &get_c3, &set_c3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c4" ), &get_c4, &set_c4 > >
   {
      TAO_JSON_DEFAULT_KEY( "f" );
   };

   void unit_test()
   {
      const type_f f;
      const value v = produce::to_value( f );
      TEST_ASSERT( v.get_object().size() == 10 );

      TEST_ASSERT( v.get_object().at( "a1" ).as< int >() == 111 );
      TEST_ASSERT( v.get_object().at( "a2" ).as< int >() == 112 );

      TEST_ASSERT( v.get_object().at( "b1" ).as< int >() == 221 );
      TEST_ASSERT( v.get_object().at( "b2" ).as< int >() == 222 );
      TEST_ASSERT( v.get_object().at( "b3" ).as< int >() == 223 );
      TEST_ASSERT( v.get_object().at( "b4" ).as< int >() == 224 );

      TEST_ASSERT( v.get_object().at( "c1" ).as< int >() == 331 );
      TEST_ASSERT( v.get_object().at( "c2" ).as< int >() == 332 );
      TEST_ASSERT( v.get_object().at( "c3" ).as< int >() == 333 );
      TEST_ASSERT( v.get_object().at( "c4" ).as< int >() == 334 );

      //      v.to( f );
   }

}  // namespace tao::json

#include "../../test/json/main.hpp"
