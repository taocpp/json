// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_EVENTS_FROM_STREAM_HPP
#define TAO_JSON_JAXN_EVENTS_FROM_STREAM_HPP

#include <istream>

#include "../../external/pegtl/internal/istream_reader.hpp"
#include "../../external/pegtl/parse.hpp"

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
            template< typename Consumer >
            void from_stream( Consumer& consumer, std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
            {
               json_pegtl::istream_input<> in( stream, maximum_buffer_size, source ? source : "tao::json::events::jaxn::from_stream" );
               json_pegtl::parse< jaxn::internal::grammar, jaxn::internal::action, jaxn::internal::control >( in, consumer );
            }

            template< typename Consumer >
            void from_stream( Consumer& consumer, std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
            {
               from_stream( consumer, stream, source.c_str(), maximum_buffer_size );
            }

         }  // namespace events

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
