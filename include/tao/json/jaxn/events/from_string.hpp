// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_EVENTS_FROM_STRING_HPP
#define TAO_JSON_JAXN_EVENTS_FROM_STRING_HPP

#include "../../external/pegtl/parse.hpp"
#include "../../external/string_view.hpp"

#include "../internal/action.hpp"
#include "../internal/control.hpp"
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

            template< typename Consumer >
            void from_string( Consumer& consumer, const char* data, const std::size_t size, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > in( data, data + size, source ? source : "tao::json::events::jaxn::from_string", byte, line, column );
               json_pegtl::parse< jaxn::internal::grammar, jaxn::internal::action, jaxn::internal::control >( in, consumer );
            }

            template< typename Consumer >
            void from_string( Consumer& consumer, const char* data, const std::size_t size, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               from_string( consumer, data, size, source.c_str(), byte, line, column );
            }

            template< typename Consumer, typename... Ts >
            void from_string( Consumer& consumer, const tao::string_view data, Ts&&... ts )
            {
               from_string( consumer, data.data(), data.size(), std::forward< Ts >( ts )... );
            }

         }  // namespace events

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
