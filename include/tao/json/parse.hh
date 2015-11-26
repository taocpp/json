// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PARSE_HH
#define TAOCPP_JSON_INCLUDE_PARSE_HH

#include "external/pegtl/file_parser.hh"

#include "internal/parse.hh"

namespace tao
{
   namespace json
   {
      inline void parse( value & output, const char * data, const size_t size )
      {
         internal::result_state result;
         tao_json_pegtl::input input( 1, 0, data, data + size, "TODO" );
         tao_json_pegtl::parse_input< internal::grammar, tao_json_pegtl::nothing, internal::control >( input, result );
         output = std::move( result.result );
      }

      inline value parse( const char * data, const size_t size )
      {
         internal::result_state result;
         tao_json_pegtl::input input( 1, 0, data, data + size, "TODO" );
         tao_json_pegtl::parse_input< internal::grammar, tao_json_pegtl::nothing, internal::control >( input, result );
         return std::move( result.result );
      }

      inline void parse( value & output, const std::string & data )
      {
         parse( output, data.data(), data.size() );
      }

      inline value parse( const std::string & data )
      {
         return parse( data.data(), data.size() );
      }

      inline void parse_file( value & output, const std::string & file )
      {
         internal::result_state result;
         tao_json_pegtl::file_parser( file ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control >( result );
         output = std::move( result.result );
      }

      inline value parse_file( const std::string & file )
      {
         internal::result_state result;
         tao_json_pegtl::file_parser( file ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control >( result );
         return std::move( result.result );
      }

   } // json

} // tao

#endif
