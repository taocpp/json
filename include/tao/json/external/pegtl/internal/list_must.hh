// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_LIST_MUST_HH
#define TAO_CPP_PEGTL_INTERNAL_LIST_MUST_HH

#include "seq.hh"
#include "star.hh"
#include "must.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Rule, typename Sep >
      using list_must = seq< Rule, star< Sep, must< Rule > > >;

   } // namespace internal

} // namespace tao_json_pegtl

#endif
