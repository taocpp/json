// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_INTERNAL_MEMBER_KEY_HPP
#define TAO_JSON_BINDING_INTERNAL_MEMBER_KEY_HPP

#include <string>

#include "../../internal/string_t.hpp"
#include "../../internal/type_traits.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         namespace internal
         {
            struct use_default_key
            {
            };

            template< typename K, typename V >
            struct member_key;

            template< char... Cs, typename V >
            struct member_key< json::internal::string_t< Cs... >, V >
            {
               template< template< typename... > class Traits >
               static std::string key()
               {
                  return json::internal::string_t< Cs... >::as_string();
               }

               template< template< typename... > class Traits = traits, typename Consumer >
               static void produce_key( Consumer& consumer )
               {
                  consumer.key( json::internal::string_t< Cs... >::as_string_view() );
               }
            };


            template< typename V >
            struct member_key< use_default_key, V >
            {
               template< template< typename... > class Traits >
               static std::string key()
               {
                  return Traits< V >::default_key::as_string();
               }

               template< template< typename... > class Traits = traits, typename Consumer >
               static void produce_key( Consumer& consumer )
               {
                  consumer.key( Traits< V >::default_key::as_string_view() );
               }
            };

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
