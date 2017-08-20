// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_GRAMMAR_HPP

#include <cstdint>

#include "../../internal/action.hpp"
#include "../../internal/control.hpp"
#include "../../internal/endian.hpp"
#include "../../internal/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace ubjson
         {
            struct number
            {
               using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::ANY >;

               template< json_pegtl::apply_mode A,
                         json_pegtl::rewind_mode M,
                         template< typename... > class Action,
                         template< typename... > class Control,
                         typename Input,
                         typename Consumer >
               static bool match_impl( Input& in, Consumer& consumer )
               {
                  if( in.peek_char() == '-' ) {
                     in.bump_in_this_line();
                     if( in.empty() || !internal::rules::sor_value::match_number< true, A, json_pegtl::rewind_mode::DONTCARE, Action, Control >( in, consumer ) ) {
                        throw json_pegtl::parse_error( "incomplete number", in );
                     }
                     return true;
                  }
                  return internal::rules::sor_value::match_number< false, A, M, Action, Control >( in, consumer );
               }

               template< json_pegtl::apply_mode A,
                         json_pegtl::rewind_mode M,
                         template< typename... > class Action,
                         template< typename... > class Control,
                         typename Input,
                         typename Consumer >
               static bool match( Input& in, Consumer& consumer )
               {
                  if( in.size( 2 ) && match_impl< A, M, Action, Control >( in, consumer ) ) {
                     in.discard();
                     return true;
                  }
                  return false;
               }
            };

         }  // namespace ubjson

      }  // namespace events

      namespace internal
      {
         // clang-format off
         template<> WEAK_PREFIX const std::string errors< events::ubjson::number >::error_message WEAK_SUFFIX = "invalid ubjson high-precision number";
         // clang-format on

      }  // namespace internal

      namespace events
      {
         namespace ubjson
         {
            struct data
            {
               using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::ANY >;

               template< json_pegtl::apply_mode A,
                         json_pegtl::rewind_mode M,
                         template< typename... > class Action,
                         template< typename... > class Control,
                         typename Input,
                         typename Consumer >
               static bool match( Input& in, Consumer& consumer )
               {
                  // This rule never returns false unless the input is empty.
                  return ( !in.empty() ) && match_impl( in, consumer );
               }

               template< typename Input, typename Consumer >
               static bool match_impl( Input& in, Consumer& consumer )
               {
                  switch( in.peek_char() ) {
                     case 'Z':
                        consumer.null();
                        in.bump_in_this_line();
                        return true;
                     case 'N':
                        in.bump_in_this_line();
                        return true;
                     case 'T':
                        consumer.boolean( true );
                        in.bump_in_this_line();
                        return true;
                     case 'F':
                        consumer.boolean( false );
                        in.bump_in_this_line();
                        return true;
                     case 'i':
                        consumer.number( read_number< std::int64_t, std::int8_t >( in ) );
                        return true;
                     case 'U':
                        consumer.number( read_number< std::uint64_t, std::uint8_t >( in ) );
                        return true;
                     case 'I':
                        consumer.number( read_number< std::int64_t, std::int16_t >( in ) );
                        return true;
                     case 'l':
                        consumer.number( read_number< std::int64_t, std::int32_t >( in ) );
                        return true;
                     case 'L':
                        consumer.number( read_number< std::int64_t, std::int64_t >( in ) );
                        return true;
                     case 'd':
                        consumer.number( read_number< double, float >( in ) );
                        return true;
                     case 'D':
                        consumer.number( read_number< double, double >( in ) );
                        return true;
                     case 'H':
                        return match_high_precision( in, consumer );
                     case 'C':
                        return match_char( in, consumer );
                     case 'S':
                        consumer.string( read_container< tao::string_view >( in, read_size( in ) ) );
                  }
                  throw json_pegtl::parse_error( "unknown ubjson marker", in );
               }

               template< typename Input, typename Consumer >
               static bool match_char( Input& in, Consumer& consumer )
               {
                  if( ( in.size( 2 ) < 2 ) || ( in.peek_byte( 1 ) > 127 ) ) {
                     throw json_pegtl::parse_error( "invalid ubjson char", in );
                  }
                  consumer.string( std::string( 1, in.peek_char( 1 ) ) );
                  in.bump_in_this_line( 2 );
                  return true;
               }

               template< typename Input >
               static std::size_t read_size( Input& in )
               {
                  in.bump_in_this_line();  // Skip marker byte.
                  if( !in.size( 1 ) ) {
                     throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                  }
                  switch( in.peek_char() ) {
                     case 'i':
                        return read_number< std::int64_t, std::int8_t >( in );
                     case 'U':
                        return read_number< std::int64_t, std::uint8_t >( in );
                     case 'I':
                        return read_number< std::int64_t, std::int16_t >( in );
                     case 'l':
                        return read_number< std::int64_t, std::int32_t >( in );
                     case 'L':
                        return read_number< std::int64_t, std::int64_t >( in );
                     case 'd':
                        return read_number< std::int64_t, float >( in );
                     case 'D':
                        return read_number< std::int64_t, double >( in );
                  }
                  throw json_pegtl::parse_error( "unexpected ubjson high precision number size marker", in );
               }

               template< typename Input, typename Consumer >
               static bool match_high_precision( Input& in, Consumer& consumer )
               {
                  const auto size = read_size( in );
                  if( size < 0 ) {
                     throw json_pegtl::parse_error( "negative ubjson high precision number size", in );
                  }
                  if( in.size( size ) < size ) {
                     throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                  }
                  json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > i2( in.current(), in.current() + size, "UBJSON" );
                  json_pegtl::parse_nested< json_pegtl::must< number, json_pegtl::eof >, internal::action, internal::control >( in, i2, consumer );
                  return true;
               }

               template< typename Result, typename Number, typename Input >
               static Result read_number( Input& in )
               {
                  if( in.size( sizeof( Number ) ) > sizeof( Number ) ) {
                     const Result result = json::internal::be_to_h< Number >( in.current() + 1 );
                     in.bump_in_this_line( 1 + sizeof( Number ) );
                     return result;
                  }
                  throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
               }

               template< typename Result, typename Input >
               static Result read_container( Input& in, const std::size_t size )
               {
                  using value_t = typename Result::value_type;
                  if( in.size( size ) < size ) {
                     throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                  }
                  const value_t* pointer = reinterpret_cast< const value_t* >( in.current() );
                  Result result( pointer, size );
                  in.bump_in_this_line( size );
                  return result;
               }

               template< typename Input >
               static tao::string_view read_key( Input& in )
               {
                  if( in.empty() ) {
                     throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                  }
                  switch( in.peek_byte() ) {
                     case 0xd9:
                        return read_container< tao::string_view >( in, read_number< std::size_t, std::uint8_t >( in ) );
                     case 0xda:
                        return read_container< tao::string_view >( in, read_number< std::size_t, std::uint16_t >( in ) );
                     case 0xdb:
                        return read_container< tao::string_view >( in, read_number< std::size_t, std::uint32_t >( in ) );
                  }
                  throw json_pegtl::parse_error( "unexpected key type", in );
               }

               template< typename Input, typename Consumer >
               static bool match_array( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     if( in.empty() ) {
                        throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                     }
                     match_impl( in, consumer );
                     consumer.element();
                  }
                  consumer.end_array( size );
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_object( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     consumer.key( read_key( in ) );
                     if( in.empty() ) {
                        throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                     }
                     match_impl( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
                  return true;
               }
            };

            struct grammar : json_pegtl::must< data, json_pegtl::eof >
            {
            };

         }  // namespace ubjson

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
