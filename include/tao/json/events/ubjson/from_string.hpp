// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_FROM_STRING_HPP

#include "../../external/pegtl/parse.hpp"
#include "../../external/string_view.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace ubjson
         {
            template< typename Consumer >
            void from_string( Consumer& consumer, const char* data, const std::size_t size, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t byte_in_line = 0 )
            {
               json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > in( data, data + size, source ? source : "tao::json::ubjson::from_string", byte, line, byte_in_line );
               json_pegtl::parse< ubjson::grammar >( in, consumer );
            }

            template< typename Consumer >
            void from_string( Consumer& consumer, const char* data, const std::size_t size, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
            {
               ubjson::from_string( consumer, data, size, source.c_str(), byte, line, column );
            }

            template< typename Consumer, typename... Ts >
            void from_string( Consumer& consumer, const tao::string_view data, Ts&&... ts )
            {
               ubjson::from_string( consumer, data.data(), data.size(), std::forward< Ts >( ts )... );
            }

         }  // namespace ubjson

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
