// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP
#define TAOCPP_JSON_INCLUDE_CBOR_FROM_STRING_HPP

#include <cstddef>
#include <string>

#include "../value.hpp"

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "../events/cbor/from_string.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< template< typename... > class... Transformers >
         data from_string( const char* data, const std::size_t size, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            events::transformer< events::to_value, Transformers... > consumer;
            events::cbor::from_string( data, size, consumer, source, byte, line, column );
            return std::move( consumer.value );
         }

         template< template< typename... > class... Transformers >
         data from_string( const char* data, const std::size_t size, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Transformers... >( data, size, source.c_str(), byte, line, column );
         }

         template< template< typename... > class... Transformers >
         data from_string( const std::string& data, const char* source = nullptr, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Transformers... >( data.data(), data.size(), source, byte, line, column );
         }

         template< template< typename... > class... Transformers >
         data from_string( const std::string& data, const std::string& source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t column = 0 )
         {
            return from_string< Transformers... >( data.data(), data.size(), source.c_str(), byte, line, column );
         }

         template< template< typename... > class Traits, template< typename... > class... Transformers, typename... Ts >
         basic_custom_value< Traits > basic_custom_from_string( Ts&&... ts )
         {
            return from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

         template< template< typename... > class... Transformers, typename... Ts >
         custom_value custom_from_string( Ts&&... ts )
         {
            return from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

         template< template< typename... > class... Transformers, template< typename... > class Traits, typename... Ts >
         void from_string( basic_custom_value< Traits >& output, Ts&&... ts )
         {
            output = from_string< Transformers... >( std::forward< Ts >( ts )... );
         }

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
