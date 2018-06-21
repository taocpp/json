// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP

#include <cstdint>

#include "../../binary_view.hpp"
#include "../../external/byte.hpp"
#include "../../external/pegtl.hpp"
#include "../../external/string_view.hpp"
#include "../../internal/endian.hpp"
#include "../../internal/throw_parse_error.hpp"
#include "../../utf8.hpp"

#include "format.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         namespace internal
         {
            // clang-format off
            template< typename Input, typename... Ts >
            [[noreturn]] void throw_parse_error( Input& in, const Ts&... ts )
            {
               json::internal::throw_parse_error( in, "msgpack:", ts... );
            }
            // clang-format on

            template< typename Input >
            void throw_on_empty( Input& in, const std::size_t required = 1 )
            {
               if( in.size( required ) < required ) {
                  throw_parse_error( in, "unexpected end of input" );
               }
            }

            template< typename Input >
            format peek_format( Input& in )
            {
               return static_cast< format >( in.peek_byte() );
            }

            template< typename Input >
            std::uint8_t peek_byte_safe( Input& in )
            {
               throw_on_empty( in );
               return in.peek_byte();
            }

            template< typename Input >
            format peek_format_safe( Input& in )
            {
               return static_cast< format >( peek_byte_safe( in ) );
            }

            template< typename Result, typename Number, typename Input >
            Result parse_number( Input& in )
            {
               throw_on_empty( in, 1 + sizeof( Number ) );
               const Result result( json::internal::be_to_h< Number >( in.current() + 1 ) );
               in.bump_in_this_line( 1 + sizeof( Number ) );
               return result;
            }

            template< utf8_mode U, typename Result, typename Input >
            Result parse_container( Input& in, const std::size_t size )
            {
               using value_t = typename Result::value_type;
               throw_on_empty( in, size );
               const auto* pointer = static_cast< const value_t* >( static_cast< const void* >( in.current() ) );
               Result result( pointer, size );
               json::internal::consume_utf8< U >( in, size );
               return result;
            }

            template< utf8_mode U, typename Input >
            tao::string_view parse_key( Input& in )
            {
               const auto b = peek_byte_safe( in );
               if( ( std::uint8_t( format::FIXSTR_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXSTR_MAX ) ) ) {
                  in.bump_in_this_line();
                  return parse_container< U, tao::string_view >( in, b - std::uint8_t( format::FIXSTR_MIN ) );
               }
               switch( format( b ) ) {
                  case format::STR8:
                     return parse_container< U, tao::string_view >( in, parse_number< std::size_t, std::uint8_t >( in ) );
                  case format::STR16:
                     return parse_container< U, tao::string_view >( in, parse_number< std::size_t, std::uint16_t >( in ) );
                  case format::STR32:
                     return parse_container< U, tao::string_view >( in, parse_number< std::size_t, std::uint32_t >( in ) );
                  default:
                     throw_parse_error( in, "unexpected key type" );
               }
            }

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

               template< typename Input, typename Consumer >
               static bool match_impl( Input& in, Consumer& consumer )
               {
                  const auto b = in.peek_byte();
                  if( b <= std::uint8_t( format::POSITIVE_MAX )) {
                     consumer.number( std::uint64_t( b ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  if( b >= std::uint8_t( format::NEGATIVE_MIN ) ) {
                     consumer.number( std::int64_t( std::int8_t( b ) ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  if( ( std::uint8_t( format::FIXMAP_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXMAP_MAX ) ) ) {
                     in.bump_in_this_line();
                     return match_object( in, consumer, b - std::uint8_t( format::FIXMAP_MIN ) );
                  }
                  if( ( std::uint8_t( format::FIXARRAY_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXARRAY_MAX ) ) ) {
                     in.bump_in_this_line();
                     return match_array( in, consumer, b - std::uint8_t( format::FIXARRAY_MIN ) );
                  }
                  if( ( std::uint8_t( format::FIXSTR_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXSTR_MAX ) ) ) {
                     in.bump_in_this_line();
                     consumer.string( parse_container< V, tao::string_view >( in, b - std::uint8_t( format::FIXSTR_MIN ) ) );
                     return true;
                  }
                  switch( format( b ) ) {
                     case format::NIL:
                        consumer.null();
                        in.bump_in_this_line();
                        return true;
                     case format::UNUSED:
                        throw_parse_error( in, "unused first byte 0xc1" );
                     case format::TRUE:
                     case format::FALSE:
                        consumer.boolean( b & 1 );
                        in.bump_in_this_line();
                        return true;
                     case format::BIN8:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, parse_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case format::BIN16:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, parse_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case format::BIN32:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, parse_number< std::size_t, std::uint32_t >( in ) ) );
                        return true;
                     case format::EXT8:
                        discard( in, parse_number< std::size_t, std::uint8_t >( in ) + 1 );
                        return true;
                     case format::EXT16:
                        discard( in, parse_number< std::size_t, std::uint16_t >( in ) + 1 );
                        return true;
                     case format::EXT32:
                        discard( in, parse_number< std::size_t, std::uint32_t >( in ) + 1 );
                        return true;
                     case format::FLOAT32:
                        consumer.number( parse_number< double, float >( in ) );
                        return true;
                     case format::FLOAT64:
                        consumer.number( parse_number< double, double >( in ) );
                        return true;
                     case format::UINT8:
                        consumer.number( parse_number< std::uint64_t, std::uint8_t >( in ) );
                        return true;
                     case format::UINT16:
                        consumer.number( parse_number< std::uint64_t, std::uint16_t >( in ) );
                        return true;
                     case format::UINT32:
                        consumer.number( parse_number< std::uint64_t, std::uint32_t >( in ) );
                        return true;
                     case format::UINT64:
                        consumer.number( parse_number< std::uint64_t, std::uint64_t >( in ) );
                        return true;
                     case format::INT8:
                        consumer.number( parse_number< std::int64_t, std::int8_t >( in ) );
                        return true;
                     case format::INT16:
                        consumer.number( parse_number< std::int64_t, std::int16_t >( in ) );
                        return true;
                     case format::INT32:
                        consumer.number( parse_number< std::int64_t, std::int32_t >( in ) );
                        return true;
                     case format::INT64:
                        consumer.number( parse_number< std::int64_t, std::int64_t >( in ) );
                        return true;
                     case format::FIXEXT1:
                        discard( in, 3 );
                        return true;
                     case format::FIXEXT2:
                        discard( in, 4 );
                        return true;
                     case format::FIXEXT4:
                        discard( in, 6 );
                        return true;
                     case format::FIXEXT8:
                        discard( in, 10 );
                        return true;
                     case format::FIXEXT16:
                        discard( in, 18 );
                        return true;
                     case format::STR8:
                        consumer.string( parse_container< V, tao::string_view >( in, parse_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case format::STR16:
                        consumer.string( parse_container< V, tao::string_view >( in, parse_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case format::STR32:
                        consumer.string( parse_container< V, tao::string_view >( in, parse_number< std::size_t, std::uint32_t >( in ) ) );
                        return true;
                     case format::ARRAY16:
                        return match_array( in, consumer, parse_number< std::size_t, std::uint16_t >( in ) );
                     case format::ARRAY32:
                        return match_array( in, consumer, parse_number< std::size_t, std::uint32_t >( in ) );
                     case format::MAP16:
                        return match_object( in, consumer, parse_number< std::size_t, std::uint16_t >( in ) );
                     case format::MAP32:
                        return match_object( in, consumer, parse_number< std::size_t, std::uint32_t >( in ) );
                     default:
                        // LCOV_EXCL_START
                        assert( false );
                        return false;
                        // LCOV_EXCL_STOP
                  }
               }

               template< typename Input >
               static void discard( Input& in, const std::size_t count )
               {
                  throw_on_empty( in, count );
                  in.bump_in_this_line( count );
               }

               template< typename Input, typename Consumer >
               static bool match_array( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     throw_on_empty( in );
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
                     consumer.key( parse_key< V >( in ) );
                     throw_on_empty( in );
                     match_impl( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
                  return true;
               }
            };

            template< utf8_mode V >
            struct basic_grammar : json_pegtl::must< data< V >, json_pegtl::eof >
            {
            };

            using grammar = basic_grammar< utf8_mode::CHECK >;

         }  // namespace internal

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
