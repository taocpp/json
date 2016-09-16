// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_NLOHMANN_TRAVERSE_VALUE_HH
#define TAOCPP_JSON_INCLUDE_JSON_NLOHMANN_TRAVERSE_VALUE_HH

#include <cstdint>
#include <string>
#include <stdexcept>

namespace tao
{
   namespace json
   {
      namespace nlohmann
      {
         // SAX producer for an nlohmann/json value
         template< typename Value, typename Handler >
         void traverse_value( const Value & v, Handler & handler )
         {
            switch( v.type() ) {
               case Value::value_t::null:
                  handler.null();
                  break;
               case Value::value_t::boolean:
                  handler.boolean( v.get< bool >() );
                  break;
               case Value::value_t::number_integer:
                  handler.number( v.get< std::int64_t >() );
                  break;
               case Value::value_t::number_unsigned:
                  handler.number( v.get< std::uint64_t >() );
                  break;
               case Value::value_t::number_float:
                  handler.number( v.get< double >() );
                  break;
               case Value::value_t::string:
                  handler.string( v.get_ref< const std::string & >() );
                  break;
               case Value::value_t::array:
                  handler.begin_array();
                  for( const auto & e : v ) {
                     tao::json::nlohmann::traverse_value( e, handler );
                     handler.element();
                  }
                  handler.end_array();
                  break;
               case Value::value_t::object:
                  handler.begin_object();
                  for( Value::const_iterator it = v.begin(); it != v.end(); ++it ) {
                     handler.key( it.key() );
                     tao::json::nlohmann::traverse_value( it.value(), handler );
                     handler.member();
                  }
                  handler.end_object();
                  break;
               default:
                  throw std::logic_error( "invalid value for type()" );  // LCOV_EXCL_LINE
            }
         }

      } // nlohmann

   } // json

} // tao

#endif
