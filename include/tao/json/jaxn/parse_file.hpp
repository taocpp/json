// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JAXN_PARSE_FILE_HPP
#define TAOCPP_JSON_INCLUDE_JAXN_PARSE_FILE_HPP

#include <string>
#include <utility>

#include "../events/jaxn/parse_file.hpp"
#include "../events/to_value.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class Traits >
         basic_value< Traits > parse_file( const std::string& filename )
         {
            events::to_basic_value< Traits > consumer;
            events::jaxn::parse_file( filename, consumer );
            return std::move( consumer.value );
         }

         template< template< typename... > class Traits >
         void parse_file( basic_value< Traits >& output, const std::string& filename )
         {
            output = parse_file< Traits >( filename );
         }

         inline value parse_file( const std::string& filename )
         {
            return parse_file< traits >( filename );
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
