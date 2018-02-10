// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_BINDING_HPP
#define TAOCPP_JSON_INCLUDE_BINDING_HPP

#include <string>

#include "events/produce.hpp"
#include "external/pegtl/internal/pegtl_string.hpp"
#include "value.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename T, T >
         struct element;

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P >
         {
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
         struct element_function;

         template< typename C, typename R, R ( C::*P )() >
         struct element_function< R ( C::* )(), P >
         {
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
         struct element_function< R ( C::* )() const, P >
         {
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

         template< typename K, typename T, T >
         struct member;

         template< char... Cs, typename C, typename T, T C::*P >
         struct member< key< Cs... >, T C::*, P >
         {
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
         struct array
         {
            template< template< typename... > class Traits, typename Base, typename C >
            static void assign( basic_value< Traits, Base >& v, const C& x )
            {
               v.append( { As::read( x )... } );
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_array( sizeof...( As ) );
               using swallow = bool[];
               (void)swallow{ ( As::template produce< Traits >( consumer, x ), consumer.element(), true )... };
               consumer.end_array( sizeof...( As ) );
            }
         };

         template< typename... As >
         struct object
         {
            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool unsafe_emplace( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace( A::key(), A::read( x ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void assign( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_object();
               using swallow = bool[];
               (void)swallow{ unsafe_emplace< As >( v, x )... };
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_object( sizeof...( As ) );
               using swallow = bool[];
               (void)swallow{ ( As::template produce_key< Traits >( consumer ), As::template produce< Traits >( consumer, x ), consumer.member(), true )... };
               consumer.end_object( sizeof...( As ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAOCPP_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAOCPP_JSON_BIND_ELEMENT_FUNCTION( ... ) tao::json::binding::element_function< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAOCPP_JSON_BIND_MEMBER( KeY, ... ) tao::json::binding::member< TAOCPP_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
