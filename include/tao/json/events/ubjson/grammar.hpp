// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_UBJSON_GRAMMAR_HPP

#include <cstdint>

#include "../../external/string_view.hpp"
#include "../../internal/action.hpp"
#include "../../internal/control.hpp"
#include "../../internal/endian.hpp"
#include "../../internal/grammar.hpp"
#include "../../utf8.hpp"

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
                     if( in.empty() || !json::internal::rules::sor_value::match_number< true, A, json_pegtl::rewind_mode::DONTCARE, Action, Control >( in, consumer ) ) {
                        throw json_pegtl::parse_error( "incomplete number", in );
                     }
                     return true;
                  }
                  return json::internal::rules::sor_value::match_number< false, A, M, Action, Control >( in, consumer );
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
         template<> WEAK_PREFIX const std::string errors< events::ubjson::number >::error_message WEAK_SUFFIX = "invalid ubjson high-precision number";  // NOLINT
         // clang-format on

      }  // namespace internal

      namespace events
      {
         namespace ubjson
         {
            namespace internal
            {
               template< typename Input >
               void throw_on_empty( Input& in, const std::size_t required = 1 )
               {
                  if( in.size( required ) < required ) {
                     throw json_pegtl::parse_error( "unexpected end of ubjson input", in );
                  }
               }

               template< typename Input >
               char read_char_unsafe( Input& in )
               {
                  const auto r = in.peek_char();
                  in.bump_in_this_line( 1 );
                  return r;
               }

               template< typename Input >
               char peek_char_safe( Input& in )
               {
                  throw_on_empty( in );
                  return in.peek_char();
               }

               template< typename Input >
               char read_char_safe( Input& in )
               {
                  throw_on_empty( in );
                  const auto r = in.peek_char();
                  in.bump_in_this_line( 1 );
                  return r;
               }

            }  // namespace internal

            template< utf8_mode V >
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

               template< typename Result, typename Number, typename Input >
               static Result read_number( Input& in )
               {
                  internal::throw_on_empty( in, sizeof( Number ) );
                  const auto r = static_cast< Result >( json::internal::be_to_h< Number >( in.current() ) );
                  in.bump_in_this_line( sizeof( Number ) );
                  return r;
               }

               template< typename Input, typename Consumer >
               static bool match_impl( Input& in, Consumer& consumer )
               {
                  return match_impl_impl( in, consumer, internal::read_char_unsafe( in ) );
               }

               template< typename Input, typename Consumer >
               static bool match_impl_impl( Input& in, Consumer& consumer, const char c )
               {
                  switch( c ) {
                     case 'Z':
                        consumer.null();
                        break;
                     case 'T':
                        consumer.boolean( true );
                        break;
                     case 'F':
                        consumer.boolean( false );
                        break;
                     case 'i':
                        consumer.number( read_number< std::int64_t, std::int8_t >( in ) );
                        break;
                     case 'U':
                        consumer.number( read_number< std::uint64_t, std::uint8_t >( in ) );
                        break;
                     case 'I':
                        consumer.number( read_number< std::int64_t, std::int16_t >( in ) );
                        break;
                     case 'l':
                        consumer.number( read_number< std::int64_t, std::int32_t >( in ) );
                        break;
                     case 'L':
                        consumer.number( read_number< std::int64_t, std::int64_t >( in ) );
                        break;
                     case 'd':
                        consumer.number( read_number< double, float >( in ) );
                        break;
                     case 'D':
                        consumer.number( read_number< double, double >( in ) );
                        break;
                     case 'H':
                        match_high_precision( in, consumer );
                        break;
                     case 'C':
                        match_char( in, consumer );
                        break;
                     case 'S':
                        consumer.string( read_container< V, tao::string_view >( in, read_size( in ) ) );
                        break;
                     case '[':
                        match_array( in, consumer );
                        break;
                     case '{':
                        match_object( in, consumer );
                        break;
                     default:
                        throw json_pegtl::parse_error( "unknown ubjson type " + std::to_string( unsigned( c ) ), in );
                  }
                  return true;
               }

               template< typename Input, typename Consumer >
               static void match_char( Input& in, Consumer& consumer )
               {
                  if( in.empty() || ( in.peek_byte( 0 ) > 127 ) ) {
                     throw json_pegtl::parse_error( "missing or invalid ubjson char", in );
                  }
                  consumer.string( std::string( 1, in.peek_char( 0 ) ) );
                  in.bump_in_this_line( 1 );
               }

               template< typename Input >
               static std::int64_t read_size_impl( Input& in )
               {
                  switch( const auto c = internal::read_char_safe( in ) ) {
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
                     default:
                        throw json_pegtl::parse_error( "unknown ubjson size type " + std::to_string( unsigned( c ) ), in );
                  }
               }

               template< typename Input >
               static std::size_t read_size( Input& in )
               {
                  const auto s = read_size_impl( in );
                  if( s < 0 ) {
                     throw json_pegtl::parse_error( "negative ubjson size " + std::to_string( s ), in );
                  }
                  const auto t = static_cast< std::uint64_t >( s );
                  if( t > static_cast< std::uint64_t >( std::numeric_limits< std::size_t >::max() ) ) {
                     throw json_pegtl::parse_error( "ubjson size exceeds size_t " + std::to_string( t ), in );
                  }
                  return static_cast< std::size_t >( t );
               }

               template< typename Input, typename Consumer >
               static void match_high_precision( Input& in, Consumer& consumer )
               {
                  const auto size = read_size( in );
                  internal::throw_on_empty( in, size );
                  json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > i2( in.current(), in.current() + size, "UBJSON" );
                  json_pegtl::parse_nested< json_pegtl::must< number, json_pegtl::eof >, json::internal::action, json::internal::control >( in, i2, consumer );
                  in.bump_in_this_line( size );
               }

               template< utf8_mode U, typename Result, typename Input >
               static Result read_container( Input& in, const std::size_t size )
               {
                  using value_t = typename Result::value_type;
                  internal::throw_on_empty( in, size );
                  const auto* pointer = static_cast< const value_t* >( static_cast< const void* >( in.current() ) );
                  Result result( pointer, size );
                  json::internal::consume_utf8< U >( in, size );
                  return result;
               }

               template< typename Input, typename Consumer >
               static void match_array( Input& in, Consumer& consumer )
               {
                  switch( internal::peek_char_safe( in ) ) {
                     case '#':
                        in.bump_in_this_line( 1 );
                        match_sized_array( in, consumer );
                        break;
                     case '$':
                        in.bump_in_this_line( 1 );
                        match_typed_array( in, consumer );
                        break;
                     default:
                        match_basic_array( in, consumer );
                        break;
                  }
               }

               template< typename Input, typename Consumer >
               static void match_sized_array( Input& in, Consumer& consumer )
               {
                  const auto size = read_size( in );
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     internal::throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void match_typed_array( Input& in, Consumer& consumer )
               {
                  const auto c = internal::read_char_safe( in );
                  if( internal::read_char_safe( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson array type not followed by size", in );
                  }
                  const auto size = read_size( in );
                  switch( c ) {
                     case 'Z':
                        consumer.begin_array( size );
                        for( std::size_t i = 0; i < size; ++i ) {
                           consumer.null();
                           consumer.element();
                        }
                        consumer.end_array( size );
                        break;
                     case 'T':
                        consumer.begin_array( size );
                        for( std::size_t i = 0; i < size; ++i ) {
                           consumer.boolean( true );
                           consumer.element();
                        }
                        consumer.end_array( size );
                        break;
                     case 'F':
                        consumer.begin_array( size );
                        for( std::size_t i = 0; i < size; ++i ) {
                           consumer.boolean( false );
                           consumer.element();
                        }
                        consumer.end_array( size );
                        break;
                     case 'U':
                        // NOTE: UBJSON encodes binary data as 'strongly typed array of uint8 values'.
                        consumer.binary( read_container< utf8_mode::TRUST, tao::byte_view >( in, size ) );
                        break;
                     case 'i':
                     case 'I':
                     case 'l':
                     case 'L':
                     case 'd':
                     case 'D':
                     case 'H':
                     case 'C':
                     case 'S':
                     case '[':
                     case '{':
                        consumer.begin_array( size );
                        for( std::size_t i = 0; i < size; ++i ) {
                           internal::throw_on_empty( in );
                           match_impl_impl( in, consumer, c );
                           consumer.element();
                        }
                        consumer.end_array( size );
                        break;
                     default:
                        throw json_pegtl::parse_error( "unknown ubjson array type " + std::to_string( unsigned( c ) ), in );
                  }
               }

               template< typename Input, typename Consumer >
               static void match_basic_array( Input& in, Consumer& consumer )
               {
                  consumer.begin_array();
                  while( internal::peek_char_safe( in ) != ']' ) {
                     match_impl( in, consumer );
                     consumer.element();
                  }
                  in.bump_in_this_line( 1 );
                  consumer.end_array();
               }

               template< typename Input, typename Consumer >
               static void match_key( Input& in, Consumer& consumer )
               {
                  consumer.key( read_container< V, tao::string_view >( in, read_size( in ) ) );
               }

               template< typename Input, typename Consumer >
               static void match_object( Input& in, Consumer& consumer )
               {
                  switch( internal::peek_char_safe( in ) ) {
                     case '#':
                        in.bump_in_this_line( 1 );
                        match_sized_object( in, consumer );
                        break;
                     case '$':
                        in.bump_in_this_line( 1 );
                        match_typed_object( in, consumer );
                        break;
                     default:
                        match_basic_object( in, consumer );
                        break;
                  }
               }

               template< typename Input, typename Consumer >
               static void match_sized_object( Input& in, Consumer& consumer )
               {
                  const auto size = read_size( in );
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     match_key( in, consumer );
                     internal::throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void match_typed_object( Input& in, Consumer& consumer )
               {
                  const auto c = internal::read_char_safe( in );
                  if( internal::read_char_safe( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson object type not followed by size", in );
                  }
                  const auto size = read_size( in );
                  consumer.begin_object( size );
                  switch( c ) {
                     case 'Z':
                        for( std::size_t i = 0; i < size; ++i ) {
                           match_key( in, consumer );
                           consumer.null();
                           consumer.member();
                        }
                        break;
                     case 'T':
                        for( std::size_t i = 0; i < size; ++i ) {
                           match_key( in, consumer );
                           consumer.boolean( true );
                           consumer.member();
                        }
                        break;
                     case 'F':
                        for( std::size_t i = 0; i < size; ++i ) {
                           match_key( in, consumer );
                           consumer.boolean( false );
                           consumer.member();
                        }
                        break;
                     case 'i':
                     case 'U':
                     case 'I':
                     case 'l':
                     case 'L':
                     case 'd':
                     case 'D':
                     case 'H':
                     case 'C':
                     case 'S':
                     case '[':
                     case '{':
                        for( std::size_t i = 0; i < size; ++i ) {
                           match_key( in, consumer );
                           match_impl_impl( in, consumer, c );
                           consumer.member();
                        }
                        break;
                     default:
                        throw json_pegtl::parse_error( "unknown ubjson object type " + std::to_string( unsigned( c ) ), in );
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void match_basic_object( Input& in, Consumer& consumer )
               {
                  consumer.begin_object();
                  while( internal::peek_char_safe( in ) != '}' ) {
                     match_key( in, consumer );
                     internal::throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.member();
                  }
                  in.bump_in_this_line( 1 );
                  consumer.end_object();
               }
            };

            struct nops : json_pegtl::star< json_pegtl::one< 'N' > >
            {
            };

            template< utf8_mode V >
            struct basic_grammar : json_pegtl::must< nops, data< V >, nops, json_pegtl::eof >
            {
            };

            // UBJSON no-ops 'N' are currently only supported as top-level padding around an actual value.

            using grammar = basic_grammar< utf8_mode::CHECK >;

         }  // namespace ubjson

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
