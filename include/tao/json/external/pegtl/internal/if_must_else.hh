// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_IF_MUST_ELSE_HH
#define TAO_CPP_PEGTL_INTERNAL_IF_MUST_ELSE_HH

#include "if_then_else.hh"
#include "must.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Cond, typename Then, typename Else >
      using if_must_else = if_then_else< Cond, must< Then >, must< Else > >;

   } // namespace internal

} // namespace tao_json_pegtl

#endif
