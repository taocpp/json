// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_TRACKING_MODE_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_TRACKING_MODE_HPP

#include "config.hpp"

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      enum class tracking_mode : bool
      {
         IMMEDIATE,
         LAZY
      };

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
