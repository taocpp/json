// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_ALNUM_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_ALNUM_HPP

#include "../config.hpp"

#include "peek_char.hpp"
#include "ranges.hpp"

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      namespace internal
      {
         using alnum = ranges< peek_char, 'a', 'z', 'A', 'Z', '0', '9' >;

      }  // namespace internal

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
