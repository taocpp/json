// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_GRAMMAR_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_GRAMMAR_HH

#include "../external/pegtl/contrib/abnf.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace rules
         {
            using namespace tao_json_pegtl;

            struct ws : one< ' ', '\t', '\n', '\r' > {};

            template< typename R, typename P = ws > struct padr : tao_json_pegtl::internal::seq< R, tao_json_pegtl::internal::star< P > > {};

            struct begin_array : padr< one< '[' > > {};
            struct begin_object : padr< one< '{' > > {};
            struct end_array : one< ']' > {};
            struct end_object : one< '}' > {};
            struct name_separator : pad< one< ':' >, ws > {};
            struct value_separator : padr< one< ',' > > {};

            struct false_ : pegtl_string_t( "false" ) {};
            struct null : pegtl_string_t( "null" ) {};
            struct true_ : pegtl_string_t( "true" ) {};

            struct digits : plus< abnf::DIGIT > {};

            struct zero : one< '0' > {};
            struct msign : one< '-' > {};
            struct esign : one< '-', '+' > {};

            struct edigits : digits {};
            struct fdigits : digits {};
            struct idigits : digits {};

            struct exp : seq< one< 'e', 'E' >, opt< esign >, must< edigits > > {};
            struct frac : if_must< one< '.' >, fdigits > {};
            struct int_ : sor< zero, idigits > {};
            struct number : seq< opt< msign >, int_, opt< frac >, opt< exp > > {};

            struct xdigit : abnf::HEXDIG {};
            struct unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
            struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
            struct escaped : sor< escaped_char, unicode > {};

            //      struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
            //      struct unescaped : plus< not_at< one< '\\', '"' > >, utf8::range< 0x20, 0x10FFFF > > {};

            struct unescaped
            {
               using analyze_t = analysis::generic< analysis::rule_type::ANY >;

               template< typename Input >
               static bool match( Input & in )
               {
                  bool result = false;

                  while ( ! in.empty() ) {
                     if ( const auto t = tao_json_pegtl::internal::peek_utf8::peek( in ) ) {
                        if ( ( 0x20 <= t.data ) && ( t.data <= 0x10ffff ) && ( t.data != '\\' ) && ( t.data != '"' ) ) {
                           in.bump_in_line( t.size );
                           result = true;
                           continue;
                        }
                        return result;
                     }
                     return result;
                  }
                  return result;
               }
            };

            struct chars : if_then_else< one< '\\' >, must< escaped >, unescaped > {};

            struct string_content : until< at< one< '"' > >, must< chars > > {};
            struct string : seq< one< '"' >, must< string_content >, any >
            {
               using content = string_content;
            };

            struct key_content : until< at< one< '"' > >, must< chars > > {};
            struct key : seq< one< '"' >, must< key_content >, any >
            {
               using content = key_content;
            };

            struct value;

            struct array_element;
            struct array_content : opt< list_must< array_element, value_separator > > {};
            struct array : seq< begin_array, array_content, must< end_array > >
            {
               using begin = begin_array;
               using end = end_array;
               using element = array_element;
               using content = array_content;
            };

            struct member : if_must< key, name_separator, value > {};
            struct object_content : opt< list_must< member, value_separator > > {};
            struct object : seq< begin_object, object_content, must< end_object > >
            {
               using begin = begin_object;
               using end = end_object;
               using element = member;
               using content = object_content;
            };

            struct value : padr< sor< string, number, object, array, false_, true_, null > > {};
            struct array_element : seq< value > {};

            struct text : seq< star< ws >, value > {};

         } // rules

         struct grammar : tao_json_pegtl::must< rules::text, tao_json_pegtl::eof > {};

      } // internal

   } // json

} // tao

#endif
