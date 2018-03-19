// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_ELEMENT_HPP
#define TAO_JSON_BINDING_ELEMENT_HPP

#include <type_traits>

#include "../consume.hpp"
#include "../forward.hpp"

#include "../events/produce.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename T, T, typename = void >
         struct element;

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_object_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( v.*P )
            {
               return v.*P;
            }

            template< typename W >
            static void write( C& v, W&& w )
            {
               v.*P = std::forward< W >( w );
            }

            template< template< typename... > class Traits, typename Base >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               v.as( x.*P );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, C& v )
            {
               json::consume< Traits >( parser, v.*P );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, v.*P );
            }
         };

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_function_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( ( v.*P )() )
            {
               return ( v.*P )();
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, ( v.*P )() );
            }
         };

         template< typename C, typename T, T( *P )( const C& ) >
         struct element< T( * )( const C& ), P >
         {
            static auto read( const C& v ) -> decltype( P( v ) )
            {
               return P( v );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, P( v ) );
            }
         };

         template< typename T, T, typename U, U, typename = void >
         struct element2;

         template< typename C, typename T, T( *P )( const C& ), typename U, U( *Q )( C& ) >
         struct element2< T( * )( const C& ), P, U( * )( C& ), Q >
         {
            static auto read( const C& v ) -> decltype( P( v ) )
            {
               return P( v );
            }

            template< typename W >
            static void write( C& v, W&& w )
            {
               Q( v ) = std::forward< W >( w );
            }

            template< template< typename... > class Traits, typename Base >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               v.as( Q( x ) );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, C& v )
            {
               json::consume< Traits >( parser, Q( v ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, P( v ) );
            }
         };

         template< typename C, typename T, T( *P )( const C& ), typename U, void( *Q )( C&, U&& ) >
         struct element2< T( * )( const C& ), P, void( * )( C&, U&& ), Q >
         {
            static auto read( const C& v ) -> decltype( P( v ) )
            {
               return P( v );
            }

            template< typename W >
            static void write( C& v, W&& w )
            {
               Q( v, std::forward< W >( w ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               Q( x, v.template as< typename std::decay< U >::type >() );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, C& v )
            {
               Q( v, json::consume< Traits >( parser ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, P( v ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
