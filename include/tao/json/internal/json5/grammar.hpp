// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JSON5_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JSON5_GRAMMAR_HPP

#include "../../external/pegtl.hpp"
#include "../../external/pegtl/contrib/abnf.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace json5
         {
            // clang-format off
            namespace rules
            {
               using namespace json_pegtl;

               struct single_line_comment : seq< one< '/' >, until< eolf > > {};

               struct end_multi_line_comment : until< json_pegtl::string< '*', '/' > > {};
               struct multi_line_comment : if_must< one< '*' >, end_multi_line_comment > {};

               struct comment : sor< single_line_comment, multi_line_comment > {};

               struct ws : sor< one< ' ', '\t', '\n', '\r' >, utf8::one< 0xA0, 0xFEFF >, if_must< one< '/' >, comment > > {};

               template< typename R, typename P = ws >
               using padr = json_pegtl::internal::seq< R, json_pegtl::internal::star< P > >;

               struct begin_array : padr< one< '[' > > {};
               struct begin_object : padr< one< '{' > > {};
               struct end_array : one< ']' > {};
               struct end_object : one< '}' > {};
               struct name_separator : pad< one< ':' >, ws > {};
               struct value_separator : padr< one< ',' > > {};
               struct element_separator : padr< one< ',' > > {};

               struct false_ : TAOCPP_JSON_PEGTL_STRING( "false" ) {};
               struct null : TAOCPP_JSON_PEGTL_STRING( "null" ) {};
               struct true_ : TAOCPP_JSON_PEGTL_STRING( "true" ) {};

               struct nan : TAOCPP_JSON_PEGTL_STRING( "NaN" ) {};
               struct infinity : TAOCPP_JSON_PEGTL_STRING( "Infinity" ) {};

               struct digits : plus< abnf::DIGIT > {};

               struct zero : one< '0' > {};
               struct msign : one< '-', '+' > {};
               struct esign : one< '-', '+' > {};

               struct edigits : digits {};
               struct fdigits : digits {};
               struct idigits : digits {};

               struct exp : seq< one< 'e', 'E' >, opt< esign >, must< edigits > > {};
               struct int_ : sor< zero, idigits > {};
               struct number : seq< opt< msign >, sor< nan, infinity, seq< int_, opt< seq< one< '.' >, opt< fdigits > >, opt< exp > > >, seq< one< '.' >, must< fdigits >, opt< exp > > > > {};

               struct hexcontent : plus< abnf::HEXDIG > {};
               struct hexnumber : seq< bytes< 2 >, must< hexcontent > > {};

               struct xdigit : abnf::HEXDIG {};
               struct escaped_unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};

               struct escaped_char : one< 'b', 'f', 'n', 'r', 't' > {};
               struct escaped_eol : eol {};
               struct escaped_any : any {};
               struct escaped : sor< escaped_char, escaped_unicode, escaped_eol, escaped_any > {};

               template< char D >
               struct unescaped
               {
                  using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::ANY >;

                  template< typename Input >
                  static bool match( Input& in )
                  {
                     bool result = false;

                     while( !in.empty() ) {
                        if( const auto t = json_pegtl::internal::peek_utf8::peek( in ) ) {
                           if( ( 0x20 <= t.data ) && ( t.data <= 0x10ffff ) && ( t.data != '\\' ) && ( t.data != D ) ) {
                              in.bump_in_this_line( t.size );
                              result = true;
                              continue;
                           }
                        }
                        return result;
                     }
                     throw std::logic_error( "code should be unreachable" );  // LCOV_EXCL_LINE
                  }
               };

               template< char D >
               struct chars : if_then_else< one< '\\' >, escaped, unescaped< D > > {};

               template< char D >
               struct string_content : until< at< one< D > >, must< chars< D > > > {};

               template< char D >
               struct string : seq< one< D >, must< string_content< D > >, any >
               {
                  using content = string_content< D >;
               };

               template< char D >
               struct key_content : until< at< one< D > >, must< chars< D > > > {};

               template< char D >
               struct key : seq< one< D >, must< key_content< D > >, any >
               {
                  using content = key_content< D >;
               };

               // TODO: Check if this is correct/complete wrt JSON5
               struct identifier_first : ranges< 'a', 'z', 'A', 'Z', '_', '_', '$' > {};
               struct identifier_other : ranges< 'a', 'z', 'A', 'Z', '0', '9', '_', '_', '$' > {};
               struct identifier : seq< identifier_first, star< identifier_other > > {};

               struct value;

               struct array_element;
               struct array_content : opt< list_tail< array_element, element_separator > > {};
               struct array : seq< begin_array, array_content, must< end_array > >
               {
                  using begin = begin_array;
                  using end = end_array;
                  using element = array_element;
                  using content = array_content;
               };

               struct member : if_must< sor< key< '"' >, key< '\'' >, identifier >, name_separator, value > {};
               struct object_content : opt< list_tail< member, value_separator > > {};
               struct object : seq< begin_object, object_content, must< end_object > >
               {
                  using begin = begin_object;
                  using end = end_object;
                  using element = member;
                  using content = object_content;
               };

               struct sor_value
               {
                  using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::SOR, string< '"' >, string< '\'' >, hexnumber, number, object, array, false_, true_, null >;

                  template< apply_mode A,
                            rewind_mode M,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match_impl( Input& in, States&&... st )
                  {
                     switch( in.peek_char() ) {
                        case '\'': return Control< string< '\'' > >::template match< A, M, Action, Control >( in, st... );
                        case '"': return Control< string< '"' > >::template match< A, M, Action, Control >( in, st... );
                        case '{': return Control< object >::template match< A, M, Action, Control >( in, st... );
                        case '[': return Control< array >::template match< A, M, Action, Control >( in, st... );
                        case 'n': return Control< null >::template match< A, M, Action, Control >( in, st... );
                        case 't': return Control< true_ >::template match< A, M, Action, Control >( in, st... );
                        case 'f': return Control< false_ >::template match< A, M, Action, Control >( in, st... );
                        case '0': {
                           const auto c = in.peek_char( 1 );
                           if( c == 'x' || c == 'X' ) {
                              return Control< hexnumber >::template match< A, M, Action, Control >( in, st... );
                           }
                           return Control< number >::template match< A, M, Action, Control >( in, st... );
                        }
                        default: return Control< number >::template match< A, M, Action, Control >( in, st... );
                     }
                  }

                  template< apply_mode A,
                            rewind_mode M,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match( Input& in, States&&... st )
                  {
                     if( in.size( 1 ) && match_impl< A, M, Action, Control >( in, st... ) ) {
                        in.discard();
                        return true;
                     }
                     return false;
                  }
               };

               struct value : padr< sor_value > {};
               struct array_element : value {};

               struct text : seq< star< ws >, value > {};

            }  // namespace rules

            struct grammar : json_pegtl::must< rules::text, json_pegtl::eof > {};
            // clang-format on

         }  // namespace json5

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
