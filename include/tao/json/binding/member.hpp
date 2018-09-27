// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_MEMBER_HPP
#define TAO_JSON_BINDING_MEMBER_HPP

#include <string>

#include "../internal/string_t.hpp"
#include "../internal/type_traits.hpp"

#include "element.hpp"
#include "member_kind.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< char... Cs >
         using key = internal::string_t< Cs... >;

         template< member_kind R, typename K >
         struct member_key;

         template< member_kind R, char... Cs >
         struct member_key< R, key< Cs... > >
         {
            static constexpr member_kind kind = R;

            static std::string key()
            {
               return binding::key< Cs... >::as_string();
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce_key( Consumer& consumer )
            {
               consumer.key( binding::key< Cs... >::as_string_view() );
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

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED_MEMBER, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL_MEMBER, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
