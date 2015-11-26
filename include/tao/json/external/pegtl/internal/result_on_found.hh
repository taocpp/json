// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_RESULT_ON_FOUND_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_RESULT_ON_FOUND_HH

namespace tao_json_pegtl
{
   namespace internal
   {
      enum class result_on_found : bool
      {
         SUCCESS = true,
         FAILURE = false
      };

   } // internal

} // tao_json_pegtl

#endif
