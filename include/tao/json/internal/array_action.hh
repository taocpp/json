// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ARRAY_ACTION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ARRAY_ACTION_HH

#include "../external/pegtl/input.hh"
#include "../external/pegtl/nothing.hh"

#include "array_state.hh"
#include "grammar.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule > struct array_action : tao_json_pegtl::nothing< Rule > {};

         template<>
         struct array_action< rules::array::element >
         {
            template< template< typename ... > class Traits >
            static void apply( const tao_json_pegtl::input &, array_state< Traits > & result )
            {
               result.push_back();
            }
         };

      } // internal

   } // json

} // tao

#endif
