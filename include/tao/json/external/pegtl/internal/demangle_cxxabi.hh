// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_DEMANGLE_CXXABI_HH
#define TAO_CPP_PEGTL_INTERNAL_DEMANGLE_CXXABI_HH

#include <string>
#include <memory>
#include <cstdlib>
#include <cxxabi.h>

namespace tao_json_pegtl
{
   namespace internal
   {
      inline std::string demangle( const char * symbol )
      {
         const std::unique_ptr< char, decltype( & std::free ) > demangled( abi::__cxa_demangle( symbol, nullptr, nullptr, nullptr ), & std::free );
         return demangled ? demangled.get() : symbol;
      }

   } // namespace internal

} // namespace tao_json_pegtl

#endif
