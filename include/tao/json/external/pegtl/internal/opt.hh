// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_OPT_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_OPT_HH

#include "skip_control.hh"
#include "seq.hh"
#include "trivial.hh"

#include "../analysis/generic.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename ... Rules > struct opt;

      template< typename ... Rules >
      struct skip_control< opt< Rules ... > > : std::true_type {};

      template<>
      struct opt<>
            : trivial< true > {};

      template< typename ... Rules >
      struct opt
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT, Rules ... >;

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return in.empty() || rule_match_three< seq< Rules ... >, A, Action, Control >::match( in, st ... ) || true;
         }
      };

   } // internal

} // tao_json_pegtl

#endif
