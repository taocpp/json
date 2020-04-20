// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONTRIB_PRETTY_HPP
#define TAO_JSON_PEGTL_CONTRIB_PRETTY_HPP

#include <iostream>

#include "../config.hpp"
#include "../visit.hpp"

#include "pretty_traits.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule >
      struct pretty_impl
      {
         static void visit( std::ostream& os )
         {
            os << internal::demangle< Rule >() << " := ";
            pretty_traits< Rule, typename Rule::rule_t >::print( os, false );
            os << std::endl;
         }
      };

   }  // namespace internal

   template< typename Grammar >
   void pretty( std::ostream& os = std::cout )
   {
      visit< Grammar, internal::pretty_impl >( os );
   }

}  // namespace TAO_JSON_PEGTL_NAMESPACE

#endif
