// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_IF_MUST_HH
#define TAO_CPP_PEGTL_INTERNAL_IF_MUST_HH

#include "seq.hh"
#include "must.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Cond, typename ... Thens >
      using if_must = seq< Cond, must< Thens ... > >;

   } // namespace internal

} // namespace tao_json_pegtl

#endif
