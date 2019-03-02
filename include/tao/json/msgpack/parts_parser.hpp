// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_PARTS_PARSER_HPP
#define TAO_JSON_MSGPACK_PARTS_PARSER_HPP

#include <cstdint>
#include <optional>

#include "../events/discard.hpp"
#include "../external/pegtl/string_input.hpp"
#include "../utf8.hpp"

#include "internal/format.hpp"
#include "internal/grammar.hpp"

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
               return format( json::internal::peek_uint8( in ) );
            }

            template< typename Input >
            bool read_boolean( Input& in )
            {
               const auto b = internal::peek_format( in );
               switch( b ) {
                  case internal::format::BOOL_TRUE:
                  case internal::format::BOOL_FALSE:
                     in.bump_in_this_line( 1 );
                     return bool( b & 1 );
                  default:
                     throw json_pegtl::parse_error( "expected boolean", in );  // NOLINT
               }
               std::abort();
            }

            template< typename Input >
            tao::binary_view read_binary( Input& in )
            {
               switch( peek_format( in ) ) {
                  case format::BIN8:
                     return json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint8_t >( in, 1 ) );
                  case format::BIN16:
                     return json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) );
                  case format::BIN32:
                     return json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) );
                  default:
                     throw json_pegtl::parse_error( "expected binary data", in );  // NOLINT
               }
            }

            inline std::int64_t test_signed( const std::uint64_t i )
            {
               if( ( i & ( std::uint64_t( 1 ) << 63 ) ) != 0 ) {
                  throw std::runtime_error( "integer overflow for signed" );  // NOLINT
               }
               return std::int64_t( i );
            }

            template< typename Input >
            std::int64_t read_signed( Input& in )
            {
               const auto b = json::internal::peek_uint8( in );
               if( b <= std::uint8_t( format::POSITIVE_MAX ) ) {
                  in.bump_in_this_line();
                  return std::int64_t( b );
               }
               if( b >= std::uint8_t( format::NEGATIVE_MIN ) ) {
                  in.bump_in_this_line();
                  return std::int64_t( std::int8_t( b ) );
               }
               switch( format( b ) ) {
                  case format::UINT8:
                     return json::internal::read_big_endian_number< std::int64_t, std::uint8_t >( in, 1 );
                  case format::UINT16:
                     return json::internal::read_big_endian_number< std::int64_t, std::uint16_t >( in, 1 );
                  case format::UINT32:
                     return json::internal::read_big_endian_number< std::int64_t, std::uint32_t >( in, 1 );
                  case format::UINT64:
                     return test_signed( json::internal::read_big_endian_number< std::uint64_t >( in, 1 ) );
                  case format::INT8:
                     return json::internal::read_big_endian_number< std::int64_t, std::int8_t >( in, 1 );
                  case format::INT16:
                     return json::internal::read_big_endian_number< std::int64_t, std::int16_t >( in, 1 );
                  case format::INT32:
                     return json::internal::read_big_endian_number< std::int64_t, std::int32_t >( in, 1 );
                  case format::INT64:
                     return json::internal::read_big_endian_number< std::int64_t >( in, 1 );
                  default:
                     throw json_pegtl::parse_error( "expected signed number", in );  // NOLINT
               }
            }

            inline std::uint64_t test_unsigned( const std::int64_t i )
            {
               if( i < 0 ) {
                  throw std::runtime_error( "negative number for unsigned" );  // NOLINT
               }
               return std::uint64_t( i );
            }

            template< typename Input >
            std::uint64_t read_unsigned( Input& in )
            {
               const auto b = json::internal::peek_uint8( in );
               if( b <= std::uint8_t( format::POSITIVE_MAX ) ) {
                  in.bump_in_this_line();
                  return std::uint64_t( b );
               }
               switch( format( b ) ) {
                  case format::UINT8:
                     return json::internal::read_big_endian_number< std::uint64_t, std::uint8_t >( in, 1 );
                  case format::UINT16:
                     return json::internal::read_big_endian_number< std::uint64_t, std::uint16_t >( in, 1 );
                  case format::UINT32:
                     return json::internal::read_big_endian_number< std::uint64_t, std::uint32_t >( in, 1 );
                  case format::UINT64:
                     return json::internal::read_big_endian_number< std::uint64_t >( in, 1 );
                  case format::INT8:
                     return test_unsigned( json::internal::read_big_endian_number< std::int64_t, std::int8_t >( in, 1 ) );
                  case format::INT16:
                     return test_unsigned( json::internal::read_big_endian_number< std::int64_t, std::int16_t >( in, 1 ) );
                  case format::INT32:
                     return test_unsigned( json::internal::read_big_endian_number< std::int64_t, std::int32_t >( in, 1 ) );
                  case format::INT64:
                     return test_unsigned( json::internal::read_big_endian_number< std::int64_t >( in, 1 ) );
                  default:
                     throw json_pegtl::parse_error( "expected positive number", in );  // NOLINT
               }
            }

            template< typename Input >
            double read_double( Input& in )
            {
               switch( peek_format( in ) ) {
                  case format::FLOAT32:
                     return json::internal::read_big_endian_number< double, float >( in, 1 );
                  case format::FLOAT64:
                     return json::internal::read_big_endian_number< double >( in, 1 );
                  default:
                     throw json_pegtl::parse_error( "expected floating point number", in );  // NOLINT
               }
            }

            template< format Min, format Max, format S16, format S32, typename Input >
            std::size_t read_container_size( Input& in )
            {
               const auto b = json::internal::peek_uint8( in );
               if( ( std::uint8_t( Min ) <= b ) && ( b <= std::uint8_t( Max ) ) ) {
                  in.bump_in_this_line();
                  return state_t( b - std::uint8_t( Min ) );
               }
               switch( format( b ) ) {
                  case S16:
                     return state_t( json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) );
                  case S32:
                     return state_t( json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) );
                  default:
                     throw json_pegtl::parse_error( "expected container", in );  // NOLINT
               }
            }

         }  // namespace internal

         template< utf8_mode V = utf8_mode::check, typename Input = json_pegtl::string_input< json_pegtl::tracking_mode::lazy > >
         class basic_parts_parser
         {
         public:
            template< typename... Ts >
            explicit basic_parts_parser( Ts&&... ts )
               : m_input( std::forward< Ts >( ts )... )
            {
            }

            bool empty()
            {
               return m_input.empty();
            }

            bool null()
            {
               if( internal::peek_format( m_input ) == internal::format::NIL ) {
                  m_input.bump_in_this_line( 1 );
                  return true;
               }
               return false;
            }

            bool boolean()
            {
               return internal::read_boolean( m_input );
            }

            std::string_view string()
            {
               return internal::read_string< V >( m_input );
            }

            tao::binary_view binary()
            {
               return internal::read_binary( m_input );
            }

            std::string_view key()
            {
               return internal::read_string< V >( m_input );
            }

            std::int64_t number_signed()
            {
               return internal::read_signed( m_input );
            }

            std::uint64_t number_unsigned()
            {
               return internal::read_unsigned( m_input );
            }

            double number_double()
            {
               return internal::read_double( m_input );
            }

            struct state_t
            {
               explicit state_t( const std::size_t in_size )
                  : size( in_size )
               {
               }

               std::size_t i = 0;
               std::size_t size;
            };

            state_t begin_array()
            {
               return state_t( internal::read_container_size< internal::format::FIXARRAY_MIN, internal::format::FIXARRAY_MAX, internal::format::ARRAY16, internal::format::ARRAY32 >( m_input ) );
            }

            state_t begin_object()
            {
               return state_t( internal::read_container_size< internal::format::FIXMAP_MIN, internal::format::FIXMAP_MAX, internal::format::MAP16, internal::format::MAP32 >( m_input ) );
            }

            void end_array( state_t& p )
            {
               if( p.size != p.i ) {
                  throw json_pegtl::parse_error( "array size mismatch", m_input );
               }
            }

            void end_object( state_t& p )
            {
               if( p.size != p.i ) {
                  throw json_pegtl::parse_error( "object size mismatch", m_input );
               }
            }

            void element( state_t& p )
            {
               if( p.i++ >= p.size ) {
                  throw json_pegtl::parse_error( "unexpected array end", m_input );
               }
            }

            void member( state_t& p )
            {
               if( p.i++ >= p.size ) {
                  throw json_pegtl::parse_error( "unexpected object end", m_input );
               }
            }

            bool element_or_end_array( state_t& p )
            {
               return p.i++ < p.size;
            }

            bool member_or_end_object( state_t& p )
            {
               return p.i++ < p.size;
            }

            void skip_value()
            {
               json::events::discard consumer;  // TODO: Optimise to not generate events (which requires preparing their - discarded - arguments)?
               json_pegtl::parse< json_pegtl::must< internal::data< V > > >( m_input, consumer );
            }

            auto mark()
            {
               return m_input.template mark< json_pegtl::rewind_mode::required >();
            }

            template< typename T >
            void throw_parse_error( T&& t ) const
            {
               throw json_pegtl::parse_error( std::forward< T >( t ), m_input );
            }

         protected:
            Input m_input;
         };

         using parts_parser = basic_parts_parser<>;

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
