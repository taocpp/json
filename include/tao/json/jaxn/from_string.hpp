// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JAXN_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_JAXN_FROM_STRING_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "../events/jaxn/from_string.hpp"
#include "../events/to_value.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class Traits >
         basic_value< Traits > from_string( const char* data, const std::size_t size, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            events::to_basic_value< Traits > consumer;
            events::jaxn::from_string( data, size, consumer, source, byte, line, column );
            return std::move( consumer.value );
         }

         template< template< typename... > class Traits >
         basic_value< Traits > from_string( const char* data, const std::size_t size, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Traits >( data, size, source.c_str(), byte, line, column );
         }

         inline value from_string( const char* data, const std::size_t size, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< traits >( data, size, source, byte, line, column );
         }

         inline value from_string( const char* data, const std::size_t size, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< traits >( data, size, source.c_str(), byte, line, column );
         }

         template< template< typename... > class Traits >
         basic_value< Traits > from_string( const std::string& data, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Traits >( data.data(), data.size(), source, byte, line, column );
         }

         template< template< typename... > class Traits >
         basic_value< Traits > from_string( const std::string& data, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Traits >( data.data(), data.size(), source.c_str(), byte, line, column );
         }

         inline value from_string( const std::string& data, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return jaxn::from_string( data.data(), data.size(), source, byte, line, column );
         }

         inline value from_string( const std::string& data, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return jaxn::from_string( data.data(), data.size(), source.c_str(), byte, line, column );
         }

         template< template< typename... > class Traits, typename... Ts >
         void from_string( basic_value< Traits >& output, Ts&&... ts )
         {
            output = from_string< Traits >( std::forward< Ts >( ts )... );
         }

         inline namespace literals
         {
            inline value operator"" _jaxn( const char* data, const std::size_t size )
            {
               return jaxn::from_string( data, size, "literal" );
            }

         }  // namespace literals

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
