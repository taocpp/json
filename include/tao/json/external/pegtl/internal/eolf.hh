// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_EOLF_HH
#define TAO_CPP_PEGTL_INTERNAL_EOLF_HH

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      struct eolf
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT >;

         template< typename Input >
         static bool match( Input & in )
         {
            using eol_t = typename Input::eol_t;
            const auto p = eol_t::match( in );
            return p.first || ( ! p.second );
         }
      };

      template<>
      struct skip_control< eolf > : std::true_type {};

   } // namespace internal

} // namespace tao_json_pegtl

#endif
