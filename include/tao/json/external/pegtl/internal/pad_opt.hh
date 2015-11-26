// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_PAD_OPT_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_PAD_OPT_HH

#include "seq.hh"
#include "opt.hh"
#include "star.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Rule, typename Pad >
      using pad_opt = seq< star< Pad >, opt< Rule, star< Pad > > >;

   } // internal

} // tao_json_pegtl

#endif
