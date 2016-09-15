// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_FROM_STRING_HH
#define TAOCPP_JSON_INCLUDE_FROM_STRING_HH

#include <cstddef>
#include <utility>
#include <string>

#include "sax/from_string.hh"
#include "sax/to_value.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      inline basic_value< Traits > from_string( const char * data, const std::size_t size, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
      {
         sax::to_basic_value< Traits > handler;
         sax::from_string( data, size, handler, source, line, column );
         return std::move( handler.value );
      }

      template< template< typename ... > class Traits >
      inline basic_value< Traits > from_string( const char * data, const std::size_t size, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return from_string< Traits >( data, size, source.c_str(), line, column );
      }

      inline value from_string( const char * data, const std::size_t size, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return from_string< traits >( data, size, source, line, column );
      }

      inline value from_string( const char * data, const std::size_t size, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return from_string< traits >( data, size, source.c_str(), line, column );
      }

      template< template< typename ... > class Traits >
      inline basic_value< Traits > from_string( const std::string & data, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return from_string< Traits >( data.data(), data.size(), source, line, column );
      }

      template< template< typename ... > class Traits >
      inline basic_value< Traits > from_string( const std::string & data, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return from_string< Traits >( data.data(), data.size(), source.c_str(), line, column );
      }

      inline value from_string( const std::string & data, const char * source = nullptr, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return json::from_string( data.data(), data.size(), source, line, column );
      }

      inline value from_string( const std::string & data, const std::string & source, const std::size_t line = 1, const std::size_t column = 0 )
      {
         return json::from_string( data.data(), data.size(), source.c_str(), line, column );
      }

      template< template< typename ... > class Traits, typename ... Ts >
      inline void from_string( basic_value< Traits > & output, Ts && ... ts )
      {
         output = from_string< Traits >( std::forward< Ts >( ts ) ... );
      }

      inline namespace literals
      {
         inline value operator"" _json( const char * data, const std::size_t size )
         {
            return json::from_string( data, size, "literal" );
         }

      } // literals

   } // json

} // tao

#endif
