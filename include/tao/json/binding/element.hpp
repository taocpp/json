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
         struct element< T C::*, P, std::enable_if_t< std::is_member_object_pointer< T C::* >::value > >
         {
            using internal_t = std::decay_t< decltype( std::declval< C >().*P ) >;

            static auto read( const C& v )
            {
               return v.*P;
            }

            template< typename W >
            static void write( C& v, W&& w )
            {
               v.*P = std::forward< W >( w );
            }

            template< template< typename... > class Traits, typename Base >
            static void to( const basic_value< Traits, Base >& v, C& x )
            {
               v.to( x.*P );
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
         struct element< T C::*, P, std::enable_if_t< std::is_member_function_pointer< T C::* >::value > >
         {
            using internal_t = std::decay_t< decltype( ( std::declval< const C >().*P )() ) >;

            static auto read( const C& v )
            {
               return ( v.*P )();
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, ( v.*P )() );
            }
         };

         template< typename C, typename T, T ( *P )( const C& ) >
         struct element< T ( * )( const C& ), P >
         {
            static auto read( const C& v )
            {
               return P( v );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, P( v ) );
            }
         };

         template< typename CP, CP, typename P, P, typename = void >
         struct element2;

         template< typename A, typename CR, CR ( *CP )( const A& ), typename R, R ( *P )( A& ) >
         struct element2< CR ( * )( const A& ), CP, R ( * )( A& ), P >
         {
            static auto read( const A& v )
            {
               return CP( v );
            }

            template< typename W >
            static void write( A& v, W&& w )
            {
               P( v ) = std::forward< W >( w );
            }

            template< template< typename... > class Traits, typename Base >
            static void to( const basic_value< Traits, Base >& v, A& x )
            {
               v.to( P( x ) );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, A& v )
            {
               json::consume< Traits >( parser, P( v ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const A& v )
            {
               events::produce< Traits >( consumer, CP( v ) );
            }
         };

         template< typename A, typename CR, CR ( *CP )( const A& ), typename R, void ( *P )( A&, R&& ) >
         struct element2< CR ( * )( const A& ), CP, void ( * )( A&, R&& ), P >
         {
            static auto read( const A& v )
            {
               return CP( v );
            }

            template< typename W >
            static void write( A& v, W&& w )
            {
               CP( v, std::forward< W >( w ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void to( const basic_value< Traits, Base >& v, A& x )
            {
               CP( x, v.template as< std::decay_t< R > >() );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, A& v )
            {
               P( v, json::consume< Traits >( parser ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const A& v )
            {
               events::produce< Traits >( consumer, CP( v ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
