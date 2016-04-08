// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_OBJECT_ACTION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_OBJECT_ACTION_HH

#include "../external/pegtl/input.hh"

#include "unescape_action.hh"
#include "object_state.hh"
#include "grammar.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template < typename Rule > struct object_action : unescape_action< Rule > {};

         template<>
         struct object_action< rules::object::element >
         {
            template< template< typename ... > class Traits >
            static void apply( const tao_json_pegtl::input &, object_state< Traits > & result )
            {
               result.insert();
            }
         };

      } // internal

   } // json

} // tao

#endif
