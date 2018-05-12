// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_HPP
#define TAO_JSON_BINDING_HPP

#include "binding/constant.hpp"
#include "binding/element.hpp"
#include "binding/factory.hpp"
#include "binding/for_nothing_value.hpp"
#include "binding/for_unknown_key.hpp"
#include "binding/inherit.hpp"
#include "binding/member.hpp"
#include "binding/versions.hpp"

#include "binding/internal/array.hpp"
#include "binding/internal/inherit.hpp"
#include "binding/internal/object.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         namespace internal
         {
            template< typename... As >
            struct make_array
            {
               using list = json::internal::merge_type_lists< internal::inherit_elements< As >... >;
               using type = internal::array< list >;
            };

         }  // namespace internal

         template< typename... As >
         using array = typename internal::make_array< As... >::type;

         template< for_unknown_key E, for_nothing_value N, typename... As >
         using basic_object = internal::basic_object< E, N, json::internal::merge_type_lists< internal::inherit_members< As >... > >;

         template< typename... As >
         using object = basic_object< for_unknown_key::THROW, for_nothing_value::ENCODE, As... >;

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
