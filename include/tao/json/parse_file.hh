// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
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
      inline void parse_file( value & output, const std::string & filename )
      {
         internal::result_state result;
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control >( result );
         output = std::move( result.result );
      }

      inline value parse_file( const std::string & filename )
      {
         internal::result_state result;
         tao_json_pegtl::file_parser( filename ).parse< internal::grammar, tao_json_pegtl::nothing, internal::control >( result );
         return std::move( result.result );
      }

   } // json

} // tao

#endif
