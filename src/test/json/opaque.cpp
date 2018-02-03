// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/self_contained.hpp>
#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
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

      void unit_test()
      {
         const employee e;

         value v1 = {
            { "account", 1 },
            { "employee", &e }
         };

         const auto s1 = to_string( from_string( to_string( v1 ) ) );

         value v2 = v1;

         make_self_contained( v2 );

         const auto s2 = to_string( from_string( to_string( v2 ) ) );

         TEST_ASSERT( s1 == s2 );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
