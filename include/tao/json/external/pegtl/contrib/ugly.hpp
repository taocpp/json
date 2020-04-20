// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONTRIB_UGLY_HPP
#define TAO_JSON_PEGTL_CONTRIB_UGLY_HPP

#include <iostream>

#include "../config.hpp"
#include "../visit.hpp"

#include "../internal/demangle.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule >
      struct ugly_one_impl
      {
         static void visit( std::ostream& os )
         {
            os << demangle< Rule >() << std::endl;
         }
      };

      template< typename Rule >
      struct ugly_two_impl
      {
         static void visit( std::ostream& os )
         {
            const auto first = demangle< Rule >();
            const auto second = demangle< typename Rule::rule_t >();

            os << first;
            if( first != second ) {
               os << " => "
                         << second;
            }
            os << std::endl;
         }
      };

   }  // namespace internal

   template< typename Grammar >
   void ugly_one( std::ostream& os = std::cout )
   {
      visit< Grammar, internal::ugly_one_impl >( os );
   }

   template< typename Grammar >
   void ugly_two( std::ostream& os = std::cout )
   {
      visit< Grammar, internal::ugly_two_impl >( os );
   }

}  // namespace TAO_JSON_PEGTL_NAMESPACE

#endif
