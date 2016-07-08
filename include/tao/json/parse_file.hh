// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PARSE_FILE_HH
#define TAOCPP_JSON_INCLUDE_PARSE_FILE_HH

#include "external/pegtl/file_parser.hh"

#include "internal/result_state.hh"
#include "internal/control.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      void parse_file( basic_value< Traits > & output, const std::string & filename )
      {
         internal::result_state< Traits > result;
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< Traits >::template control >( result );
         output = std::move( result.result );
      }

      template< template< typename ... > class Traits >
      basic_value< Traits > parse_file( const std::string & filename )
      {
         internal::result_state< Traits > result;
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< Traits >::template control >( result );
         return std::move( result.result );
      }

      inline value parse_file( const std::string & filename )
      {
         internal::result_state< traits > result;
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< traits >::control >( result );
         return std::move( result.result );
      }

   } // json

} // tao

#endif
