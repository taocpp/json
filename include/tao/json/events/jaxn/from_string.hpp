// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JAXN_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JAXN_FROM_STRING_HPP

#include "../../external/pegtl/parse.hpp"

#include "../../internal/jaxn/action.hpp"
#include "../../internal/jaxn/control.hpp"
#include "../../internal/jaxn/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace jaxn
         {
            // Events producer to parse a JAXN string representation.

            template< typename Consumer >
            void from_string( const char* data, const std::size_t size, Consumer& consumer, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > in( data, data + size, source ? source : "tao::json::events::jaxn::from_string", byte, line, column );
               json_pegtl::parse< internal::jaxn::grammar, internal::jaxn::action, internal::jaxn::control >( in, consumer );
            }

            template< typename Consumer >
            void from_string( const char* data, const std::size_t size, Consumer& consumer, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               jaxn::from_string( data, size, consumer, source.c_str(), byte, line, column );
            }

            template< typename Consumer >
            void from_string( const std::string& data, Consumer& consumer, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               jaxn::from_string( data.data(), data.size(), consumer, source, byte, line, column );
            }

            template< typename Consumer >
            void from_string( const std::string& data, Consumer& consumer, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               jaxn::from_string( data, consumer, source.c_str(), byte, line, column );
            }

         }  // namespace jaxn

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
