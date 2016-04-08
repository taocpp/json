// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_RESULT_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_RESULT_STATE_HH

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits >
         struct result_state
         {
            result_state() = default;
            result_state( const result_state & ) = delete;
            void operator= ( const result_state & ) = delete;

            value_base< Traits > result;
         };

      } // internal

   } // json

} // tao

#endif
