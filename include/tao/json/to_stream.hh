// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TO_STREAM_HH
#define TAOCPP_JSON_INCLUDE_TO_STREAM_HH

#include <ostream>
#include <cstddef>

#include "value.hh"
#include "sax/from_value.hh"
#include "sax/to_stream.hh"
#include "sax/to_pretty_stream.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      void to_stream( std::ostream & os, const basic_value< Traits > & v )
      {
         sax::to_stream consumer( os );
         sax::from_value( v, consumer );
      }

      template< template< typename ... > class Traits >
      void to_stream( std::ostream & os, const basic_value< Traits > & v, const std::size_t indent )
      {
         sax::to_pretty_stream consumer( os, indent );
         sax::from_value( v, consumer );
      }

   } // json

} // tao

#endif
