// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP

#include <cstdint>

#include "../../byte_view.hpp"
#include "../../external/byte.hpp"
#include "../../external/pegtl.hpp"
#include "../../external/string_view.hpp"
#include "../../internal/endian.hpp"
#include "../../utf8.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         namespace internal
         {
            template< typename Input >
            void throw_on_empty( Input& in, const std::size_t required = 1 )
            {
               if( in.size( required ) < required ) {
                  throw json_pegtl::parse_error( "unexpected end of msgpack input", in );
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

               template< typename Result, typename Number, typename Input >
               static Result read_number( Input& in )
               {
                  throw_on_empty( in, 1 + sizeof( Number ) );
                  const Result result( json::internal::be_to_h< Number >( in.current() + 1 ) );
                  in.bump_in_this_line( 1 + sizeof( Number ) );
                  return result;
               }

               template< typename Input, typename Consumer >
               static bool match_impl( Input& in, Consumer& consumer )
               {
                  const auto b = in.peek_byte();
                  if( b <= 0x7f ) {
                     consumer.number( std::uint64_t( b ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  if( b >= 0xe0 ) {
                     consumer.number( std::int64_t( std::int8_t( b ) ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  if( ( 0x80 <= b ) && ( b <= 0x8f ) ) {
                     in.bump_in_this_line();
                     return match_object( in, consumer, b - 0x80 );
                  }
                  if( ( 0x90 <= b ) && ( b <= 0x9f ) ) {
                     in.bump_in_this_line();
                     return match_array( in, consumer, b - 0x90 );
                  }
                  if( ( 0xa0 <= b ) && ( b <= 0xbf ) ) {
                     in.bump_in_this_line();
                     consumer.string( read_container< V, tao::string_view >( in, b - 0xa0 ) );
                     return true;
                  }
                  switch( b ) {
                     case 0xc0:
                        consumer.null();
                        in.bump_in_this_line();
                        return true;
                     case 0xc1:
                        throw json_pegtl::parse_error( "unused msgpack first byte 0xc1", in );
                     case 0xc2:
                     case 0xc3:
                        consumer.boolean( b & 1 );
                        in.bump_in_this_line();
                        return true;
                     case 0xc4:
                        consumer.binary( read_container< utf8_mode::TRUST, tao::byte_view >( in, read_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case 0xc5:
                        consumer.binary( read_container< utf8_mode::TRUST, tao::byte_view >( in, read_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case 0xc6:
                        consumer.binary( read_container< utf8_mode::TRUST, tao::byte_view >( in, read_number< std::size_t, std::uint32_t >( in ) ) );
                        return true;
                     case 0xc7:
                        discard( in, read_number< std::size_t, std::uint8_t >( in ) + 1 );
                        return true;
                     case 0xc8:
                        discard( in, read_number< std::size_t, std::uint16_t >( in ) + 1 );
                        return true;
                     case 0xc9:
                        discard( in, read_number< std::size_t, std::uint32_t >( in ) + 1 );
                        return true;
                     case 0xca:
                        consumer.number( read_number< double, float >( in ) );
                        return true;
                     case 0xcb:
                        consumer.number( read_number< double, double >( in ) );
                        return true;
                     case 0xcc:
                        consumer.number( read_number< std::uint64_t, std::uint8_t >( in ) );
                        return true;
                     case 0xcd:
                        consumer.number( read_number< std::uint64_t, std::uint16_t >( in ) );
                        return true;
                     case 0xce:
                        consumer.number( read_number< std::uint64_t, std::uint32_t >( in ) );
                        return true;
                     case 0xcf:
                        consumer.number( read_number< std::uint64_t, std::uint64_t >( in ) );
                        return true;
                     case 0xd0:
                        consumer.number( read_number< std::int64_t, std::int8_t >( in ) );
                        return true;
                     case 0xd1:
                        consumer.number( read_number< std::int64_t, std::int16_t >( in ) );
                        return true;
                     case 0xd2:
                        consumer.number( read_number< std::int64_t, std::int32_t >( in ) );
                        return true;
                     case 0xd3:
                        consumer.number( read_number< std::int64_t, std::int64_t >( in ) );
                        return true;
                     case 0xd4:
                        discard( in, 3 );
                        return true;
                     case 0xd5:
                        discard( in, 4 );
                        return true;
                     case 0xd6:
                        discard( in, 6 );
                        return true;
                     case 0xd7:
                        discard( in, 10 );
                        return true;
                     case 0xd8:
                        discard( in, 18 );
                        return true;
                     case 0xd9:
                        consumer.string( read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case 0xda:
                        consumer.string( read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case 0xdb:
                        consumer.string( read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint32_t >( in ) ) );
                        return true;
                     case 0xdc:
                        return match_array( in, consumer, read_number< std::size_t, std::uint16_t >( in ) );
                     case 0xdd:
                        return match_array( in, consumer, read_number< std::size_t, std::uint32_t >( in ) );
                     case 0xde:
                        return match_object( in, consumer, read_number< std::size_t, std::uint16_t >( in ) );
                     case 0xdf:
                        return match_object( in, consumer, read_number< std::size_t, std::uint32_t >( in ) );
                  }
                  // LCOV_EXCL_START
                  assert( false );
                  return false;
                  // LCOV_EXCL_STOP
               }

               template< typename Input >
               static void discard( Input& in, const std::size_t count )
               {
                  throw_on_empty( in, count );
                  in.bump_in_this_line( count );
               }

               template< utf8_mode U, typename Result, typename Input >
               static Result read_container( Input& in, const std::size_t size )
               {
                  using value_t = typename Result::value_type;
                  throw_on_empty( in, size );
                  const auto* pointer = static_cast< const value_t* >( static_cast< const void* >( in.current() ) );
                  Result result( pointer, size );
                  json::internal::consume_utf8< U >( in, size );
                  return result;
               }

               template< typename Input >
               static tao::string_view read_key( Input& in )
               {
                  throw_on_empty( in, 1 );
                  const auto b = in.peek_byte();
                  if( ( 0xa0 <= b ) && ( b <= 0xbf ) ) {
                     in.bump_in_this_line();
                     return read_container< V, tao::string_view >( in, b - 0xa0 );
                  }
                  switch( b ) {
                     case 0xd9:
                        return read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint8_t >( in ) );
                     case 0xda:
                        return read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint16_t >( in ) );
                     case 0xdb:
                        return read_container< V, tao::string_view >( in, read_number< std::size_t, std::uint32_t >( in ) );
                  }
                  throw json_pegtl::parse_error( "unexpected key type", in );
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
                     consumer.key( read_key( in ) );
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
