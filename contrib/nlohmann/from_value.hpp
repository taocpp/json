// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CONTRIB_NLOHMANN_FROM_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_CONTRIB_NLOHMANN_FROM_VALUE_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

namespace tao
{
   namespace json
   {
      namespace nlohmann
      {
         // Events producer from an nlohmann/json value.

         template< typename Value, typename Consumer >
         void from_value( const Value& v, Consumer& consumer )
         {
            switch( v.type() ) {
               case Value::value_t::discarded:
                  throw std::logic_error( "invalid discarded value" );
               case Value::value_t::null:
                  consumer.null();
                  return;
               case Value::value_t::boolean:
                  consumer.boolean( v.template get< bool >() );
                  return;
               case Value::value_t::number_integer:
                  consumer.number( v.template get< std::int64_t >() );
                  return;
               case Value::value_t::number_unsigned:
                  consumer.number( v.template get< std::uint64_t >() );
                  return;
               case Value::value_t::number_float:
                  consumer.number( v.template get< double >() );
                  return;
               case Value::value_t::string:
                  consumer.string( v.template get_ref< const std::string& >() );
                  return;
               case Value::value_t::array:
                  consumer.begin_array();
                  for( const auto& e : v ) {
                     tao::json::nlohmann::from_value( e, consumer );
                     consumer.element();
                  }
                  consumer.end_array();
                  return;
               case Value::value_t::object:
                  consumer.begin_object();
                  for( typename Value::const_iterator it = v.begin(); it != v.end(); ++it ) {
                     consumer.key( it.key() );
                     tao::json::nlohmann::from_value( it.value(), consumer );
                     consumer.member();
                  }
                  consumer.end_object();
                  return;
            }
            throw std::logic_error( "invalid value for type()" );  // LCOV_EXCL_LINE
         }

      }  // nlohmann

   }  // namespace json

}  // namespace tao

#endif
