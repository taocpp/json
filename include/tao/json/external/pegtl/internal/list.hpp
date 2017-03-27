// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_LIST_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_LIST_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "star.hpp"

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule, typename Sep >
         using list = seq< Rule, star< Sep, Rule > >;

      }  // namespace internal

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
