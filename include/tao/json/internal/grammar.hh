// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_GRAMMAR_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_GRAMMAR_HH

#include "../external/pegtl.hh"
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

            struct comment;

            struct ws : sor<comment, one< ' ', '\t', '\n', '\r' > > {};

            template< typename R, typename P = ws > struct padr : tao_json_pegtl::internal::seq< R, tao_json_pegtl::internal::star< P > > {};

            struct begin_array : padr< one< '[' > > {};
            struct begin_object : padr< one< '{' > > {};
            struct end_array : one< ']' > {};
            struct end_object : one< '}' > {};
            struct name_separator : pad< one< ':' >, ws > {};
            struct value_separator : padr< one< ',' > > {};
            struct element_separator : padr< one< ',' > > {};

            struct false_ : tao_json_pegtl_string_t( "false" ) {};
            struct null : tao_json_pegtl_string_t( "null" ) {};
            struct true_ : tao_json_pegtl_string_t( "true" ) {};

            struct digits : tao_json_pegtl::plus< abnf::DIGIT > {};

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
            struct unicode : tao_json_pegtl::list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
            struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
            struct escaped : sor< escaped_char, unicode > {};

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
            struct array_content : opt< list_must< array_element, element_separator > > {};
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

            struct end_star_comment : until< seq< one<'*'>, one<'/'> > >{};
            struct star_comment : if_must< seq< one<'/'>, one<'*'> >,  end_star_comment> {}; 
            struct end_line_comment : until< eolf >{};
            struct line_comment : if_must< seq< one<'/'>, one<'/'> >, end_line_comment> {};

            struct comment : sor<star_comment, line_comment> {};

            struct sor_value
            {
               using analyze_t = analysis::generic< analysis::rule_type::SOR, string, number, object, array, false_, true_, null >;

               template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
               static bool match( Input & in, States && ... st )
               {
                  switch( in.peek_char() ) {
                     case '"': return Control< string >::template match< A, Action, Control >( in, st ... );
                     case '{': return Control< object >::template match< A, Action, Control >( in, st ... );
                     case '[': return Control< array >::template match< A, Action, Control >( in, st ... );
                     case 'n': return Control< null >::template match< A, Action, Control >( in, st ... );
                     case 't': return Control< true_ >::template match< A, Action, Control >( in, st ... );
                     case 'f': return Control< false_ >::template match< A, Action, Control >( in, st ... );
                     default: return Control< number >::template match< A, Action, Control >( in, st ... );
                  }
               }
            };

            struct value : padr< discard_if< sor_value > > {};
            struct array_element : seq< value > {};

            struct text : seq< star< ws >, value > {};

         } // rules

         struct grammar : tao_json_pegtl::must< rules::text, tao_json_pegtl::eof > {};

      } // internal

   } // json

} // tao

#endif
