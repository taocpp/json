// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_MEMBER_HPP
#define TAO_JSON_BINDING_MEMBER_HPP

#include <string>

#include "element.hpp"
#include "member_kind.hpp"

#include "internal/type_key.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< member_kind R, typename K, typename T, T P >
         struct member
            : public element< T, P >,
              public internal::type_key< K, typename element< T, P >::internal_t >
         {
            static constexpr member_kind kind = R;

            template< template< typename... > class Traits, typename C >
            static bool is_nothing( const C& x )
            {
               return json::internal::is_nothing< Traits >( element< T, P >::read( x ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED_MEMBER, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL_MEMBER, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#define TAO_JSON_BIND_REQUIRED1( ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED_MEMBER, tao::json::binding::internal::use_default_key, decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL1( ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL_MEMBER, tao::json::binding::internal::use_default_key, decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
