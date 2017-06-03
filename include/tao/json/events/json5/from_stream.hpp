// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_JSON5_FROM_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_JSON5_FROM_STREAM_HPP

#include <istream>

#include "../../external/pegtl/internal/istream_reader.hpp"
#include "../../external/pegtl/parse.hpp"

#include "../../internal/json5/action.hpp"
#include "../../internal/json5/control.hpp"
#include "../../internal/json5/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace json5
         {
            template< typename Consumer >
            inline void from_stream( std::istream& stream, Consumer& consumer, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
            {
               json_pegtl::istream_input<> in( stream, maximum_buffer_size, source ? source : "tao::json::events::json5::from_stream" );
               json_pegtl::parse< internal::json5::grammar, internal::json5::action, internal::json5::control >( in, consumer );
            }

            template< typename Consumer >
            inline void from_stream( std::istream& stream, Consumer& consumer, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
            {
               json5::from_stream( stream, consumer, source.c_str(), maximum_buffer_size );
            }

         }  // namespace json5

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
