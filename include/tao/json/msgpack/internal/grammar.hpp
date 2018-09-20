// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP

#include <cstdint>

#include "../../binary_view.hpp"
#include "../../external/byte.hpp"
#include "../../external/pegtl.hpp"
#include "../../external/string_view.hpp"
#include "../../internal/format.hpp"
#include "../../internal/parse_util.hpp"
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
            template< typename Input >
            format peek_format( Input& in )
            {
               return static_cast< format >( in.peek_byte() );
            }

            template< typename Input >
            format peek_format_safe( Input& in )
            {
               return static_cast< format >( json::internal::peek_byte_safe( in ) );
            }

            template< utf8_mode U, typename Result, typename Input >
            Result parse_container( Input& in, const std::size_t size )
            {
               using value_t = typename Result::value_type;
               json::internal::throw_on_empty( in, size );
               const auto* pointer = static_cast< const value_t* >( static_cast< const void* >( in.current() ) );
               Result result( pointer, size );
               json::internal::consume_utf8< U >( in, size );
               return result;
            }

            template< utf8_mode U, typename Input >
            tao::string_view parse_key( Input& in )
            {
               const auto b = json::internal::peek_byte_safe( in );
               if( ( std::uint8_t( format::FIXSTR_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXSTR_MAX ) ) ) {
                  in.bump_in_this_line();
                  return parse_container< U, tao::string_view >( in, b - std::uint8_t( format::FIXSTR_MIN ) );
               }
               switch( format( b ) ) {
                  case format::STR8:
                     return parse_container< U, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint8_t >( in, 1 ) );
                  case format::STR16:
                     return parse_container< U, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1 ) );
                  case format::STR32:
                     return parse_container< U, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) );
                  default:
                     throw json_pegtl::parse_error( "unexpected key type", in );
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
                  if( b <= std::uint8_t( format::POSITIVE_MAX ) ) {
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
                        throw json_pegtl::parse_error( "unused first byte 0xc1", in );
                     case format::BOOL_TRUE:
                        consumer.boolean( true );
                        in.bump_in_this_line();
                        return true;
                     case format::BOOL_FALSE:
                        consumer.boolean( false );
                        in.bump_in_this_line();
                        return true;
                     case format::BIN8:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint8_t >( in, 1 ) ) );
                        return true;
                     case format::BIN16:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1 ) ) );
                        return true;
                     case format::BIN32:
                        consumer.binary( parse_container< utf8_mode::TRUST, tao::binary_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) ) );
                        return true;
                     case format::EXT8:
                        discard( in, json::internal::read_be_number_safe< std::size_t, std::uint8_t >( in, 1 ) + 1 );
                        return true;
                     case format::EXT16:
                        discard( in, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1 ) + 1 );
                        return true;
                     case format::EXT32:
                        discard( in, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) + 1 );
                        return true;
                     case format::FLOAT32:
                        consumer.number( json::internal::read_be_number_safe< double, float >( in, 1 ) );
                        return true;
                     case format::FLOAT64:
                        consumer.number( json::internal::read_be_number_safe< double >( in, 1 ) );
                        return true;
                     case format::UINT8:
                        consumer.number( json::internal::read_be_number_safe< std::uint64_t, std::uint8_t >( in, 1 ) );
                        return true;
                     case format::UINT16:
                        consumer.number( json::internal::read_be_number_safe< std::uint64_t, std::uint16_t >( in, 1 ) );
                        return true;
                     case format::UINT32:
                        consumer.number( json::internal::read_be_number_safe< std::uint64_t, std::uint32_t >( in, 1 ) );
                        return true;
                     case format::UINT64:
                        consumer.number( json::internal::read_be_number_safe< std::uint64_t >( in, 1 ) );
                        return true;
                     case format::INT8:
                        consumer.number( json::internal::read_be_number_safe< std::int64_t, std::int8_t >( in, 1 ) );
                        return true;
                     case format::INT16:
                        consumer.number( json::internal::read_be_number_safe< std::int64_t, std::int16_t >( in, 1 ) );
                        return true;
                     case format::INT32:
                        consumer.number( json::internal::read_be_number_safe< std::int64_t, std::int32_t >( in, 1 ) );
                        return true;
                     case format::INT64:
                        consumer.number( json::internal::read_be_number_safe< std::int64_t >( in, 1 ) );
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
                        consumer.string( parse_container< V, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint8_t >( in, 1 ) ) );
                        return true;
                     case format::STR16:
                        consumer.string( parse_container< V, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1 ) ) );
                        return true;
                     case format::STR32:
                        consumer.string( parse_container< V, tao::string_view >( in, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) ) );
                        return true;
                     case format::ARRAY16:
                        return match_array( in, consumer, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1) );
                     case format::ARRAY32:
                        return match_array( in, consumer, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) );
                     case format::MAP16:
                        return match_object( in, consumer, json::internal::read_be_number_safe< std::size_t, std::uint16_t >( in, 1 ) );
                     case format::MAP32:
                        return match_object( in, consumer, json::internal::read_be_number_safe< std::size_t, std::uint32_t >( in, 1 ) );
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
                  json::internal::throw_on_empty( in, count );
                  in.bump_in_this_line( count );
               }

               template< typename Input, typename Consumer >
               static bool match_array( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     json::internal::throw_on_empty( in );
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
                     json::internal::throw_on_empty( in );
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
