// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ARRAY_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ARRAY_STATE_HH

#include <utility>

#include "../value.hh"

#include "result_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct array_state
               : public result_state
         {
            value array;

            array_state()
                  : array( empty_array )
            { }

            void push_back()
            {
               array.emplace_back( std::move( this->result ) );
            }

            void success( result_state & r )
            {
               r.result = std::move( array );
            }
         };

      } // internal

   } // json

} // tao

#endif
