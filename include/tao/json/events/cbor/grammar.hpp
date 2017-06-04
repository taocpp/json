// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_CBOR_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_CBOR_GRAMMAR_HPP

#include <cstdint>
#include <utility>

#include "major.hpp"

#include "../../byte.hpp"
#include "../../internal/temporary.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         namespace cbor
         {
            namespace internal
            {
               template< typename Input >
               void throw_on_empty( Input& in )
               {
                  if( in.empty() ) {
                     throw json_pegtl::parse_error( "unexpected end of input", in );
                  }
               }

               template< typename Input >
               major peek_major( Input& in )
               {
                  return static_cast< major >( in.peek_byte() & major_mask );
               }

               template< typename Input >
               std::uint8_t peek_minor( Input& in )
               {
                  return in.peek_byte() & minor_mask;
               }

               template< typename Input >
               std::uint8_t peek_byte_safe( Input& in )
               {
                  throw_on_empty( in );
                  return in.peek_byte();
               }

               template< typename Input >
               major peek_major_safe( Input& in )
               {
                  return static_cast< major >( peek_byte_safe( in ) & major_mask );
               }

            }  // namespace internal

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
                  switch( internal::peek_major( in ) ) {
                     case major::UNSIGNED:
                        return match_unsigned( in, consumer );
                     case major::NEGATIVE:
                        return match_negative( in, consumer );
                     case major::BINARY:
                        return match_binary( in, consumer );
                     case major::STRING:
                        return match_string( in, consumer );
                     case major::ARRAY:
                        return match_array( in, consumer );
                     case major::OBJECT:
                        return match_object( in, consumer );
                     case major::TAG:
                        return match_tag( in, consumer );
                     case major::OTHER:
                        return match_other( in, consumer );
                  }
                  assert( false );
               }

               template< typename Input >
               static void skip_unsigned( Input& in )
               {
                  // Assumes in.size( 1 ) >= 1 and in.peek_byte() is the byte with major/minor.

                  switch( internal::peek_minor( in ) ) {
                     default:
                        in.bump_in_this_line();
                        return;
                     case 24:
                        in.bump_in_this_line( 2 );
                        return;
                     case 25:
                        in.bump_in_this_line( 3 );
                        return;
                     case 26:
                        in.bump_in_this_line( 5 );
                        return;
                     case 27:
                        in.bump_in_this_line( 9 );
                        return;
                     case 28:
                     case 29:
                     case 30:
                     case 31:
                        throw json_pegtl::parse_error( "unexpected minor for number or length", in );
                  }
               }

               template< typename Input >
               static std::uint64_t read_embedded_impl( Input& in )
               {
                  const auto result = internal::peek_minor( in ) & minor_mask;
                  in.bump_in_this_line();
                  return result;
               }

               template< typename Unsigned, typename Input >
               static std::uint64_t read_unsigned_impl( Input& in )
               {
                  if( in.size( sizeof( Unsigned ) ) > sizeof( Unsigned ) ) {
                     const Unsigned result = json::internal::be_to_h< Unsigned >( in.current() + 1 );
                     in.bump_in_this_line( 1 + sizeof( Unsigned ) );
                     return result;
                  }
                  throw json_pegtl::parse_error( "unexpected end of input", in );
               }

               template< typename Input >
               static std::uint64_t read_unsigned( Input& in )
               {
                  // Assumes in.size( 1 ) >= 1 and in.peek_byte() is the byte with major/minor.

                  switch( internal::peek_minor( in ) ) {
                     default:
                        return read_embedded_impl( in );
                     case 24:
                        return read_unsigned_impl< std::uint8_t >( in );
                     case 25:
                        return read_unsigned_impl< std::uint16_t >( in );
                     case 26:
                        return read_unsigned_impl< std::uint32_t >( in );
                     case 27:
                        return read_unsigned_impl< std::uint64_t >( in );
                     case 28:
                     case 29:
                     case 30:
                     case 31:
                        throw json_pegtl::parse_error( "unexpected minor for number or length", in );
                  }
               }

               template< typename Input, typename Consumer >
               static bool match_unsigned( Input& in, Consumer& consumer )
               {
                  consumer.number( read_unsigned( in ) );
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_negative( Input& in, Consumer& consumer )
               {
                  const auto u = read_unsigned( in );
                  if( u > 9223372036854775808ull ) {
                     throw json_pegtl::parse_error( "negative integer overflow", in );
                  }
                  consumer.number( std::int64_t( ~u ) );
                  return true;
               }

               template< typename Result, typename Input >
               static Result read_string( Input& in, const major m )
               {
                  using value_t = typename Result::value_type;

                  // Assumes in.size( 1 ) >= 1 and in.peek_byte() is the byte with major/minor.

                  // TODO: Check text strings and text string chunks for valid UTF-8 as per RFC 7049?

                  if( internal::peek_minor( in ) != minor_mask ) {
                     const auto size = read_unsigned( in );
                     if( in.size( size ) < size ) {
                        throw json_pegtl::parse_error( "unexpected end of input", in );
                     }
                     const value_t* pointer = reinterpret_cast< const value_t* >( in.current() );
                     Result result( pointer, pointer + size );
                     in.bump_in_this_line( size );
                     return result;
                  }
                  Result result;
                  in.bump_in_this_line();
                  while( internal::peek_byte_safe( in ) != 0xff ) {
                     if( internal::peek_major( in ) != m ) {
                        throw json_pegtl::parse_error( "non-matching fragment in indefinite length string", in );  // string = text or byte string in RFC 7049 terminology
                     }
                     const auto size = read_unsigned( in );
                     if( in.size( size ) < size ) {
                        throw json_pegtl::parse_error( "unexpected end of input", in );
                     }
                     const value_t* pointer = reinterpret_cast< const value_t* >( in.current() );
                     result.insert( result.end(), pointer, pointer + size );
                     in.bump_in_this_line( size );
                  }
                  in.bump_in_this_line();
                  return result;
               }

               template< typename Input, typename Consumer >
               static bool match_string( Input& in, Consumer& consumer )
               {
                  consumer.string( read_string< std::string >( in, major::STRING ) );
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_binary( Input& in, Consumer& consumer )
               {
                  consumer.binary( read_string< std::vector< byte > >( in, major::BINARY ) );
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_array( Input& in, Consumer& consumer )
               {
                  if( internal::peek_minor( in ) != minor_mask ) {
                     const auto size = read_unsigned( in );
                     consumer.begin_array( size );
                     for( std::uint64_t i = 0; i < size; ++i ) {
                        internal::throw_on_empty( in );
                        match_impl( in, consumer );
                        consumer.element();
                     }
                     consumer.end_array( size );
                  }
                  else {
                     in.bump_in_this_line();
                     consumer.begin_array();
                     while( internal::peek_byte_safe( in ) != 0xff ) {
                        match_impl( in, consumer );
                        consumer.element();
                     }
                     in.bump_in_this_line();
                     consumer.end_array();
                  }
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_object( Input& in, Consumer& consumer )
               {
                  if( internal::peek_minor( in ) != minor_mask ) {
                     const auto size = read_unsigned( in );
                     consumer.begin_object( size );
                     for( std::uint64_t i = 0; i < size; ++i ) {
                        if( internal::peek_major_safe( in ) != major::STRING ) {
                           throw json_pegtl::parse_error( "non-string object key", in );
                        }
                        internal::throw_on_empty( in );
                        consumer.key( read_string< std::string >( in, major::STRING ) );
                        internal::throw_on_empty( in );
                        match_impl( in, consumer );
                        consumer.member();
                     }
                     consumer.end_object( size );
                  }
                  else {
                     in.bump_in_this_line();
                     consumer.begin_object();
                     while( internal::peek_byte_safe( in ) != 0xff ) {
                        if( internal::peek_major( in ) != major::STRING ) {
                           throw json_pegtl::parse_error( "non-string object key", in );
                        }
                        consumer.key( read_string< std::string >( in, major::STRING ) );
                        internal::throw_on_empty( in );
                        match_impl( in, consumer );
                        consumer.member();
                     }
                     in.bump_in_this_line();
                     consumer.end_object();
                  }
                  return true;
               }

               template< typename Input, typename Consumer >
               static bool match_tag( Input& in, Consumer& )
               {
                  skip_unsigned( in );
                  return true;
               }

               template< typename Floating, typename Input >
               static double read_floating_impl( Input& in )
               {
                  // TODO: Map NaN/Inf to JSON null.
                  if( in.size( sizeof( Floating ) ) > sizeof( Floating ) ) {
                     const Floating result = json::internal::be_to_h< Floating >( in.current() + 1 );
                     in.bump_in_this_line( 1 + sizeof( Floating ) );
                     return result;
                  }
                  throw json_pegtl::parse_error( "unexpected end of input", in );
               }

               template< typename Input, typename Consumer >
               static bool match_other( Input& in, Consumer& consumer )
               {
                  switch( internal::peek_minor( in ) ) {
                     case 20:
                        consumer.boolean( false );
                        in.bump_in_this_line();
                        return true;
                     case 21:
                        consumer.boolean( true );
                        in.bump_in_this_line();
                        return true;
                     case 22:
                        consumer.null();
                        in.bump_in_this_line();
                        return true;
                     case 26:
                        consumer.number( read_floating_impl< float >( in ) );
                        return true;
                     case 27:
                        consumer.number( read_floating_impl< double >( in ) );
                        return true;
                     case 24:
                     case 25:  // TODO: 16bit float?
                     default:
                        throw json_pegtl::parse_error( "unsupported minor for major 7", in );
                  }
               }
            };

            struct grammar : json_pegtl::must< data, json_pegtl::eof >
            {
            };

         }  // namespace cbor

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
