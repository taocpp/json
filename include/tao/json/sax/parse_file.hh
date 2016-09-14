// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HH
#define TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HH

#include "../external/pegtl/file_parser.hh"

#include "../internal/grammar.hh"
#include "../internal/action.hh"
#include "../internal/control.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         template< typename Handler >
         void parse_file( const std::string & filename, Handler & handler )
         {
            tao_json_pegtl::file_parser( filename ).parse< internal::grammar, internal::action, internal::control >( handler );
         }

      } // sax

   } // json

} // tao

#endif
