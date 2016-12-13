// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_ANALYSIS_COUNTED_HH
#define TAO_CPP_PEGTL_ANALYSIS_COUNTED_HH

#include "generic.hh"

namespace tao_json_pegtl
{
   namespace analysis
   {
      template< rule_type Type, unsigned Count, typename ... Rules >
      struct counted
            : generic< ( Count != 0 ) ? Type : rule_type::OPT, Rules ... >
      { };

   } // namespace analysis

} // namespace tao_json_pegtl

#endif
