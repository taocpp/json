// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH

#include <ostream>

#include "../external/double.hh"
#include "../value.hh"

#include "escape.hh"
#include "value_writer.hh"
#include "pretty_writer.hh"
#include "write.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const basic_value< Traits > & v )
         {
            internal::value_writer writer( o );
            internal::write( v, writer );
         }

         template< template< typename ... > class Traits >
         void to_stream( std::ostream & o, const basic_value< Traits > & v, const unsigned indent )
         {
            internal::pretty_writer writer( o, indent );
            internal::write( v, writer );
         }

      } // internal

   } // json

} // tao

#endif
