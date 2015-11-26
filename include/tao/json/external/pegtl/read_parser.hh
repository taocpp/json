// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_READ_PARSER_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_READ_PARSER_HH

#include "data_parser.hh"

#include "internal/file_reader.hh"

namespace tao_json_pegtl
{
   class read_parser
         : public data_parser
   {
   public:
      explicit
      read_parser( const std::string & filename )
            : data_parser( internal::file_reader( filename ).read(), filename )
      { }

      read_parser( const std::string & filename, const tao_json_pegtl::input & from )
            : data_parser( internal::file_reader( filename ).read(), filename, from )
      { }
   };

} // tao_json_pegtl

#endif
