// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PARSE_FILE_HH
#define TAOCPP_JSON_INCLUDE_PARSE_FILE_HH

#include "external/pegtl/file_parser.hh"

#include "internal/value_builder.hh"

#include "internal/grammar.hh"
#include "internal/action.hh"
#include "internal/control.hh"

namespace tao
{
   namespace json
   {
      template< typename Handler >
      void parse_file( const std::string & filename, Handler & handler )
      {
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, internal::action, internal::control >( handler );
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > parse_file( const std::string & filename )
      {
         internal::value_builder< Traits > handler;
         json::parse_file( filename, handler );
         return std::move( handler.value_ );
      }

      template< template< typename ... > class Traits >
      void parse_file( basic_value< Traits > & output, const std::string & filename )
      {
         output = parse_file< Traits >( filename );
      }

      inline value parse_file( const std::string & filename )
      {
         return parse_file< traits >( filename );
      }

   } // json

} // tao

#endif
