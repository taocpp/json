// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_EOL_HH
#define TAO_CPP_PEGTL_INTERNAL_EOL_HH

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      struct eol
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            using eol_t = typename Input::eol_t;
            return eol_t::match( in ).first;
         }
      };

      template<>
      struct skip_control< eol > : std::true_type {};

   } // namespace internal

} // namespace tao_json_pegtl

#endif
