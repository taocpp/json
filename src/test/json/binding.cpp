// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/other_to.hpp>

#include <type_traits>

namespace tao
{
   namespace json
   {
      template< typename T, T >
      struct element_binding;

      template< typename C, typename T, T C::*P >
      struct element_binding< T C::*, P >
      {
         using class_type = C;
         using value_type = T;

         static const T& read( const C& v )
         {
            return v.*P;
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const C& v )
         {
            events::produce< Traits >( consumer, read( v ) );
         }
      };

      template< typename T, T >
      struct element_function_binding;

      template< typename C, typename R, R ( C::*P )() >
      struct element_function_binding< R ( C::* )(), P >
      {
         using class_type = C;
         using value_type = R;

         static R read( const C& v ) noexcept
         {
            return ( v.*P )();
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const C& v )
         {
            events::produce< Traits >( consumer, read( v ) );
         }
      };

      template< typename C, typename R, R ( C::*P )() const >
      struct element_function_binding< R ( C::* )() const, P >
      {
         using class_type = C;
         using value_type = R;

         static R read( const C& v ) noexcept
         {
            return ( v.*P )();
         }

         template< template< typename... > class Traits = traits, typename Consumer >
         static void produce( Consumer& consumer, const C& v )
         {
            events::produce< Traits >( consumer, read( v ) );
         }
      };

      template< char... Cs >
      using key = json_pegtl::string< Cs... >;

      template< typename T, T, typename K >
      struct member_binding;

      template< typename C, typename T, T C::*P, char... Cs >
      struct member_binding< T C::*, P, key< Cs... > >
      {
         using key_type = json::key< Cs... >;
         using class_type = C;
         using value_type = T;

         static const T& read( const C& v )
         {
            return v.*P;
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
         static void produce( Consumer& consumer, const C& v )
         {
            events::produce< Traits >( consumer, read( v ) );
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
         template< typename A, template< typename... > class Traits, typename Base, typename X >
         static bool unsafe_emplace( basic_value< Traits, Base >& v, const X& x )
         {
            v.unsafe_emplace( A::key(), A::read( x ) );
            return true;
         }

         template< template< typename... > class Traits, typename Base, typename X >
         static void assign( basic_value< Traits, Base >& v, const X& x )
         {
            v.unsafe_emplace_object();
            using swallow = bool[];
            (void)swallow{ unsafe_emplace< As >( v, x )... };
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

#define BIND_ELEMENT( ... ) element_binding< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define BIND_ELEMENT_FUNCTION( ... ) element_function_binding< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define BIND_MEMBER( KeY, ... ) member_binding< decltype( __VA_ARGS__ ), __VA_ARGS__, TAOCPP_JSON_PEGTL_INTERNAL_STRING( key, KeY ) >

      struct point
      {
         double x = 1.0;
         double y = 2.0;
      };

      template<>
      struct traits< point >
         : array_binding_traits< BIND_ELEMENT( &point::x ),
                                 BIND_ELEMENT( &point::y ) >
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
         : object_binding_traits< BIND_MEMBER( "where", &employee::where ),
                                  BIND_MEMBER( "name", &employee::name ),
                                  BIND_MEMBER( "job", &employee::job ) >
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
         : array_binding_traits< BIND_ELEMENT( &std::pair< V, W >::first ),
                                 BIND_ELEMENT( &std::pair< V, W >::second ) >
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
         : array_binding_traits< BIND_ELEMENT_FUNCTION( &secret::get_int ),
                                 BIND_ELEMENT_FUNCTION( &secret::get_string ) >
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
