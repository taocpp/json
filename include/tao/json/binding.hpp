// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_HPP
#define TAO_JSON_BINDING_HPP

#include <bitset>
#include <exception>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "external/pegtl/internal/pegtl_string.hpp"

#include "consume.hpp"
#include "events/produce.hpp"
#include "forward.hpp"
#include "internal/escape.hpp"
#include "internal/type_traits.hpp"

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

         template< char... Cs >
         using key = json_pegtl::string< Cs... >;

         enum class member_kind : bool
         {
            OPTIONAL,
            REQUIRED
         };

         template< member_kind R, typename K >
         struct member_key;

         template< member_kind R, char... Cs >
         struct member_key< R, key< Cs... > >
         {
            static constexpr member_kind kind = R;

            static std::string key()
            {
               static const char s[] = { Cs..., 0 };
               return std::string( s, sizeof...( Cs ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce_key( Consumer& consumer )
            {
               static const char s[] = { Cs..., 0 };
               consumer.key( tao::string_view( s, sizeof...( Cs ) ) );
            }
         };

         template< member_kind R, typename K, typename T, T P >
         struct member
            : public element< T, P >,
              public member_key< R, K >
         {
            template< template< typename... > class Traits, typename C >
            static bool is_nothing( const C& x )
            {
               return json::internal::is_nothing< Traits >( element< T, P >::read( x ) );
            }
         };

         template< typename T >
         struct inherit;

         enum class for_unknown_key : bool
         {
            THROW,
            CONTINUE
         };

         enum class for_nothing_value : bool
         {
            ENCODE,
            SUPPRESS
         };

         namespace internal
         {
            template< typename T >
            struct inherit_elements_t
            {
               using list = json::internal::type_list< T >;
            };

            template< typename T >
            struct inherit_elements_t< inherit< T > >
            {
               using list = typename T::elements;
            };

            template< typename T >
            using inherit_elements = typename inherit_elements_t< T >::list;

            template< typename T >
            struct inherit_members_t
            {
               using list = json::internal::type_list< T >;
            };

            template< typename T >
            struct inherit_members_t< inherit< T > >
            {
               using list = typename T::members;
            };

            template< typename T >
            using inherit_members = typename inherit_members_t< T >::list;

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic pop
#endif

#define TAO_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

// TODO: Clean up this file and the structure around it.

#include "binding/internal/array.hpp"
#include "binding/internal/object.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename... As >
         using array = internal::array< json::internal::merge_type_lists< internal::inherit_elements< As >... > >;

         template< for_unknown_key E, for_nothing_value N, typename... As >
         using basic_object = internal::basic_object< E, N, json::internal::merge_type_lists< internal::inherit_members< As >... > >;

         template< typename... As >
         using object = basic_object< for_unknown_key::THROW, for_nothing_value::ENCODE, As... >;

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#include "binding/constants.hpp"
#include "binding/factory.hpp"
#include "binding/versions.hpp"

#endif
