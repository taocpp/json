// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_MEMBER_HPP
#define TAO_JSON_BINDING_MEMBER_HPP

#include <string>

#include "../external/pegtl/internal/pegtl_string.hpp"
#include "../external/string_view.hpp"
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
         using key = json_pegtl::string< Cs... >;

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

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
