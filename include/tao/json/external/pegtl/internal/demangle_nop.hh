// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_DEMANGLE_NOP_HH
#define TAO_CPP_PEGTL_INTERNAL_DEMANGLE_NOP_HH

#include <string>

namespace tao_json_pegtl
{
   namespace internal
   {
      inline std::string demangle( const char * symbol )
      {
         return symbol;
      }

   } // namespace internal

} // namespace tao_json_pegtl

#endif
