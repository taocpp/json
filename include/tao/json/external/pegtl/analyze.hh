// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_ANALYZE_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_ANALYZE_HH

#include "analysis/analyze_cycles.hh"

namespace tao_json_pegtl
{
   template< typename Rule >
   std::size_t analyze( const bool verbose = true )
   {
      return analysis::analyze_cycles< Rule >( verbose ).problems();
   }

} // tao_json_pegtl

#endif
