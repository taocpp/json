// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_ACTION_HPP
#define TAO_JSON_JAXN_INTERNAL_ACTION_HPP

#include <string_view>

#include "../../external/pegtl/contrib/change_action_and_state.hpp"
#include "../../external/pegtl/contrib/change_state.hpp"

#include "../../internal/key_state.hpp"
#include "../../internal/number_state.hpp"
#include "../../internal/string_state.hpp"

#include "binary_state.hpp"
#include "bunescape_action.hpp"
#include "grammar.hpp"
#include "unescape_action.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            template< typename Rule >
            struct action
            {
            };

            template<>
            struct action< rules::null >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.null();
               }
            };

            template<>
            struct action< rules::true_ >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.boolean( true );
               }
            };

            template<>
            struct action< rules::false_ >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.boolean( false );
               }
            };

            template<>
            struct action< rules::identifier >
            {
               template< typename Input, typename Consumer >
               static void apply( const Input& in, Consumer& consumer )
               {
                  consumer.key( std::string_view( in.begin(), in.size() ) );
               }
            };

            template< bool NEG >
            struct action< rules::hexnum< NEG > >
            {
               static char unhex( const char c )
               {
                  if( ( '0' <= c ) && ( c <= '9' ) ) {
                     return c - '0';
                  }
                  if( ( 'a' <= c ) && ( c <= 'f' ) ) {
                     return c - 'a' + 10;
                  }
                  if( ( 'A' <= c ) && ( c <= 'F' ) ) {
                     return c - 'A' + 10;
                  }
                  // LCOV_EXCL_START
                  assert( false );
                  return 0;
                  // LCOV_EXCL_STOP
               }

               template< typename Input, typename Consumer >
               static void apply( const Input& in, Consumer& consumer )
               {
                  std::uint64_t value = 0;
                  for( char c : in ) {
                     if( value & 0xF000000000000000 ) {
                        throw json_pegtl::parse_error( "JAXN hexadecimal number too large", in );  // NOLINT
                     }
                     value <<= 4;
                     value += unhex( c );
                  }
                  if( NEG ) {
                     if( value < 9223372036854775808ull ) {
                        consumer.number( -static_cast< std::int64_t >( value ) );
                     }
                     else if( value == 9223372036854775808ull ) {
                        consumer.number( static_cast< std::int64_t >( -9223372036854775807ll - 1 ) );
                     }
                     else {
                        throw json_pegtl::parse_error( "JAXN hexadecimal number too large to negate", in );  // NOLINT
                     }
                  }
                  else {
                     consumer.number( value );
                  }
               }
            };

            template<>
            struct action< rules::array::begin >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.begin_array();
               }
            };

            template<>
            struct action< rules::array::element >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.element();
               }
            };

            template<>
            struct action< rules::array::end >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.end_array();
               }
            };

            template<>
            struct action< rules::object::begin >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.begin_object();
               }
            };

            template<>
            struct action< rules::object::element >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.member();
               }
            };

            template<>
            struct action< rules::object::end >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.end_object();
               }
            };

            template<>
            struct action< rules::zero< false > >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.number( std::uint64_t( 0 ) );
               }
            };

            template<>
            struct action< rules::zero< true > >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.number( std::int64_t( 0 ) );
               }
            };

            template<>
            struct action< rules::nan >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.number( NAN );
               }
            };

            template<>
            struct action< rules::infinity< false > >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.number( INFINITY );
               }
            };

            template<>
            struct action< rules::infinity< true > >
            {
               template< typename Consumer >
               static void apply0( Consumer& consumer )
               {
                  consumer.number( -INFINITY );
               }
            };

            template<>
            struct action< rules::esign >
            {
               template< typename Input, bool NEG >
               static void apply( const Input& in, json::internal::number_state< NEG >& result )
               {
                  result.eneg = ( in.peek_char() == '-' );
               }
            };

            template<>
            struct action< rules::idigits >
            {
               template< typename Input, bool NEG >
               static void apply( const Input& in, json::internal::number_state< NEG >& result )
               {
                  const auto s = in.size();

                  if( s == 1 && in.peek_char() == '0' ) {
                     return;
                  }

                  if( s > ( 1 << 20 ) ) {
                     throw json_pegtl::parse_error( "JSON number with 1 megabyte digits", in );  // NOLINT
                  }

                  const auto c = ( std::min )( s, json::internal::max_mantissa_digits );
                  std::memcpy( result.mantissa, in.begin(), c );
                  result.exponent10 += static_cast< typename json::internal::number_state< NEG >::exponent10_t >( s - c );
                  result.msize = static_cast< typename json::internal::number_state< NEG >::msize_t >( c );

                  for( std::size_t i = c; i < s; ++i ) {
                     if( in.peek_char( i ) != '0' ) {
                        result.drop = true;
                        return;
                     }
                  }
               }
            };

            template<>
            struct action< rules::fdigits >
            {
               template< typename Input, bool NEG >
               static void apply( const Input& in, json::internal::number_state< NEG >& result )
               {
                  result.isfp = true;

                  const auto* b = in.begin();
                  const auto* e = in.end();

                  while( ( e > b ) && ( e[ -1 ] == '0' ) ) {
                     --e;
                  }
                  if( !result.msize ) {
                     while( ( b < e ) && ( b[ 0 ] == '0' ) ) {
                        ++b;
                        --result.exponent10;
                     }
                  }
                  const auto c = ( std::min )( std::size_t( e - b ), json::internal::max_mantissa_digits - result.msize );
                  std::memcpy( result.mantissa + result.msize, b, c );
                  result.exponent10 -= static_cast< typename json::internal::number_state< NEG >::exponent10_t >( c );
                  result.msize += static_cast< typename json::internal::number_state< NEG >::msize_t >( c );

                  for( const auto* r = b + c; r < e; ++r ) {
                     if( *r != '0' ) {
                        result.drop = true;
                        return;
                     }
                  }
               }
            };

            template<>
            struct action< rules::edigits >
            {
               template< typename Input, bool NEG >
               static void apply( const Input& in, json::internal::number_state< NEG >& result )
               {
                  result.isfp = true;

                  const char* b = in.begin();

                  while( ( b < in.end() ) && ( b[ 0 ] == '0' ) ) {
                     ++b;
                  }
                  if( ( in.end() - b ) > 9 ) {
                     throw json_pegtl::parse_error( "JSON exponent has more than 9 significant digits", in );  // NOLINT
                  }
                  int exponent10 = 0;

                  while( b < in.end() ) {
                     exponent10 = ( exponent10 * 10 ) + ( b[ 0 ] - '0' );
                     ++b;
                  }
                  result.exponent10 += ( result.eneg ? -exponent10 : exponent10 );
               }
            };

            template< bool NEG >
            struct action< rules::number< NEG > >
               : json_pegtl::change_state< tao::json::internal::number_state< NEG > >
            {
            };

            template<>
            struct action< rules::string >
               : json_pegtl::change_action_and_state< unescape_action, tao::json::internal::string_state >
            {
            };

            template<>
            struct action< rules::key >
               : json_pegtl::change_action_and_state< unescape_action, tao::json::internal::key_state >
            {
            };

            template<>
            struct action< rules::binary >
               : json_pegtl::change_action_and_state< bunescape_action, binary_state >
            {
            };

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
