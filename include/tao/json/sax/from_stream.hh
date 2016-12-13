// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_FROM_STREAM_HH
#define TAOCPP_JSON_INCLUDE_SAX_FROM_STREAM_HH

#include <istream>

#include "../external/pegtl/parse.hh"
#include "../external/pegtl/internal/istream_reader.hh"

#include "../internal/grammar.hh"
#include "../internal/action.hh"
#include "../internal/control.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         template< typename Consumer >
         inline void from_stream( std::istream & stream, Consumer & consumer, const char * source = nullptr, const std::size_t maximum_buffer_size = 4000 )
         {
            tao_json_pegtl::buffer_input< tao_json_pegtl::internal::istream_reader > input( source ? source : "tao::json::sax::from_stream", maximum_buffer_size, stream );
            tao_json_pegtl::parse_input< internal::grammar, internal::action, internal::control >( input, consumer );
         }

         template< typename Consumer >
         inline void from_stream( std::istream & stream, Consumer & consumer, const std::string & source, const std::size_t maximum_buffer_size = 4000 )
         {
            sax::from_stream( stream, consumer, source.c_str(), maximum_buffer_size );
         }

      } // sax

   } // json

} // tao

#endif
