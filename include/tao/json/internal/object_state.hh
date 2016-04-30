// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_OBJECT_STATE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_OBJECT_STATE_HH

#include <string>
#include <utility>

#include "../value.hh"

#include "result_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits >
         struct object_state
               : public result_state< Traits >
         {
            basic_value< Traits > object;

            std::string unescaped;

            object_state()
                  : object( empty_object )
            { }

            void insert()
            {
               object.emplace( std::move( unescaped ), std::move( this->result ) );
            }

            void success( result_state< Traits > & r )
            {
               r.result = std::move( object );
            }
         };

      } // internal

   } // json

} // tao

#endif
