// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_UBJSON_INTERNAL_GRAMMAR_HPP

#include <cstdint>

#include "../../byte_view.hpp"
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
      namespace ubjson
      {
         namespace internal
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

         }  // namespace internal

      }  // namespace ubjson

      namespace internal
      {
         // clang-format off
         template<> WEAK_PREFIX const std::string errors< ubjson::internal::number >::error_message WEAK_SUFFIX = "invalid ubjson high-precision number";  // NOLINT
         // clang-format on

      }  // namespace internal

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

            template< typename Result, typename Number, typename Input >
            Result parse_number( Input& in )
            {
               throw_on_empty( in, sizeof( Number ) );
               const auto r = static_cast< Result >( json::internal::be_to_h< Number >( in.current() ) );
               in.bump_in_this_line( sizeof( Number ) );
               return r;
            }

            template< typename Input >
            std::int64_t parse_size_impl( Input& in )
            {
               switch( const auto c = read_char_safe( in ) ) {
                  case 'i':
                     return parse_number< std::int64_t, std::int8_t >( in );
                  case 'U':
                     return parse_number< std::int64_t, std::uint8_t >( in );
                  case 'I':
                     return parse_number< std::int64_t, std::int16_t >( in );
                  case 'l':
                     return parse_number< std::int64_t, std::int32_t >( in );
                  case 'L':
                     return parse_number< std::int64_t, std::int64_t >( in );
                  default:
                     throw json_pegtl::parse_error( "unknown ubjson size type " + std::to_string( unsigned( c ) ), in );
               }
            }

            template< std::size_t L, typename Input >
            std::size_t parse_size( Input& in )
            {
               const auto s = parse_size_impl( in );
               if( s < 0 ) {
                  throw json_pegtl::parse_error( "negative ubjson size " + std::to_string( s ), in );
               }
               const auto t = static_cast< std::uint64_t >( s );
               if( t > static_cast< std::uint64_t >( L ) ) {
                  throw json_pegtl::parse_error( "ubjson size exceeds limit " + std::to_string( t ), in );
               }
               return static_cast< std::size_t >( t );
            }

            template< std::size_t L, utf8_mode U, typename Result, typename Input >
            Result parse_container( Input& in )
            {
               const auto size = parse_size< L >( in );
               using value_t = typename Result::value_type;
               throw_on_empty( in, size );
               const auto* pointer = static_cast< const value_t* >( static_cast< const void* >( in.current() ) );
               Result result( pointer, size );
               json::internal::consume_utf8< U >( in, size );
               return result;
            }

            template< std::size_t L, utf8_mode V >
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
                  return match_impl_impl( in, consumer, read_char_unsafe( in ) );
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
                        consumer.number( parse_number< std::int64_t, std::int8_t >( in ) );
                        break;
                     case 'U':
                        consumer.number( parse_number< std::uint64_t, std::uint8_t >( in ) );
                        break;
                     case 'I':
                        consumer.number( parse_number< std::int64_t, std::int16_t >( in ) );
                        break;
                     case 'l':
                        consumer.number( parse_number< std::int64_t, std::int32_t >( in ) );
                        break;
                     case 'L':
                        consumer.number( parse_number< std::int64_t, std::int64_t >( in ) );
                        break;
                     case 'd':
                        consumer.number( parse_number< double, float >( in ) );
                        break;
                     case 'D':
                        consumer.number( parse_number< double, double >( in ) );
                        break;
                     case 'H':
                        match_high_precision( in, consumer );
                        break;
                     case 'C':
                        match_char( in, consumer );
                        break;
                     case 'S':
                        consumer.string( parse_container< L, V, tao::string_view >( in ) );
                        break;
                     case '[':
                        match_array( in, consumer );
                        break;
                     case '{':
                        match_object( in, consumer );
                        break;
                     case 'N':
                        throw json_pegtl::parse_error( "unsupported ubjson no-op", in );
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

               template< typename Input, typename Consumer >
               static void match_high_precision( Input& in, Consumer& consumer )
               {
                  const auto size = parse_size< L >( in );
                  throw_on_empty( in, size );
                  json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > i2( in.current(), in.current() + size, "UBJSON" );
                  json_pegtl::parse_nested< json_pegtl::must< number, json_pegtl::eof >, json::internal::action, json::internal::control >( in, i2, consumer );
                  in.bump_in_this_line( size );
               }

               template< typename Input, typename Consumer >
               static void match_array( Input& in, Consumer& consumer )
               {
                  switch( peek_char_safe( in ) ) {
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
                  const auto size = parse_size< L >( in );
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void match_typed_array( Input& in, Consumer& consumer )
               {
                  const auto c = read_char_safe( in );
                  if( read_char_safe( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson array type not followed by size", in );
                  }
                  if( c == 'U' ) {
                     // NOTE: UBJSON encodes binary data as 'strongly typed array of uint8 values'.
                     consumer.binary( parse_container< L, utf8_mode::TRUST, tao::byte_view >( in ) );
                     return;
                  }
                  const auto size = parse_size< L >( in );
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     match_impl_impl( in, consumer, c );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void match_basic_array( Input& in, Consumer& consumer )
               {
                  consumer.begin_array();
                  while( peek_char_safe( in ) != ']' ) {
                     match_impl( in, consumer );
                     consumer.element();
                  }
                  in.bump_in_this_line( 1 );
                  consumer.end_array();
               }

               template< typename Input, typename Consumer >
               static void match_key( Input& in, Consumer& consumer )
               {
                  consumer.key( parse_container< L, V, tao::string_view >( in ) );
               }

               template< typename Input, typename Consumer >
               static void match_object( Input& in, Consumer& consumer )
               {
                  switch( peek_char_safe( in ) ) {
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
                  const auto size = parse_size< L >( in );
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     match_key( in, consumer );
                     throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void match_typed_object( Input& in, Consumer& consumer )
               {
                  const auto c = read_char_safe( in );
                  if( read_char_safe( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson object type not followed by size", in );
                  }
                  const auto size = parse_size< L >( in );
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     match_key( in, consumer );
                     match_impl_impl( in, consumer, c );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void match_basic_object( Input& in, Consumer& consumer )
               {
                  consumer.begin_object();
                  while( peek_char_safe( in ) != '}' ) {
                     match_key( in, consumer );
                     throw_on_empty( in );
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

            template< std::size_t L, utf8_mode V >
            struct basic_grammar : json_pegtl::must< nops, data< L, V >, nops, json_pegtl::eof >
            {
            };

            // UBJSON no-ops 'N' are currently only supported as top-level padding around an actual value.

            using grammar = basic_grammar< 1 << 24, utf8_mode::CHECK >;

         }  // namespace internal

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#endif
