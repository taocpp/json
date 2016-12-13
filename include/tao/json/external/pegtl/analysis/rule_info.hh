// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_ANALYSIS_RULE_INFO_HH
#define TAO_CPP_PEGTL_ANALYSIS_RULE_INFO_HH

#include <string>
#include <vector>

#include "rule_type.hh"

namespace tao_json_pegtl
{
   namespace analysis
   {
      struct rule_info
      {
         explicit
         rule_info( const rule_type in_type )
               : type( in_type )
         { }

         rule_type type;
         std::vector< std::string > rules;
      };

   } // namespace analysis

} // namespace tao_json_pegtl

#endif
