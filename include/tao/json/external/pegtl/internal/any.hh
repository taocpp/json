// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_ANY_HH
#define TAO_CPP_PEGTL_INTERNAL_ANY_HH

#include "skip_control.hh"
#include "peek_char.hh"

#include "../analysis/generic.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename Peek > struct any;

      template< typename Peek >
      struct skip_control< any< Peek > > : std::true_type {};

      template<>
      struct any< peek_char >
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               in.bump();
               return true;
            }
            return false;
         }
      };

      template< typename Peek >
      struct any
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  in.bump( t.size );
                  return true;
               }
            }
            return false;
         }
      };

   } // namespace internal

} // namespace tao_json_pegtl

#endif
