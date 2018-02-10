// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/other_to.hpp>

namespace tao
{
   namespace json
   {
      template< typename X, typename T, T X::*P >
      struct element_binding
      {
         using class_type = X;
         using value_type = T;

         static const T& read( const X& x )
         {
            return x.*P;
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, read( x ) );
         }
      };

      template< typename X, typename T, const T& ( X::*P )() const noexcept >
      struct element_mem_fun_ref
      {
         using class_type = X;
         using value_type = T;

         static const T& read( const X& x ) noexcept
         {
            return ( x.*P )();
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, read( x ) );
         }
      };

      template< typename X, typename T, T ( X::*P )() const >
      struct element_mem_fun_val
      {
         using class_type = X;
         using value_type = T;

         static T read( const X& x )
         {
            return ( x.*P )();
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, read( x ) );
         }
      };

      template< typename X, typename T, T X::*P, typename K >
      struct member_binding;

      template< char... Cs >
      using key = json_pegtl::string< Cs... >;

      template< typename X, typename T, T X::*P, char... Cs >
      struct member_binding< X, T, P, key< Cs... > >
      {
         using key_type = json::key< Cs... >;
         using class_type = X;
         using value_type = T;

         static const T& read( const X& x )
         {
            return x.*P;
         }

         static std::string key()
         {
            const char s[] = { Cs..., 0 };
            return std::string( s, sizeof...( Cs ) );
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce_key( Consumer& consumer )
         {
            static const char s[] = { Cs..., 0 };
            consumer.key( tao::string_view( s, sizeof...( Cs ) ) );
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const X& x )
         {
            events::produce< Traits >( consumer, read( x ) );
         }
      };

      template< typename... As >
      struct array_binding_traits
      {
         template< template< typename... > class Traits, typename Base, typename X >
         static void assign( basic_value< Traits, Base >& v, const X& x )
         {
            v.append( { As::read( x )... } );
         }

         template< template< typename... > class Traits = traits, typename Consumer, typename X >
         static void produce( Consumer& consumer, const X& x )
         {
            consumer.begin_array( sizeof...( As ) );
            using swallow = bool[];
            (void)swallow{ ( As::template produce< Traits >( consumer, x ), consumer.element(), true )... };
            consumer.end_array( sizeof...( As ) );
         }
      };

      template< typename... As >
      struct object_binding_traits
      {
         template< template< typename... > class Traits, typename Base, typename X >
         static void assign( basic_value< Traits, Base >& v, const X& x )
         {
            v.unsafe_emplace_object();
            using swallow = bool[];
            (void)swallow{ ( v.unsafe_emplace( As::key(), As::read( x ) ), true )... };
         }

         template< template< typename... > class Traits = traits, typename Consumer, typename X >
         static void produce( Consumer& consumer, const X& x )
         {
            consumer.begin_object( sizeof...( As ) );
            using swallow = bool[];
            (void)swallow{ ( As::template produce_key< Traits >( consumer ), As::template produce< Traits >( consumer, x ), consumer.member(), true )... };
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
         : array_binding_traits< element_binding< point, double, &point::x >,
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
         : object_binding_traits< member_binding< employee, point, &employee::where, key< 'w', 'h', 'e', 'r', 'e' > >,
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

      template< typename V, typename W >
      struct traits< std::pair< V, W > >
         : array_binding_traits< element_binding< std::pair< V, W >, V, &std::pair< V, W >::first >,
                                 element_binding< std::pair< V, W >, W, &std::pair< V, W >::second > >
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
         : array_binding_traits< element_mem_fun_val< secret, int, &secret::get_int >,
                                 element_mem_fun_ref< secret, std::string, &secret::get_string > >
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
