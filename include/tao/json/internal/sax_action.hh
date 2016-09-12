// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_SAX_ACTION_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_SAX_ACTION_HH

#include "../external/pegtl/contrib/changes.hh"

#include "errors.hh"
#include "grammar.hh"

#include "sax_number_state.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template < typename Rule >
         struct sax_action : tao_json_pegtl::nothing< Rule > {};

         template<>
         struct sax_action< rules::null >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.null();
            }
         };

         template<>
         struct sax_action< rules::true_ >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.bool_( true );
            }
         };

         template<>
         struct sax_action< rules::false_ >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.bool_( false );
            }
         };

         template<>
         struct sax_action< rules::array::begin >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.begin_array();
            }
         };

         template<>
         struct sax_action< rules::array::element >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.commit_element();
            }
         };

         template<>
         struct sax_action< rules::array::end >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.end_array();
            }
         };

         template<>
         struct sax_action< rules::object::begin >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.begin_object();
            }
         };

         template<>
         struct sax_action< rules::object::element >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.commit_member();
            }
         };

         template<>
         struct sax_action< rules::object::end >
         {
            template< typename State >
            static void apply( const tao_json_pegtl::input &, State & handler )
            {
               handler.end_object();
            }
         };

         template<>
         struct sax_action< rules::msign >
         {
            static void apply( const tao_json_pegtl::input &, sax_number_state & result )
            {
               result.mneg = true;
            }
         };

         template<>
         struct sax_action< rules::esign >
         {
            static void apply( const tao_json_pegtl::input & in, sax_number_state & result )
            {
               result.eneg = ( in.peek_char() == '-' );
            }
         };

         template<>
         struct sax_action< rules::idigits >
         {
            static void apply( const tao_json_pegtl::input & in, sax_number_state & result )
            {
               if ( in.size() > ( 1 << 20 ) ) {
                  throw tao_json_pegtl::parse_error( "JSON number with 1 megabyte digits", in );
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
         struct sax_action< rules::fdigits >
         {
            static void apply( const tao_json_pegtl::input & in, sax_number_state & result )
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
         struct sax_action< rules::edigits >
         {
            static void apply( const tao_json_pegtl::input & in, sax_number_state & result )
            {
               result.isfp = true;

               const char * b = in.begin();

               while ( ( b < in.end() ) && ( b[ 0 ] == '0' ) ) {
                  ++b;
               }
               if ( ( in.end() - b ) > 9 ) {
                  throw tao_json_pegtl::parse_error( "JSON exponent has more than 9 significant digits", in );
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
