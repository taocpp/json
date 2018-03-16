// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CBOR_PARTS_PARSER_HPP
#define TAO_JSON_CBOR_PARTS_PARSER_HPP

#include "../external/pegtl/string_input.hpp"
#include "../utf8.hpp"

#include "internal/grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< utf8_mode V = utf8_mode::CHECK, typename Input = json_pegtl::string_input< json_pegtl::tracking_mode::LAZY > >
         class basic_parts_parser
         {
         public:
            template< typename... Ts >
            explicit basic_parts_parser( Ts&&... ts )
               : m_input( std::forward< Ts >( ts )... )
            {
            }

            bool null()
            {
               if( internal::peek_byte_safe( m_input ) == std::uint8_t( internal::major::OTHER ) + 22 ) {
                  m_input.bump_in_this_line( 1 );
                  return true;
               }
               return false;
            }

            bool boolean()
            {
               const auto b = internal::peek_byte_safe( m_input );
               switch( b ) {
                  case std::uint8_t( internal::major::OTHER ) + 20:
                  case std::uint8_t( internal::major::OTHER ) + 21:
                     m_input.bump_in_this_line( 1 );
                     return bool( b - std::uint8_t( internal::major::OTHER ) - 20 );
                  default:
                     internal::throw_parse_error( m_input, "expected boolean" );
               }
               std::abort();
            }

            void check_major( const internal::major m, const char* e )
            {
               const auto b = internal::peek_major_safe( m_input );
               if( b != m ) {
                  internal::throw_parse_error( m_input, e );
               }
            }

            template< utf8_mode U, typename T >
            T cbor_string( const internal::major m, const char* e )
            {
               check_major( m, e );
               if( internal::peek_minor( m_input ) != internal::minor_mask ) {
                  return internal::parse_string_1< U, T >( m_input );
               }
               return internal::parse_string_n< U, T >( m_input, m );
            }

            std::string string()
            {
               return cbor_string< V, std::string >( internal::major::STRING, "expected string" );
            }

            std::string binary()
            {
               return cbor_string< utf8_mode::TRUST, std::vector< tao::byte > >( internal::major::BINARY, "expected binary" );
            }

            std::string key()
            {
               return string();
            }

            tao::string_view string_view()
            {
               const auto b = internal::peek_byte_safe( m_input );
               if( b != std::uint8_t( internal::major::STRING ) + internal::minor_mask ) {
                  internal::throw_parse_error( m_input, "expected definitive string" );
               }
               return internal::parse_string_1< V, tao::string_view >( m_input );
            }

            tao::binary_view binary_view()
            {
               const auto b = internal::peek_byte_safe( m_input );
               if( b!= std::uint8_t( internal::major::BINARY ) + internal::minor_mask ) {
                  internal::throw_parse_error( m_input, "expected definitive binary" );
               }
               return internal::parse_string_1< utf8_mode::TRUST, tao::binary_view >( m_input );
            }

            std::int64_t number_signed_unsigned()
            {
               const auto u = internal::parse_unsigned( m_input );
               if( u > 9223372036854775807ull ) {
                  internal::throw_parse_error( m_input, "positive integer overflow" );
               }
               return std::int64_t( u );
            }

            std::int64_t number_signed_negative()
            {
               const auto u = internal::parse_unsigned( m_input );
               if( u > 9223372036854775808ull ) {
                  internal::throw_parse_error( m_input, "negative integer overflow" );
               }
               return std::int64_t( ~u );
            }

            std::int64_t number_signed()
            {
               const auto b = internal::peek_major_safe( m_input );
               switch( b ) {
                  case internal::major::UNSIGNED:
                     return number_signed_unsigned();
                  case internal::major::NEGATIVE:
                     return number_signed_negative();
                  default:
                     internal::throw_parse_error( m_input, "expected integer" );
               }
               std::abort();
            }

            std::uint64_t number_unsigned()
            {
               check_major( internal::major::UNSIGNED, "expected unsigned" );
               return internal::parse_unsigned( m_input );
            }

            double number_double()
            {
               const auto b = internal::peek_byte_safe( m_input );
               switch( b ) {
                  case std::uint8_t( internal::major::OTHER ) + 25:
                     return internal::parse_floating_half_impl( m_input );
                  case std::uint8_t( internal::major::OTHER ) + 26:
                     return internal::parse_floating_impl< float >( m_input );
                  case std::uint8_t( internal::major::OTHER ) + 27:
                     return internal::parse_floating_impl< double >( m_input );
                  default:
                     internal::throw_parse_error( m_input, "expected floating point number" );
               }
            }

            struct state_t
            {
               state_t() = default;

               explicit state_t( const std::size_t in_size )
                  : size( in_size )
               {
               }

               std::size_t i = 0;
               tao::optional< std::size_t > size;
            };

            state_t begin_container( const internal::major m, const char* e )
            {
               check_major( m, e );
               if( internal::peek_minor( m_input ) == 31 ) {
                  m_input.bump_in_this_line( 1 );
                  return state_t();
               }
               return state_t( internal::parse_size( m_input ) );
            }

            state_t begin_array()
            {
               return begin_container( internal::major::ARRAY, "expected array" );
            }

            state_t begin_object()
            {
               return begin_container( internal::major::OBJECT, "expected object" );
            }

            void end_array_sized( state_t& p )
            {
               if( *p.size != p.i ) {
                  internal::throw_parse_error( m_input, "array size mismatch" );
               }
            }

            void end_object_sized( state_t& p )
            {
               if( *p.size != p.i ) {
                  internal::throw_parse_error( m_input, "object size mismatch" );
               }
            }

            void end_array_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) != 0xff ) {
                  internal::throw_parse_error( m_input, "array not at end" );
               }
               m_input.bump_in_this_line( 1 );
            }

            void end_object_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) != 0xff ) {
                  internal::throw_parse_error( m_input, "object not at end" );
               }
               m_input.bump_in_this_line( 1 );
            }

            void end_array( state_t& p )
            {
               if( p.size ) {
                  end_array_sized( p );
               }
               else {
                  end_array_indefinitive( p );
               }
            }

            void end_object( state_t& p )
            {
               if( p.size ) {
                  end_object_sized( p );
               }
               else {
                  end_object_indefinitive( p );
               }
            }

            void element_sized( state_t& p )
            {
               if( p.i++ >= *p.size ) {
                  internal::throw_parse_error( m_input, "unexpected array end" );
               }
            }

            void member_sized( state_t& p )
            {
               if( p.i++ >= *p.size ) {
                  internal::throw_parse_error( m_input, "unexpected object end" );
               }
            }

            void element_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  internal::throw_parse_error( m_input, "unexpected array end" );
               }
            }

            void member_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  internal::throw_parse_error( m_input, "unexpected object end" );
               }
            }

            void element( state_t& p )
            {
               if( p.size ) {
                  element_sized( p );
               }
               else {
                  element_indefinitive( p );
               }
            }

            void member( state_t& p )
            {
               if( p.size ) {
                  member_sized( p );
               }
               else {
                  member_indefinitive( p );
               }
            }

            bool element_or_end_array_sized( state_t& p )
            {
               return p.i++ < *p.size;
            }

            bool member_or_end_object_sized( state_t& p )
            {
               return p.i++ < *p.size;
            }

            bool element_or_end_array_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  m_input.bump_in_this_line( 1 );
                  return false;
               }
               return true;
            }

            bool member_or_end_object_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  m_input.bump_in_this_line( 1 );
                  return false;
               }
               return true;
            }

            bool element_or_end_array( state_t& p )
            {
               if( p.size ) {
                  return element_or_end_array_sized( p );
               }
               return element_or_end_array_indefinitive( p );
            }

            bool member_or_end_object( state_t& p )
            {
               if( p.size ) {
                  return member_or_end_object_sized( p );
               }
               return member_or_end_object_indefinitive( p );
            }

            void skip_value()
            {
               // TODO: Optimise?
               json::events::discard consumer;
               json_pegtl::parse< json_pegtl::must< internal::data< V > > >( m_input, consumer );
            }

            auto mark() -> decltype( std::declval< Input >().template mark< json_pegtl::rewind_mode::REQUIRED >() )
            {
               return m_input.template mark< json_pegtl::rewind_mode::REQUIRED >();
            }

            template< typename... Ts >
            void throw_parse_error( const Ts&... ts )
            {
               internal::throw_parse_error( m_input, ts... );
            }

         private:
            Input m_input;
         };

         using parts_parser = basic_parts_parser<>;

      }  // namespace cbor

   }  // namespace json

}  // namespace tao

#endif
