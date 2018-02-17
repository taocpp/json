// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/binding.hpp>

namespace tao
{
   namespace json
   {
      struct point
      {
         double x = 1.0;
         double y = 2.0;
      };

      template<>
      struct traits< point >
         : binding::array< TAO_JSON_BIND_ELEMENT( &point::x ),
                           TAO_JSON_BIND_ELEMENT( &point::y ) >
      {
      };

      struct employee
      {
         point where;
         std::string name = "Isidor";
         tao::optional< std::string > job = std::string( "CEO" );
      };

      template<>
      struct traits< employee >
         : binding::object< TAO_JSON_BIND_REQUIRED( "where", &employee::where ),
                            TAO_JSON_BIND_REQUIRED( "name", &employee::name ),
                            TAO_JSON_BIND_REQUIRED( "job", &employee::job ) >
      {
      };

      template<>
      struct traits< const employee* >
      {
         static void assign( value& v, const employee* e )
         {
            v.unsafe_assign_opaque_ptr( e );
         }
      };

      template< typename V, typename W >
      struct traits< std::pair< V, W > >
         : binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< V, W >::first ),
                           TAO_JSON_BIND_ELEMENT( &std::pair< V, W >::second ) >
      {
      };

      class secret
      {
      public:
         int get_int() const
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
      struct traits< secret >
         : binding::array< TAO_JSON_BIND_ELEMENT( &secret::get_int ),
                           TAO_JSON_BIND_ELEMENT( &secret::get_string ) >
      {
      };

      void unit_test()
      {
         value vv = { { "foo", empty_object } };

         const employee e{};

         value v1 = {
            { "account", 1 },
            { "employee", &e }
         };

         const auto s1 = to_string( from_string( to_string( v1 ) ) );

         value v2 = v1;

         make_self_contained( v2 );

         const auto s2 = to_string( from_string( to_string( v2 ) ) );

         TEST_ASSERT( s1 == s2 );

         value v3 = v2;

         TEST_ASSERT( v2 == v3 );
         TEST_ASSERT( s2 == to_string( from_string( "{\"account\":1,\"employee\":{\"name\":\"Isidor\",\"job\":\"CEO\",\"where\":[1.0,2.0]}}" ) ) );

         value v4 = point();

         TEST_ASSERT( v4.type() == type::ARRAY );
         TEST_ASSERT( to_string( v4 ) == "[1.0,2.0]" );

         value v5 = employee();

         TEST_ASSERT( v5.type() == type::OBJECT );

         value v6 = std::pair< int, std::string >( 42, "galaxy" );

         TEST_ASSERT( v6.type() == type::ARRAY );
         TEST_ASSERT( to_string( v6 ) == "[42,\"galaxy\"]" );

         value v7 = secret();

         TEST_ASSERT( v7.type() == type::ARRAY );
         TEST_ASSERT( to_string( v7 ) == "[7,\"dangerous\"]" );
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
