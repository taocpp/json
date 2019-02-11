// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
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
         template< member_kind R, typename K, auto P >
         struct member
            : public element< P >,
              public internal::type_key< K, typename element< P >::internal_t >
         {
            static constexpr member_kind kind = R;

            template< template< typename... > class Traits, typename C >
            static bool is_nothing( const C& x )
            {
               return json::internal::is_nothing< Traits >( element< P >::read( x ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
