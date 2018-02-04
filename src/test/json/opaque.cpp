// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/self_contained.hpp>
#include <tao/json/to_stream.hpp>
#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

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
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& consumer, const point& data )
         {
            consumer.begin_array( 2 );
            Traits< decltype( data.x ) >::template produce< Traits >( consumer, data.x );
            consumer.number( data.x );
            consumer.element();
            Traits< decltype( data.x ) >::template produce< Traits >( consumer, data.y );
            consumer.element();
            consumer.end_array( 2 );
         };
      };

      struct employee
      {
         std::string name = "Isidor";
         std::string position = "CEO";
         std::uint64_t income = 42;
      };

      template<>
      struct traits< employee >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& consumer, const employee& data )
         {
            consumer.begin_object();  // TODO: Object guard?
            consumer.key( "name" );
            Traits< std::string >::template produce< Traits >( consumer, data.name );
            consumer.member();
            if( !data.position.empty() ) {
               consumer.key( "position" );  // TODO: Does this need convenience for key+value+member?
               Traits< std::string >::template produce< Traits >( consumer, data.position );
               consumer.member();
            }
            consumer.key( "income" );
            Traits< decltype( data.income ) >::template produce< Traits >( consumer, data.income );
            consumer.member();
            consumer.end_object();
         }
      };

      template<>
      struct traits< const employee* >
      {
         static void assign( value& v, const employee* e )
         {
            v.unsafe_assign_opaque( e );
         }
      };

      namespace events
      {
         template< template< typename... > class Traits = traits, typename Consumer, typename T >
         void from_other( Consumer& c, T&& t )
         {
            Traits< typename std::decay< T >::type >::template produce< Traits >( c, std::forward< T >( t ) );
         }

      }  // namespace events

      template< template< typename... > class Traits = traits, typename T >
      void other_to_stream( std::ostream& os, const T& t )
      {
         events::to_stream consumer( os );
         events::from_other< Traits >( consumer, t );
      }

      template< template< typename... > class Traits = traits, typename T >
      void other_to_stream( std::ostream& os, const T& t, const std::size_t indent )
      {
         events::to_pretty_stream consumer( os, indent );
         events::from_other< Traits >( consumer, t );
      }

      template< template< typename... > class Traits = traits, typename... Ts >
      std::string other_to_string( Ts&&... ts )
      {
         std::ostringstream oss;
         other_to_stream< Traits >( oss, std::forward< Ts >( ts )... );
         return oss.str();
      }

      void unit_test()
      {
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

         const auto s3 = other_to_string( e );

         TEST_ASSERT( s3 == "{\"name\":\"Isidor\",\"position\":\"CEO\",\"income\":42}" );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
