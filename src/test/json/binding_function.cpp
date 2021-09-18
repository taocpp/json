// Copyright (c) 2018-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

namespace tao::json
{
   struct type_f
   {
      int private_a;
      int private_b;
   };

   [[nodiscard]] int get_a( const type_f& f ) noexcept
   {
      return f.private_a + 10;
   }

   [[nodiscard]] int& set_a( type_f& f ) noexcept
   {
      return f.private_a;
   }

   [[nodiscard]] int get_b( const type_f& f )
   {
      return f.private_b + 20;
   }

   // void set_b( type_f& f, int& b ) noexcept
   // {
   //    f.private_b = b;
   // }

   template<>
   struct traits< type_f >
      : binding::object< binding::member2< binding::member_kind::required, TAO_JSON_STRING_T( "a" ), &get_a, &set_a >,
                         binding::member< binding::member_kind::required, TAO_JSON_STRING_T( "b" ), &get_b > >
   {
      TAO_JSON_DEFAULT_KEY( "f" );
   };

   void unit_test()
   {
      const type_f f{ 41, 42 };
      const value v = produce::to_value( f );
      TEST_ASSERT( v.get_object().size() == 2 );
      TEST_ASSERT( v.get_object().at( "a" ).as< int >() == 51 );
      TEST_ASSERT( v.get_object().at( "b" ).as< int >() == 62 );
   }

}  // namespace tao::json

#include "../../test/json/main.hpp"
