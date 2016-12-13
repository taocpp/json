// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_REP_MIN_HH
#define TAO_CPP_PEGTL_INTERNAL_REP_MIN_HH

#include "seq.hh"
#include "rep.hh"
#include "star.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< unsigned Min, typename Rule, typename ... Rules >
      using rep_min = seq< rep< Min, Rule, Rules ... >, star< Rule, Rules ... > >;

   } // namespace internal

} // namespace tao_json_pegtl

#endif
