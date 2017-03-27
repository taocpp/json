// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_FROM_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_FROM_STREAM_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "sax/from_stream.hpp"
#include "sax/to_value.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      basic_value< Traits > from_stream( std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
      {
         sax::to_basic_value< Traits > consumer;
         sax::from_stream( stream, consumer, source, maximum_buffer_size );
         return std::move( consumer.value );
      }

      template< template< typename... > class Traits >
      basic_value< Traits > from_stream( std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
      {
         return from_stream< Traits >( stream, source.c_str(), maximum_buffer_size );
      }

      inline value from_stream( std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
      {
         return from_stream< traits >( stream, source, maximum_buffer_size );
      }

      inline value from_stream( std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
      {
         return from_stream< traits >( stream, source.c_str(), maximum_buffer_size );
      }

      template< template< typename... > class Traits, typename... Ts >
      void from_stream( basic_value< Traits >& output, Ts&&... ts )
      {
         output = from_stream< Traits >( std::forward< Ts >( ts )... );
      }

   }  // json

}  // tao

#endif
