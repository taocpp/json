// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/other_to.hpp>

namespace tao
{
   namespace json
   {
      template< char... Cs > using key = json_pegtl::string< Cs... >;

      template< typename X, typename T, T X::*P >
      struct element_binding
      {
         using class_type = X;
         using value_type = T;

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, x.*P );
         }
      };

      template< typename X, typename T, T X::*P, typename K >
      struct member_binding;

      template< typename X, typename T, T X::*P, char... Cs >
      struct member_binding< X, T, P, key< Cs... > >
      {
         using key_type = key< Cs... >;
         using class_type = X;
         using value_type = T;

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const std::initializer_list< char >& k = { Cs... } )
         {
            consumer.key( tao::string_view( k.begin(), k.size() ) );
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, x.*P );
         }
      };

      struct expander
      {
         expander( const std::initializer_list< int >& /*unused*/ )
         {
         }
      };

      void expand( const std::initializer_list< expander >& /*unused*/ )
      {
      }

      template< typename... As >
      struct array_binding
      {
         template< template< typename... > class Traits = traits, typename Consumer, typename X >
         static void produce( Consumer& consumer, const X& x )
         {
            consumer.begin_array( sizeof...( As ) );
            expand( { { ( As::template produce< Traits >( consumer, x ), 0 ), ( consumer.element(), 0 ) }... } );
            consumer.end_array( sizeof...( As ) );
         }
      };

      template< typename... As >
      struct object_binding
      {
         template< template< typename... > class Traits = traits, typename Consumer, typename X >
         static void produce( Consumer& consumer, const X& x )
         {
            consumer.begin_object( sizeof...( As ) );
            expand( { { ( As::template produce< Traits >( consumer ), 0 ), ( As::template produce< Traits >( consumer, x ), 0 ), ( consumer.member(), 0 ) }... } );
            consumer.end_object( sizeof...( As ) );
         }
      };

      struct point
      {
         double x = 1.0;
         double y = 2.0;
      };

      template<>
      struct traits< point >
         : array_binding< element_binding< point, double, &point::x >,
                          element_binding< point, double, &point::y > >
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
         : object_binding< member_binding< employee, point, &employee::where, key< 'w', 'h','e', 'r', 'e' > >,
                           member_binding< employee, std::string, &employee::name, key< 'n', 'a', 'm', 'e' > >,
                           member_binding< employee, tao::optional< std::string >, &employee::job, key< 'j', 'o', 'b' > > >
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

      void unit_test()
      {
         value vv = { { "foo", empty_object } };

         const employee e{};

         value v1 = {
            { "account", 1 },
            { "employee", &e }
         };

         const auto s1 = to_string( from_string( to_string( v1 ) ) );

         std::cerr << s1 << std::endl;

         value v2 = v1;

         make_self_contained( v2 );

         const auto s2 = to_string( from_string( to_string( v2 ) ) );

         TEST_ASSERT( s1 == s2 );

         value v3 = v2;

         TEST_ASSERT( v2 == v3 );
         TEST_ASSERT( s2 == "{\"account\":1,\"employee\":{\"name\":\"Isidor\",\"job\":\"CEO\",\"where\":[1.0,2.0]}}" );
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
