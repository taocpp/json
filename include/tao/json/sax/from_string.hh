// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_FROM_STRING_HH
#define TAOCPP_JSON_INCLUDE_SAX_FROM_STRING_HH

#include "../external/pegtl/parse.hh"

#include "../internal/grammar.hh"
#include "../internal/action.hh"
#include "../internal/control.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX producer to parse a JSON string representation
         template< typename Consumer >
         inline void from_string( const char * data, const std::size_t size, Consumer & consumer, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
         {
            tao_json_pegtl::input input( line, column, data, data + size, source ? source : "tao::json::sax::from_string" );
            tao_json_pegtl::parse_input< internal::grammar, internal::action, internal::control >( input, consumer );
         }

         template< typename Consumer >
         inline void from_string( const char * data, const std::size_t size, Consumer & consumer, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
         {
            sax::from_string( data, size, consumer, source.c_str(), line, column );
         }

         template< typename Consumer >
         inline void from_string( const std::string & data, Consumer & consumer, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
         {
            sax::from_string( data.data(), data.size(), consumer, source, line, column );
         }

         template< typename Consumer >
         inline void from_string( const std::string & data, Consumer & consumer, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
         {
            sax::from_string( data, consumer, source.c_str(), line, column );
         }

      } // sax

   } // json

} // tao

#endif
