// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_PARSE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_PARSE_HH

#include "../external/pegtl.hh"
#include "../external/pegtl/contrib/changes.hh"
#include "../external/pegtl/contrib/unescape.hh"
#include "../external/double.hh"

#include "errors.hh"
#include "grammar.hh"

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         struct result_state
         {
            result_state() = default;
            result_state( const result_state & ) = delete;
            void operator= ( const result_state & ) = delete;

            value result;
         };

         struct string_state
         {
            string_state() = default;
            string_state( const string_state & ) = delete;
            void operator= ( const string_state & ) = delete;

            template< typename State >
            void success( State & result )
            {
               result.result.unsafe_assign( std::move( unescaped ) );
            }

            std::string unescaped;
         };

         template< typename Rule > struct unescape_action : tao_json_pegtl::nothing< Rule > {};

         template<> struct unescape_action< rules::unicode > : tao_json_pegtl::unescape::unescape_j {};
         template<> struct unescape_action< rules::escaped_char > : tao_json_pegtl::unescape::unescape_c< rules::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
         template<> struct unescape_action< rules::unescaped > : tao_json_pegtl::unescape::append_all {};

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

         static const size_t max_mantissa_digits = 772;

         struct number_state
         {
            number_state()
            {
               mantissa[ 0 ] = '-';
            }

            int32_t exponent10 = 0;
            uint16_t msize = 0;  // Excluding sign.
            bool isfp = false;
            bool mneg = false;
            bool eneg = false;
            bool drop = false;
            char mantissa[ 1 + max_mantissa_digits + 1 ];

            template< typename State >
            void success( State & result )
            {
               if ( ! isfp ) {
                  errno = 0;
                  char * p;
                  mantissa[ msize + 1 ] = 0;
                  const int64_t ll = std::strtoll( mantissa + 1 - mneg, & p, 10 );
                  if ( ( errno != ERANGE ) && ( p == mantissa + msize + 1 ) ) {
                     result.result.unsafe_assign( ll );
                     return;
                  }
               }
               if ( drop ) {
                  mantissa[ 1 + msize++ ] = '1';
                  --exponent10;
               }
               const auto d = json_double_conversion::Strtod( json_double_conversion::Vector< const char >( mantissa + 1, msize ), exponent10 );
               result.result.unsafe_assign( mneg ? -d : d );
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
               if ( in.size() >= ( 1 << 30 ) ) {
                  throw tao_json_pegtl::parse_error( "json number with 1 gigabyte digits", in );
               }
               const auto c = std::min( in.size(), max_mantissa_digits );
               ::memcpy( result.mantissa + 1, in.begin(), c );
               result.exponent10 += in.size() - c;
               result.msize = c;

               for ( size_t i = c; i < in.size(); ++i ) {
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
               const auto c = std::min( size_t( e - b ), max_mantissa_digits - result.msize );
               ::memcpy( result.mantissa + 1 + result.msize, b, c );
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

         struct array_state
               : public result_state
         {
            value array;

            array_state()
                  : array( empty_array )
            { }

            void push_back()
            {
               array.emplace_back( std::move( this->result ) );
            }

            void success( result_state & r )
            {
               r.result = std::move( array );
            }
         };

         template< typename Rule > struct array_action : tao_json_pegtl::nothing< Rule > {};

         template<>
         struct array_action< rules::array::element >
         {
            static void apply( const tao_json_pegtl::input &, array_state & result )
            {
               result.push_back();
            }
         };

         struct object_state
               : public result_state
         {
            value object;

            std::string unescaped;

            object_state()
                  : object( empty_object )
            { }

            void insert()
            {
               object.emplace( std::move( unescaped ), std::move( this->result ) );
            }

            void success( result_state & r )
            {
               r.result = std::move( object );
            }
         };

         template < typename Rule > struct object_action : unescape_action< Rule > {};

         template<>
         struct object_action< rules::object::element >
         {
            static void apply( const tao_json_pegtl::input &, object_state & result )
            {
               result.insert();
            }
         };

         template< typename Rule > struct control : errors< Rule > {};

         template<> struct control< rules::value > : tao_json_pegtl::change_action< rules::value, value_action, tao_json_pegtl::normal > {};
         template<> struct control< rules::number > : tao_json_pegtl::change_state< rules::number, number_state, tao_json_pegtl::normal > {};
         template<> struct control< rules::string::content > : tao_json_pegtl::change_state< rules::string::content, string_state, tao_json_pegtl::normal > {};
         template<> struct control< rules::array::content > : tao_json_pegtl::change_state_and_action< rules::array::content, array_state, array_action, tao_json_pegtl::normal > {};
         template<> struct control< rules::object::content > : tao_json_pegtl::change_state_and_action< rules::object::content, object_state, object_action, tao_json_pegtl::normal > {};

      } // internal

   } // json

} // tao

#endif
