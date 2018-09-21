// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UBJSON_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_UBJSON_INTERNAL_GRAMMAR_HPP

#include <cstdint>

#include "../../binary_view.hpp"
#include "../../external/string_view.hpp"
#include "../../internal/action.hpp"
#include "../../internal/control.hpp"
#include "../../internal/endian.hpp"
#include "../../internal/grammar.hpp"
#include "../../internal/parse_util.hpp"
#include "../../utf8.hpp"

#if defined( _MSC_VER )
#define TAO_JSON_WEAK_PREFIX __declspec( selectany )
#define TAO_JSON_WEAK_SUFFIX
#else
#define TAO_JSON_WEAK_PREFIX
// clang-format off
#define TAO_JSON_WEAK_SUFFIX __attribute__(( weak ))
// clang-format on
#endif

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
               static bool match( Input& in, Consumer& consumer )
               {
                  if( in.size( 2 ) && match_unsafe< A, M, Action, Control >( in, consumer ) ) {
                     in.discard();
                     return true;
                  }
                  return false;
               }

            private:
               template< json_pegtl::apply_mode A,
                         json_pegtl::rewind_mode M,
                         template< typename... > class Action,
                         template< typename... > class Control,
                         typename Input,
                         typename Consumer >
               static bool match_unsafe( Input& in, Consumer& consumer )
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
            };

         }  // namespace internal

      }  // namespace ubjson

      namespace internal
      {
         // clang-format off
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< ubjson::internal::number >::error_message TAO_JSON_WEAK_SUFFIX = "invalid ubjson high-precision number";  // NOLINT
         // clang-format on

      }  // namespace internal

      namespace ubjson
      {
         namespace internal
         {
            template< typename Input >
            std::int64_t read_size_read( Input& in )
            {
               switch( const auto c = json::internal::read_char( in ) ) {
                  case 'i':
                     return json::internal::read_big_endian_number< std::int64_t, std::int8_t >( in );
                  case 'U':
                     return json::internal::read_big_endian_number< std::int64_t, std::uint8_t >( in );
                  case 'I':
                     return json::internal::read_big_endian_number< std::int64_t, std::int16_t >( in );
                  case 'l':
                     return json::internal::read_big_endian_number< std::int64_t, std::int32_t >( in );
                  case 'L':
                     return json::internal::read_big_endian_number< std::int64_t >( in );
                  default:
                     throw json_pegtl::parse_error( "unknown ubjson size type " + std::to_string( unsigned( c ) ), in );
               }
            }

            template< std::size_t L, typename Input >
            std::size_t read_size( Input& in )
            {
               const auto s = read_size_read( in );
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
            Result read_string( Input& in )
            {
               const auto size = read_size< L >( in );
               return json::internal::read_string< U, Result >( in, size );
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
                  if( !in.empty() ) {
                     parse_unsafe( in, consumer );
                     return true;
                  }
                  return false;
               }

            private:
               template< typename Input, typename Consumer >
               static void parse( Input& in, Consumer& consumer )
               {
                  parse( in, consumer, json::internal::read_char( in ) );
               }

               template< typename Input, typename Consumer >
               static void parse_unsafe( Input& in, Consumer& consumer )
               {
                  parse( in, consumer, json::internal::read_char_unsafe( in ) );
               }

               template< typename Input, typename Consumer >
               static void parse( Input& in, Consumer& consumer, const char c )
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
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int8_t >( in ) );
                        break;
                     case 'U':
                        consumer.number( json::internal::read_big_endian_number< std::uint64_t, std::uint8_t >( in ) );
                        break;
                     case 'I':
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int16_t >( in ) );
                        break;
                     case 'l':
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int32_t >( in ) );
                        break;
                     case 'L':
                        consumer.number( json::internal::read_big_endian_number< std::int64_t >( in ) );
                        break;
                     case 'd':
                        consumer.number( json::internal::read_big_endian_number< double, float >( in ) );
                        break;
                     case 'D':
                        consumer.number( json::internal::read_big_endian_number< double >( in ) );
                        break;
                     case 'H':
                        parse_high_precision( in, consumer );
                        break;
                     case 'C':
                        parse_char( in, consumer );
                        break;
                     case 'S':
                        consumer.string( read_string< L, V, tao::string_view >( in ) );
                        break;
                     case '[':
                        parse_array( in, consumer );
                        break;
                     case '{':
                        parse_object( in, consumer );
                        break;
                     case 'N':
                        throw json_pegtl::parse_error( "unsupported ubjson no-op", in );
                        break;
                     default:
                        throw json_pegtl::parse_error( "unknown ubjson type " + std::to_string( unsigned( c ) ), in );
                  }
               }

               template< typename Input, typename Consumer >
               static void parse_char( Input& in, Consumer& consumer )
               {
                  if( in.empty() || ( in.peek_byte( 0 ) > 127 ) ) {
                     throw json_pegtl::parse_error( "missing or invalid ubjson char", in );
                  }
                  consumer.string( std::string( 1, in.peek_char( 0 ) ) );
                  in.bump_in_this_line( 1 );
               }

               template< typename Input, typename Consumer >
               static void parse_high_precision( Input& in, Consumer& consumer )
               {
                  const auto size = read_size< L >( in );
                  json::internal::throw_on_empty( in, size );
                  json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > i2( in.current(), in.current() + size, "UBJSON" );
                  json_pegtl::parse_nested< json_pegtl::must< number, json_pegtl::eof >, json::internal::action, json::internal::control >( in, i2, consumer );
                  in.bump_in_this_line( size );
               }

               template< typename Input, typename Consumer >
               static void parse_array( Input& in, Consumer& consumer )
               {
                  switch( json::internal::peek_char( in ) ) {
                     case '#':
                        in.bump_in_this_line( 1 );
                        parse_sized_array( in, consumer );
                        break;
                     case '$':
                        in.bump_in_this_line( 1 );
                        parse_typed_array( in, consumer );
                        break;
                     default:
                        parse_basic_array( in, consumer );
                        break;
                  }
               }

               template< typename Input, typename Consumer >
               static void parse_sized_array( Input& in, Consumer& consumer )
               {
                  const auto size = read_size< L >( in );
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     parse( in, consumer );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void parse_typed_array( Input& in, Consumer& consumer )
               {
                  const auto c = json::internal::read_char( in );
                  if( json::internal::read_char( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson array type not followed by size", in );
                  }
                  if( c == 'U' ) {
                     // NOTE: UBJSON encodes binary data as 'strongly typed array of uint8 values'.
                     consumer.binary( read_string< L, utf8_mode::TRUST, tao::binary_view >( in ) );
                     return;
                  }
                  const auto size = read_size< L >( in );
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     parse( in, consumer, c );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void parse_basic_array( Input& in, Consumer& consumer )
               {
                  consumer.begin_array();
                  while( json::internal::peek_char( in ) != ']' ) {
                     parse_unsafe( in, consumer );
                     consumer.element();
                  }
                  in.bump_in_this_line( 1 );
                  consumer.end_array();
               }

               template< typename Input, typename Consumer >
               static void parse_key( Input& in, Consumer& consumer )
               {
                  consumer.key( read_string< L, V, tao::string_view >( in ) );
               }

               template< typename Input, typename Consumer >
               static void parse_object( Input& in, Consumer& consumer )
               {
                  switch( json::internal::peek_char( in ) ) {
                     case '#':
                        in.bump_in_this_line( 1 );
                        parse_sized_object( in, consumer );
                        break;
                     case '$':
                        in.bump_in_this_line( 1 );
                        parse_typed_object( in, consumer );
                        break;
                     default:
                        parse_basic_object( in, consumer );
                        break;
                  }
               }

               template< typename Input, typename Consumer >
               static void parse_sized_object( Input& in, Consumer& consumer )
               {
                  const auto size = read_size< L >( in );
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     parse_key( in, consumer );
                     parse( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void parse_typed_object( Input& in, Consumer& consumer )
               {
                  const auto c = json::internal::read_char( in );
                  if( json::internal::read_char( in ) != '#' ) {
                     throw json_pegtl::parse_error( "ubjson object type not followed by size", in );
                  }
                  const auto size = read_size< L >( in );
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     parse_key( in, consumer );
                     parse( in, consumer, c );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }

               template< typename Input, typename Consumer >
               static void parse_basic_object( Input& in, Consumer& consumer )
               {
                  consumer.begin_object();
                  while( json::internal::peek_char( in ) != '}' ) {
                     parse_key( in, consumer );
                     parse( in, consumer );
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

            using grammar = basic_grammar< 1 << 24, utf8_mode::CHECK >;

         }  // namespace internal

      }  // namespace ubjson

   }  // namespace json

}  // namespace tao

#undef TAO_JSON_WEAK_PREFIX
#undef TAO_JSON_WEAK_SUFFIX

#endif
