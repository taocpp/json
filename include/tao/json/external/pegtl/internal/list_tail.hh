// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_LIST_TAIL_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_LIST_TAIL_HH

#include "seq.hh"
#include "list.hh"
#include "opt.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Rule, typename Sep >
      using list_tail = seq< list< Rule, Sep >, opt< Sep > >;

   } // internal

} // tao_json_pegtl

#endif
