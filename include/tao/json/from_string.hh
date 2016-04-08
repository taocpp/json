// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_FROM_STRING_HH
#define TAOCPP_JSON_INCLUDE_FROM_STRING_HH

#include "external/pegtl/parse.hh"

#include "internal/result_state.hh"
#include "internal/control.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      inline void from_string( value_base< Traits > & output, const char * data, const size_t size, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         internal::result_state< Traits > result;
         tao_json_pegtl::input input( line, column, data, data + size, source ? source : __PRETTY_FUNCTION__ );
         tao_json_pegtl::parse_input< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< Traits >::template control >( input, result );
         output = std::move( result.result );
      }

      template< template< typename ... > class Traits >
      inline value_base< Traits > from_string( const char * data, const size_t size, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         internal::result_state< Traits > result;
         tao_json_pegtl::input input( line, column, data, data + size, source ? source : __PRETTY_FUNCTION__ );
         tao_json_pegtl::parse_input< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< Traits >::template control >( input, result );
         return std::move( result.result );
      }

      inline value from_string( const char * data, const size_t size, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         internal::result_state< traits > result;
         tao_json_pegtl::input input( line, column, data, data + size, source ? source : __PRETTY_FUNCTION__ );
         tao_json_pegtl::parse_input< internal::grammar, tao_json_pegtl::nothing, internal::control_selector< traits >::template control >( input, result );
         return std::move( result.result );
      }

      template< template< typename ... > class Traits >
      inline void from_string( value_base< Traits > & output, const char * data, const size_t size, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         from_string( output, data, size, source.c_str(), line, column );
      }

      template< template< typename ... > class Traits >
      inline value_base< Traits > from_string( const char * data, const size_t size, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         return from_string< Traits >( data, size, source.c_str(), line, column );
      }

      inline value from_string( const char * data, const size_t size, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         return from_string( data, size, source.c_str(), line, column );
      }

      template< template< typename ... > class Traits >
      inline void from_string( value_base< Traits > & output, const std::string & data, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         from_string( output, data.data(), data.size(), source, line, column );
      }

      template< template< typename ... > class Traits >
      inline value_base< Traits > from_string( const std::string & data, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         return from_string< Traits >( data.data(), data.size(), source, line, column );
      }

      inline value from_string( const std::string & data, const char * source = nullptr, const size_t line = 1, const size_t column = 0 )
      {
         return from_string( data.data(), data.size(), source, line, column );
      }

      template< template< typename ... > class Traits >
      inline void from_string( value_base< Traits > & output, const std::string & data, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         from_string( output, data.data(), data.size(), source.c_str(), line, column );
      }

      template< template< typename ... > class Traits >
      inline value_base< Traits > from_string( const std::string & data, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         return from_string< Traits >( data.data(), data.size(), source.c_str(), line, column );
      }

      inline value from_string( const std::string & data, const std::string & source, const size_t line = 1, const size_t column = 0 )
      {
         return from_string( data.data(), data.size(), source.c_str(), line, column );
      }

   } // json

} // tao

#endif
