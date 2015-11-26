// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_ANALYSIS_COUNTED_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_ANALYSIS_COUNTED_HH

#include "generic.hh"

namespace tao_json_pegtl
{
   namespace analysis
   {
      template< rule_type Type, unsigned Count, typename ... Rules >
      struct counted
            : generic< Count ? Type : rule_type::OPT, Rules ... >
      { };

   } // analysis

} // tao_json_pegtl

#endif
