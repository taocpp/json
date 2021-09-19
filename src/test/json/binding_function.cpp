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

   // void set_b( type_f& f, int& b ) noexcept
   // {
   //    f.private_b = b;
   // }

   template<>
   struct traits< type_f >
      : binding::object< binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a1" ), &get_a1 >,
                         binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a2" ), &get_a2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b1" ), &get_b1, &set_b1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b2" ), &get_b2, &set_b2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b3" ), &get_b3, &set_b3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b4" ), &get_b4, &set_b4 > >
   {
      TAO_JSON_DEFAULT_KEY( "f" );
   };

   void unit_test()
   {
      const type_f f;
      const value v = produce::to_value( f );
      TEST_ASSERT( v.get_object().size() == 6 );

      TEST_ASSERT( v.get_object().at( "a1" ).as< int >() == 111 );
      TEST_ASSERT( v.get_object().at( "a2" ).as< int >() == 112 );

      TEST_ASSERT( v.get_object().at( "b1" ).as< int >() == 221 );
      TEST_ASSERT( v.get_object().at( "b2" ).as< int >() == 222 );
      TEST_ASSERT( v.get_object().at( "b3" ).as< int >() == 223 );
      TEST_ASSERT( v.get_object().at( "b4" ).as< int >() == 224 );
   }

}  // namespace tao::json

#include "../../test/json/main.hpp"
