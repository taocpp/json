// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_ANALYZE_HH
#define TAO_CPP_PEGTL_ANALYZE_HH

#include "analysis/analyze_cycles.hh"

namespace tao_json_pegtl
{
   template< typename Rule >
   std::size_t analyze( const bool verbose = true )
   {
      return analysis::analyze_cycles< Rule >( verbose ).problems();
   }

} // namespace tao_json_pegtl

#endif
