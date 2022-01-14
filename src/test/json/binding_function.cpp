// Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
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

   struct type_g
   {
      int private_b1 = 21;
      int private_b2 = 22;
      int private_b3 = 23;
      int private_b4 = 24;

      int private_c1 = 31;
      int private_c2 = 32;
      int private_c3 = 33;
      int private_c4 = 34;
   };

   [[nodiscard]] int get_fa1( const type_f& f )
   {
      return f.private_a1 + 100;
   }

   [[nodiscard]] int get_fa2( const type_f& f ) noexcept
   {
      return f.private_a2 + 100;
   }

   [[nodiscard]] int get_fb1( const type_f& f )
   {
      return f.private_b1 + 200;
   }

   [[nodiscard]] int& set_fb1( type_f& f )
   {
      return f.private_b1;
   }

   [[nodiscard]] int get_fb2( const type_f& f ) noexcept
   {
      return f.private_b2 + 200;
   }

   [[nodiscard]] int& set_fb2( type_f& f )
   {
      return f.private_b2;
   }

   [[nodiscard]] int get_fb3( const type_f& f )
   {
      return f.private_b3 + 200;
   }

   [[nodiscard]] int& set_fb3( type_f& f ) noexcept
   {
      return f.private_b3;
   }

   [[nodiscard]] int get_fb4( const type_f& f ) noexcept
   {
      return f.private_b4 + 200;
   }

   [[nodiscard]] int& set_fb4( type_f& f ) noexcept
   {
      return f.private_b4;
   }

   [[nodiscard]] int get_fc1( const type_f& f ) noexcept
   {
      return f.private_c1 + 300;
   }

   void set_fc1( type_f& f, const int c ) noexcept
   {
      f.private_c1 = c;
   }

   [[nodiscard]] int get_fc2( const type_f& f )
   {
      return f.private_c2 + 300;
   }

   void set_fc2( type_f& f, const int& c ) noexcept
   {
      f.private_c2 = c;
   }

   [[nodiscard]] int get_fc3( const type_f& f ) noexcept
   {
      return f.private_c3 + 300;
   }

   void set_fc3( type_f& f, int&& c ) noexcept
   {
      f.private_c3 = c;
   }

   [[nodiscard]] int get_fc4( const type_f& f )
   {
      return f.private_c4 + 300;
   }

   void set_fc4( type_f& f, const int& c )
   {
      f.private_c4 = c;
   }

   [[nodiscard]] int get_gb1( const type_g& f )
   {
      return f.private_b1 + 200;
   }

   [[nodiscard]] int& set_gb1( type_g& f )
   {
      return f.private_b1;
   }

   [[nodiscard]] int get_gb2( const type_g& f ) noexcept
   {
      return f.private_b2 + 200;
   }

   [[nodiscard]] int& set_gb2( type_g& f )
   {
      return f.private_b2;
   }

   [[nodiscard]] int get_gb3( const type_g& f )
   {
      return f.private_b3 + 200;
   }

   [[nodiscard]] int& set_gb3( type_g& f ) noexcept
   {
      return f.private_b3;
   }

   [[nodiscard]] int get_gb4( const type_g& f ) noexcept
   {
      return f.private_b4 + 200;
   }

   [[nodiscard]] int& set_gb4( type_g& f ) noexcept
   {
      return f.private_b4;
   }

   [[nodiscard]] int get_gc1( const type_g& f ) noexcept
   {
      return f.private_c1 + 300;
   }

   void set_gc1( type_g& f, const int c ) noexcept
   {
      f.private_c1 = c;
   }

   [[nodiscard]] int get_gc2( const type_g& f )
   {
      return f.private_c2 + 300;
   }

   void set_gc2( type_g& f, const int& c ) noexcept
   {
      f.private_c2 = c;
   }

   [[nodiscard]] int get_gc3( const type_g& f ) noexcept
   {
      return f.private_c3 + 300;
   }

   void set_gc3( type_g& f, int&& c ) noexcept
   {
      f.private_c3 = c;
   }

   [[nodiscard]] int get_gc4( const type_g& f )
   {
      return f.private_c4 + 300;
   }

   void set_gc4( type_g& f, const int& c )
   {
      f.private_c4 = c;
   }

   template<>
   struct traits< type_f >
      : binding::object< binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a1" ), &get_fa1 >,
                         binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "a2" ), &get_fa2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b1" ), &get_fb1, &set_fb1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b2" ), &get_fb2, &set_fb2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b3" ), &get_fb3, &set_fb3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b4" ), &get_fb4, &set_fb4 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c1" ), &get_fc1, &set_fc1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c2" ), &get_fc2, &set_fc2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c3" ), &get_fc3, &set_fc3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c4" ), &get_fc4, &set_fc4 > >
   {
      TAO_JSON_DEFAULT_KEY( "f" );
   };

   template<>
   struct traits< type_g >
      : binding::object< binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b1" ), &get_gb1, &set_gb1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b2" ), &get_gb2, &set_gb2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b3" ), &get_gb3, &set_gb3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "b4" ), &get_gb4, &set_gb4 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c1" ), &get_gc1, &set_gc1 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c2" ), &get_gc2, &set_gc2 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c3" ), &get_gc3, &set_gc3 >,
                         binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "c4" ), &get_gc4, &set_gc4 > >
   {
      TAO_JSON_DEFAULT_KEY( "f" );
   };

   void unit_test()
   {
      const type_f f;

      value v = produce::to_value( f );

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

      v.get_object().erase( "a1" );
      v.get_object().erase( "a2" );

      const type_g g = v.as< type_g >();

      TEST_ASSERT( g.private_b1 == 221 );
      TEST_ASSERT( g.private_b2 == 222 );
      TEST_ASSERT( g.private_b3 == 223 );
      TEST_ASSERT( g.private_b4 == 224 );

      TEST_ASSERT( g.private_c1 == 331 );
      TEST_ASSERT( g.private_c2 == 332 );
      TEST_ASSERT( g.private_c3 == 333 );
      TEST_ASSERT( g.private_c4 == 334 );
   }

}  // namespace tao::json

#include "../../test/json/main.hpp"
