// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_ACTION_HPP
#define TAO_JSON_INTERNAL_ACTION_HPP

#include "../external/pegtl/nothing.hpp"

#include "errors.hpp"
#include "grammar.hpp"

#include "number_state.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule >
         struct action
            : json_pegtl::nothing< Rule >
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
         struct action< rules::esign >
         {
            template< typename Input, bool NEG >
            static void apply( const Input& in, number_state< NEG >& result )
            {
               result.eneg = ( in.peek_char() == '-' );
            }
         };

         template<>
         struct action< rules::idigits >
         {
            template< typename Input, bool NEG >
            static void apply( const Input& in, number_state< NEG >& result )
            {
               const auto s = in.size();

               if( s == 1 && in.peek_char() == '0' ) {
                  return;
               }

               if( s > ( 1 << 20 ) ) {
                  throw std::runtime_error( "JSON number with 1 megabyte digits" );  // NOLINT
               }

               const auto c = std::min( s, max_mantissa_digits );
               std::memcpy( result.mantissa, in.begin(), c );
               result.exponent10 += static_cast< typename number_state< NEG >::exponent10_t >( s - c );
               result.msize = static_cast< typename number_state< NEG >::msize_t >( c );

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
            static void apply( const Input& in, number_state< NEG >& result )
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
               const auto c = std::min( std::size_t( e - b ), max_mantissa_digits - result.msize );
               std::memcpy( result.mantissa + result.msize, b, c );
               result.exponent10 -= static_cast< typename number_state< NEG >::exponent10_t >( c );
               result.msize += static_cast< typename number_state< NEG >::msize_t >( c );

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
            static void apply( const Input& in, number_state< NEG >& result )
            {
               result.isfp = true;

               const char* b = in.begin();

               while( ( b < in.end() ) && ( b[ 0 ] == '0' ) ) {
                  ++b;
               }
               if( ( in.end() - b ) > 9 ) {
                  throw std::runtime_error( "JSON exponent has more than 9 significant digits" );  // NOLINT
               }
               int exponent10 = 0;

               while( b < in.end() ) {
                  exponent10 = ( exponent10 * 10 ) + ( b[ 0 ] - '0' );
                  ++b;
               }
               result.exponent10 += ( result.eneg ? -exponent10 : exponent10 );
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
