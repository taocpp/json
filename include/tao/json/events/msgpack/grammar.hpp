// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_MSGPACK_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_MSGPACK_GRAMMAR_HPP

#include <cstdint>

#include "../../byte.hpp"
#include "../../internal/endian.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace msgpack
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
                  const auto b = in.peek_byte();
                  if( b <= 0x7f ) {
                     consumer.number( std::uint64_t( b ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  else if( b >= 0xe0 ) {
                     consumer.number( std::int64_t( std::int8_t( b ) ) );
                     in.bump_in_this_line();
                     return true;
                  }
                  else if( ( 0x80 <= b ) && ( b <= 0x8f ) ) {
                     in.bump_in_this_line();
                     return match_object( in, consumer, b - 0x80 );
                  }
                  else if( ( 0x90 <= b ) && ( b <= 0x9f ) ) {
                     in.bump_in_this_line();
                     return match_array( in, consumer, b - 0x90 );
                  }
                  else if( ( 0xa0 <= b ) && ( b <= 0xbf ) ) {
                     in.bump_in_this_line();
                     consumer.string( read_container< std::string >( in, b - 0xa0 ) );
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
                        consumer.binary( read_container< std::vector< byte > >( in, read_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case 0xc5:
                        consumer.binary( read_container< std::vector< byte > >( in, read_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case 0xc6:
                        consumer.binary( read_container< std::vector< byte > >( in, read_number< std::size_t, std::uint32_t >( in ) ) );
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
                        consumer.string( read_container< std::string >( in, read_number< std::size_t, std::uint8_t >( in ) ) );
                        return true;
                     case 0xda:
                        consumer.string( read_container< std::string >( in, read_number< std::size_t, std::uint16_t >( in ) ) );
                        return true;
                     case 0xdb:
                        consumer.string( read_container< std::string >( in, read_number< std::size_t, std::uint32_t >( in ) ) );
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
                  if( in.size( count ) < count ) {
                     throw json_pegtl::parse_error( "unexpected end of input", in );
                  }
                  in.bump_in_this_line( count );
               }

               template< typename Result, typename Number, typename Input >
               static std::uint64_t read_number( Input& in )
               {
                  if( in.size( sizeof( Number ) ) > sizeof( Number ) ) {
                     const Result result = json::internal::be_to_h< Number >( in.current() + 1 );
                     in.bump_in_this_line( 1 + sizeof( Number ) );
                     return result;
                  }
                  throw json_pegtl::parse_error( "unexpected end of msgpack input", in );
               }

               template< typename Result, typename Input >
               static Result read_container( Input& in, const std::size_t size )
               {
                  using value_t = typename Result::value_type;
                  if( in.size( size ) < size ) {
                     throw json_pegtl::parse_error( "unexpected end of input", in );
                  }
                  const value_t* pointer = reinterpret_cast< const value_t* >( in.current() );
                  Result result( pointer, pointer + size );
                  in.bump_in_this_line( size );
                  return result;
               }

               template< typename Input >
               static std::string read_key( Input& in )
               {
                  if( in.empty() ) {
                     throw json_pegtl::parse_error( "unexpected end of input", in );
                  }
                  switch( in.peek_byte() ) {
                     case 0xd9:
                        return read_container< std::string >( in, read_number< std::size_t, std::uint8_t >( in ) );
                     case 0xda:
                        return read_container< std::string >( in, read_number< std::size_t, std::uint16_t >( in ) );
                     case 0xdb:
                        return read_container< std::string >( in, read_number< std::size_t, std::uint32_t >( in ) );
                  }
                  throw json_pegtl::parse_error( "unexpected key type", in );
               }

               template< typename Input, typename Consumer >
               static bool match_array( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     if( in.empty() ) {
                        throw json_pegtl::parse_error( "unexpected end of input", in );
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
                        throw json_pegtl::parse_error( "unexpected end of input", in );
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

         }  // namespace msgpack

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
