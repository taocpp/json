// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_ACTION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_ACTION_HH

#include <cstring>

#include "../external/pegtl/input.hh"

#include "unescape_action.hh"
#include "number_state.hh"
#include "grammar.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template < typename Rule > struct value_action : unescape_action< Rule > {};

         template<>
         struct value_action< rules::null >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & result )
            {
               result.result.unsafe_assign( nullptr );
            }
         };

         template<>
         struct value_action< rules::true_ >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & result )
            {
               result.result.unsafe_assign( true );
            }
         };

         template<>
         struct value_action< rules::false_ >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & result )
            {
               result.result.unsafe_assign( false );
            }
         };

         template<>
         struct value_action< rules::msign >
         {
            static void apply( const tao_json_pegtl::input &, number_state & result )
            {
               result.mneg = true;
            }
         };

         template<>
         struct value_action< rules::esign >
         {
            static void apply( const tao_json_pegtl::input & in, number_state & result )
            {
               result.eneg = ( in.peek_char() == '-' );
            }
         };

         template<>
         struct value_action< rules::idigits >
         {
            static void apply( const tao_json_pegtl::input & in, number_state & result )
            {
               if ( in.size() > ( 1 << 20 ) ) {
                  throw tao_json_pegtl::parse_error( "json number with 1 megabyte digits", in );
               }
               const auto c = std::min( in.size(), max_mantissa_digits );
               std::memcpy( result.mantissa + 1, in.begin(), c );
               result.exponent10 += in.size() - c;
               result.msize = c;

               for ( std::size_t i = c; i < in.size(); ++i ) {
                  if ( in.peek_char( i ) != '0' ) {
                     result.drop = true;
                     return;
                  }
               }
            }
         };

         template<>
         struct value_action< rules::fdigits >
         {
            static void apply( const tao_json_pegtl::input & in, number_state & result )
            {
               result.isfp = true;

               const auto * b = in.begin();
               const auto * e = in.end();

               while ( ( e > b ) && ( e[ -1 ] == '0' ) ) {
                  --e;
               }
               if ( ! result.msize ) {
                  while ( ( b < e ) && ( b[ 0 ] == '0' ) ) {
                     ++b;
                     --result.exponent10;
                  }
               }
               const auto c = std::min( std::size_t( e - b ), max_mantissa_digits - result.msize );
               std::memcpy( result.mantissa + 1 + result.msize, b, c );
               result.exponent10 -= c;
               result.msize += c;

               for ( const auto * r = b + c; r < e; ++r ) {
                  if ( *r != '0' ) {
                     result.drop = true;
                     return;
                  }
               }
            }
         };

         template<>
         struct value_action< rules::edigits >
         {
            static void apply( const tao_json_pegtl::input & in, number_state & result )
            {
               result.isfp = true;

               const char * b = in.begin();

               while ( ( b < in.end() ) && ( b[ 0 ] == '0' ) ) {
                  ++b;
               }
               if ( ( in.end() - b ) > 9 ) {
                  throw tao_json_pegtl::parse_error( "json exponent has more than 9 significant digits", in );
               }
               int exponent10 = 0;

               while ( b < in.end() ) {
                  exponent10 = ( exponent10 * 10 ) + ( b[ 0 ] - '0' );
                  ++b;
               }
               result.exponent10 += ( result.eneg ? -exponent10 : exponent10 );
            }
         };

      } // internal

   } // json

} // tao

#endif
