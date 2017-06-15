// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JAXN_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_JAXN_TO_STREAM_HPP

#include <cstddef>
#include <ostream>

#include "../events/from_value.hpp"
#include "../events/jaxn/to_pretty_stream.hpp"
#include "../events/jaxn/to_stream.hpp"
#include "../value.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class Traits >
         void to_stream( std::ostream& os, const basic_value< Traits >& v )
         {
            events::jaxn::to_stream consumer( os );
            events::from_value( v, consumer );
         }

         template< template< typename... > class Traits >
         void to_stream( std::ostream& os, const basic_value< Traits >& v, const std::size_t indent )
         {
            events::jaxn::to_pretty_stream consumer( os, indent );
            events::from_value( v, consumer );
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
