// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_EVENTS_FROM_INPUT_HPP
#define TAO_JSON_JAXN_EVENTS_FROM_INPUT_HPP

#include <utility>

#include "../../external/pegtl/parse.hpp"

#include "../internal/action.hpp"
#include "../internal/errors.hpp"
#include "../internal/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace events
         {
            // Events producer to parse a JAXN string representation.

            template< typename Consumer, typename Input >
            void from_input( Consumer& consumer, Input&& in )
            {
               json_pegtl::parse< internal::grammar, internal::action, internal::errors >( std::forward< Input >( in ), consumer );
            }

         }  // namespace events

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
