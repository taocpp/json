// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_GRAMMAR_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_JAXN_GRAMMAR_HPP

#include "../../external/pegtl.hpp"
#include "../../external/pegtl/contrib/abnf.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace jaxn
         {
            // clang-format off
            namespace rules
            {
               using namespace json_pegtl;

               struct line_comment : seq< one< '/' >, until< eolf > > {};

               struct end_block_comment : until< json_pegtl::string< '*', '/' > > {};
               struct block_comment : if_must< one< '*' >, end_block_comment > {};

               struct comment : sor< line_comment, block_comment > {};

               struct ws : sor< one< ' ', '\t', '\n', '\r' >, if_must< one< '/' >, comment > > {};

               template< typename R, typename P = ws >
               using padr = json_pegtl::internal::seq< R, json_pegtl::internal::star< P > >;

               struct begin_array : padr< one< '[' > > {};
               struct begin_object : padr< one< '{' > > {};
               struct end_array : one< ']' > {};
               struct end_object : one< '}' > {};
               struct name_separator : pad< one< ':' >, ws > {};
               struct value_separator : padr< one< ',' > > {};
               struct element_separator : padr< one< ',' > > {};
               struct concatenate : pad< one< '+' >, ws > {};

               struct false_ : TAOCPP_JSON_PEGTL_STRING( "false" ) {};
               struct null : TAOCPP_JSON_PEGTL_STRING( "null" ) {};
               struct true_ : TAOCPP_JSON_PEGTL_STRING( "true" ) {};

               struct nan : TAOCPP_JSON_PEGTL_STRING( "NaN" ) {};

               template< bool NEG >
               struct infinity : TAOCPP_JSON_PEGTL_STRING( "Infinity" ) {};

               template< bool NEG >
               struct hexnum : plus< abnf::HEXDIG > {};

               struct digits : plus< abnf::DIGIT > {};

               struct esign : one< '-', '+' > {};

               struct edigits : digits {};
               struct fdigits : digits {};
               struct idigits : digits {};

               struct exp : seq< one< 'e', 'E' >, opt< esign >, must< edigits > > {};
               struct frac0 : seq< one< '.' >, opt< fdigits > > {};
               struct frac1 : if_must< one< '.' >, fdigits > {};

               template< bool NEG >
               struct number : seq< sor< seq< idigits, opt< frac0 > >, frac1 >, opt< exp > > {};

               struct xdigit : abnf::HEXDIG {};
               struct escaped_unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
               struct escaped_hexcode : seq< one< 'x' >, rep< 2, must< xdigit > > > {};

               struct escaped_char : one< '"', '\'', '\\', '/', 'b', 'f', 'n', 'r', 't', 'v', '0' > {};
               struct escaped : sor< escaped_char, escaped_hexcode, escaped_unicode > {};

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
                     throw json_pegtl::parse_error( "invalid character in string", in );
                  }
               };

               template< char D >
               struct chars : if_then_else< one< '\\' >, must< escaped >, unescaped< D > > {};

               template< char D >
               struct qstring_content : until< at< one< D > >, must< chars< D > > > {};

               template< char D >
               struct qstring : seq< one< D >, must< qstring_content< D > >, any > {};

               struct qstring_fragment : sor< qstring< '"' >, qstring< '\'' > > {};

               struct binary_prefix : one< '$' > {};

               struct binary_hexcode : rep< 2, abnf::HEXDIG > {};

               struct binary_part : plus< binary_hexcode > {};

               struct binary_value : list_must< binary_part, one< '.' > > {};

               struct binary_string : seq< binary_prefix, opt< binary_value > > {};

               struct string_fragment : sor< qstring< '"' >, qstring< '\'' >, binary_string > {};

               struct string : list_must< string_fragment, concatenate > {};

               struct key : list_must< qstring_fragment, concatenate > {};

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

               struct mkey : sor< key, identifier > {};
               struct member : if_must< mkey, name_separator, value > {};
               struct object_content : opt< list_tail< member, value_separator > > {};
               struct object : seq< begin_object, object_content, must< end_object > >
               {
                  using begin = begin_object;
                  using end = end_object;
                  using element = member;
                  using content = object_content;
               };

               template< bool NEG >
               struct zero {};

               struct sor_value
               {
                  // TODO: Can we use a short-cut to simply say: Yes, I guarantee progress if match() returns "true"?
                  using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::SOR, string, number< false >, object, array, false_, true_, null >;

                  template< typename Rule,
                            apply_mode A,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match_must( Input& in, States&&... st )
                  {
                     return Control< must< Rule > >::template match< A, rewind_mode::DONTCARE, Action, Control >( in, st... );
                  }

                  template< bool NEG,
                            apply_mode A,
                            rewind_mode M,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match_zero( Input& in, States&&... st )
                  {
                     if( in.size( 2 ) > 1 ) {
                        switch( in.peek_char( 1 ) ) {
                           case '.':
                           case 'e':
                           case 'E':
                              return Control< number< NEG > >::template match< A, M, Action, Control >( in, st... );

                           case 'x':
                           case 'X':
                              in.bump_in_this_line( 2 );
                              return Control< must< hexnum< NEG > > >::template match< A, M, Action, Control >( in, st... );

                           case '0':
                           case '1':
                           case '2':
                           case '3':
                           case '4':
                           case '5':
                           case '6':
                           case '7':
                           case '8':
                           case '9':
                              throw json_pegtl::parse_error( "invalid leading zero", in );
                        }
                     }
                     in.bump_in_this_line();
                     Control< zero< NEG > >::template apply0< Action >( in, st... );
                     return true;
                  }

                  template< bool NEG,
                            apply_mode A,
                            rewind_mode M,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match_number( Input& in, States&&... st )
                  {
                     switch( in.peek_char() ) {
                        case 'N':
                           return Control< must< nan > >::template match< A, M, Action, Control >( in, st... );

                        case 'I':
                           return Control< must< infinity< NEG > > >::template match< A, M, Action, Control >( in, st... );

                        case '0':
                           if( !match_zero< NEG, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                              throw json_pegtl::parse_error( "incomplete number", in );
                           }
                           return true;

                        default:
                           return Control< number< NEG > >::template match< A, M, Action, Control >( in, st... );
                     }
                  }

                  template< apply_mode A,
                            rewind_mode M,
                            template< typename... > class Action,
                            template< typename... > class Control,
                            typename Input,
                            typename... States >
                  static bool match_impl( Input& in, States&&... st )
                  {
                     switch( in.peek_char() ) {
                        case '$':
                        case '"':
                        case '\'':
                           return Control< string >::template match< A, M, Action, Control >( in, st... );

                        case '{': return Control< object >::template match< A, M, Action, Control >( in, st... );
                        case '[': return Control< array >::template match< A, M, Action, Control >( in, st... );
                        case 'n': return Control< null >::template match< A, M, Action, Control >( in, st... );
                        case 't': return Control< true_ >::template match< A, M, Action, Control >( in, st... );
                        case 'f': return Control< false_ >::template match< A, M, Action, Control >( in, st... );

                        case '+':
                           in.bump_in_this_line();
                           if( in.empty() || !match_number< false, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                              throw json_pegtl::parse_error( "incomplete number", in );
                           }
                           return true;

                        case '-':
                           in.bump_in_this_line();
                           if( in.empty() || !match_number< true, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                              throw json_pegtl::parse_error( "incomplete number", in );
                           }
                           return true;

                        default:
                           return match_number< false, A, M, Action, Control >( in, st... );
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
                     if( in.size( 2 ) && match_impl< A, M, Action, Control >( in, st... ) ) {
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

         }  // namespace jaxn

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
