// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_INCLUDE_JSON_BINDING_HPP
#define TAOCPP_INCLUDE_JSON_BINDING_HPP

#include <string>
#include <type_traits>

#include "external/pegtl/internal/pegtl_string.hpp"

#include "consume.hpp"
#include "events/produce.hpp"
#include "value.hpp"

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

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

            template< typename U >
            static void write( C& v, U&& u )
            {
               v.*P = std::forward< U >( u );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, v.*P );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& producer, C& v )
            {
               json::consume< Traits >( producer, v.*P );
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

         template< char... Cs >
         using key = json_pegtl::string< Cs... >;

         template< typename K, typename T, T >
         struct member;

         template< char... Cs, typename T, T P >
         struct member< key< Cs... >, T, P >
            : element< T, P >
         {
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
         };

         template< typename... As >
         struct array
         {
            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool unsafe_emplace_back( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_back( A::read( x ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void assign( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_array();
               (void)internal::swallow{ unsafe_emplace_back< As >( v, x )... };
            }

            template< typename A, template< typename... > class Traits, typename Consumer, typename C >
            static bool produce_element( Consumer& consumer, const C& x )
            {
               A::template produce< Traits >( consumer, x );
               consumer.element();
               return true;
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_array( sizeof...( As ) );
               (void)internal::swallow{ produce_element< As, Traits >( consumer, x )... };
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
               (void)internal::swallow{ unsafe_emplace< As >( v, x )... };
            }

            template< typename A, template< typename... > class Traits, typename Consumer, typename C >
            static bool produce_member( Consumer& consumer, const C& x )
            {
               A::template produce_key< Traits >( consumer );
               A::template produce< Traits >( consumer, x );
               consumer.member();
               return true;
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_object( sizeof...( As ) );
               (void)internal::swallow{ produce_member< As, Traits >( consumer, x )... };
               consumer.end_object( sizeof...( As ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic pop
#endif

#define TAOCPP_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAOCPP_JSON_BIND_MEMBER( KeY, ... ) tao::json::binding::member< TAOCPP_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
