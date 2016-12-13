// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_CONTRIB_CHANGES_HH
#define TAO_CPP_PEGTL_CONTRIB_CHANGES_HH

#include <type_traits>

#include "../normal.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      struct dummy_disabled_state
      {
         template< typename ... Ts >
         void success( Ts && ... )
         { }
      };

      template< tao_json_pegtl::apply_mode A, typename State >
      using state_disable_helper = typename std::conditional< A == tao_json_pegtl::apply_mode::ACTION, State, dummy_disabled_state >::type;

   } // namespace internal

   template< typename Rule, typename State, template< typename ... > class Base = tao_json_pegtl::normal >
   struct change_state
         : public Base< Rule >
   {
      template< tao_json_pegtl::apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         internal::state_disable_helper< A, State > s;

         if ( Base< Rule >::template match< A, Action, Control >( in, s ) ) {
            s.success( st ... );
            return true;
         }
         return false;
      }
   };

   template< typename Rule, template< typename ... > class Action, template< typename ... > class Base = tao_json_pegtl::normal >
   struct change_action
         : public Base< Rule >
   {
      template< tao_json_pegtl::apply_mode A, template< typename ... > class, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         return Base< Rule >::template match< A, Action, Control >( in, st ... );
      }
   };

   template< template< typename ... > class Action, template< typename ... > class Base >
   struct change_both_helper
   {
      template< typename T > using change_action = change_action< T, Action, Base >;
   };

   template< typename Rule, typename State, template< typename ... > class Action, template< typename ... > class Base = tao_json_pegtl::normal >
   struct change_state_and_action
         : public change_state< Rule, State, change_both_helper< Action, Base >::template change_action >
   { };

} // namespace tao_json_pegtl

#endif
