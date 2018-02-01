// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <tao/json/external/pegtl.hpp>
#include <tao/json/internal/jaxn/grammar.hpp>

using namespace tao::json_pegtl;

namespace config
{
   // clang-format off
   namespace rules
   {
      namespace jaxn = tao::json::internal::jaxn::rules;

      using ws = plus< jaxn::ws >;

      struct value;
      struct identifier : plus< ranges< 'a', 'z', 'A', 'Z', '0', '9', '-', '-', '_', '_', '$' > > {};

      struct rkey;
      struct function : seq< identifier, ws, list< value, jaxn::element_separator > > {};
      struct expression : if_must< string< '$', '(' >, sor< function, rkey >, one< ')' > > {};

      struct xdigit : abnf::HEXDIG {};
      struct escaped_unicode_code_point_content : seq< one< '{' >, plus< xdigit > > {};
      struct escaped_unicode_code_point : seq< one< 'u' >, escaped_unicode_code_point_content, one< '}' > > {};
      struct escaped_unicode : list< seq< one< 'u' >, rep< 4, xdigit > >, one< '\\' > > {};

      struct escaped_char : one< '"', '\'', '\\', '/', 'b', 'f', 'n', 'r', 't', 'v', '0' > {};
      struct escaped : sor< escaped_char, escaped_unicode_code_point, escaped_unicode > {};

      template< char D >
      struct unescaped
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input& in )
         {
            bool result = false;

            while( !in.empty() ) {
               if( const auto t = internal::peek_utf8::peek( in ) ) {
                  if( ( 0x20 <= t.data ) && ( t.data <= 0x10FFFF ) && ( t.data != '\\' ) && ( t.data != D ) ) {
                     in.bump_in_this_line( t.size );
                     result = true;
                     continue;
                  }
               }
               return result;
            }
            throw parse_error( "invalid character in string", in );
         }
      };

      template< char D >
      struct chars : if_then_else< one< '\\' >, must< escaped >, unescaped< D > > {};

      template< char D >
      struct qstring_content : until< at< one< D > >, must< chars< D > > > {};

      template< char D >
      struct qstring : seq< one< D >, must< qstring_content< D > >, any > {};

      struct string_fragment : sor< qstring< '"' >, qstring< '\'' >, expression > {};

      struct string : list_must< string_fragment, jaxn::value_concat > {};

      struct binary_prefix : one< '$' > {};

      struct bescaped_hexcode : seq< one< 'x' >, rep< 2, must< xdigit > > > {};

      struct bescaped_char : one< '"', '\'', '\\', '/', 'b', 'f', 'n', 'r', 't', 'v', '0' > {};
      struct bescaped : sor< bescaped_char, bescaped_hexcode > {};

      template< char D >
      struct bunescaped
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input& in )
         {
            bool result = false;

            while( !in.empty() ) {
               const auto t = in.peek_char();
               if( ( 0x20 <= t ) && ( t <= 0x7E ) && ( t != '\\' ) && ( t != D ) ) {
                  in.bump_in_this_line( 1 );
                  result = true;
                  continue;
               }
               return result;
            }
            throw parse_error( "invalid character in binary string", in );
         }
      };

      template< char D >
      struct bchars : if_then_else< one< '\\' >, must< bescaped >, bunescaped< D > > {};

      template< char D >
      struct bqstring_content : until< at< one< D > >, must< bchars< D > > > {};

      template< char D >
      struct bqstring : seq< one< D >, must< bqstring_content< D > >, any > {};

      struct bstring : sor< bqstring< '"' >, bqstring< '\'' > > {};

      struct bbyte : rep< 2, abnf::HEXDIG > {};

      struct bpart : plus< bbyte > {};

      struct bdirect : list_must< bpart, one< '.' > > {};

      struct bvalue : seq< binary_prefix, opt< sor< bstring, bdirect > > > {};

      struct binary : list_must< sor< expression, bvalue >, jaxn::value_concat > {};

      struct array_element;
      struct array_content : opt< list_tail< array_element, jaxn::element_separator > > {};
      struct array : seq< jaxn::begin_array, array_content, must< jaxn::end_array > >
      {
         using begin = jaxn::begin_array;
         using end = jaxn::end_array;
         using element = array_element;
         using content = array_content;
      };

      struct key : string {};

      struct mkey_part : sor< key, identifier > {};
      struct mkey : list< mkey_part, one< '.' > > {};
      struct member : if_must< mkey, jaxn::name_separator, value > {};
      struct object_content : star< member, opt< jaxn::value_separator > > {};
      struct object : seq< jaxn::begin_object, object_content, must< jaxn::end_object > >
      {
         using begin = jaxn::begin_object;
         using end = jaxn::end_object;
         using element = member;
         using content = object_content;
      };

      struct rkey_part : sor< key, identifier, expression > {};
      struct rkey : list< mkey_part, one< '.' > > {};
      struct expression_list : seq< expression, star< jaxn::value_concat, sor< expression, must< sor< string, binary > > > > > {};

      struct sor_value
      {
         using analyze_t = analysis::generic< analysis::rule_type::SOR, string, jaxn::number< false >, object, array, jaxn::false_, jaxn::true_, jaxn::null >;

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
                     return Control< jaxn::number< NEG > >::template match< A, M, Action, Control >( in, st... );

                  case 'x':
                  case 'X':
                     in.bump_in_this_line( 2 );
                     return Control< must< jaxn::hexnum< NEG > > >::template match< A, M, Action, Control >( in, st... );

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
                     throw parse_error( "invalid leading zero", in );
               }
            }
            in.bump_in_this_line();
            // TODO: Control< jaxn::zero< NEG > >::template apply0< Action >( in, st... );
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
                  return Control< must< jaxn::nan > >::template match< A, M, Action, Control >( in, st... );

               case 'I':
                  return Control< must< jaxn::infinity< NEG > > >::template match< A, M, Action, Control >( in, st... );

               case '0':
                  if( !match_zero< NEG, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                     throw parse_error( "incomplete number", in );
                  }
                  return true;

               default:
                  return Control< jaxn::number< NEG > >::template match< A, M, Action, Control >( in, st... );
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
               case '{': return Control< object >::template match< A, M, Action, Control >( in, st... );
               case '[': return Control< array >::template match< A, M, Action, Control >( in, st... );
               case 'n': return Control< jaxn::null >::template match< A, M, Action, Control >( in, st... );
               case 't': return Control< jaxn::true_ >::template match< A, M, Action, Control >( in, st... );
               case 'f': return Control< jaxn::false_ >::template match< A, M, Action, Control >( in, st... );

               case '"':
               case '\'':
                  return Control< string >::template match< A, M, Action, Control >( in, st... );

               case '$':
                  if( in.peek_char( 1 ) == '(' ) {
                     return Control< expression_list >::template match< A, M, Action, Control >( in, st... );
                  }
                  else {
                     return Control< binary >::template match< A, M, Action, Control >( in, st... );
                  }

               case '+':
                  in.bump_in_this_line();
                  if( in.empty() || !match_number< false, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                     throw parse_error( "incomplete number", in );
                  }
                  return true;

               case '-':
                  in.bump_in_this_line();
                  if( in.empty() || !match_number< true, A, rewind_mode::DONTCARE, Action, Control >( in, st... ) ) {
                     throw parse_error( "incomplete number", in );
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

      struct value : jaxn::padr< sor_value > {};
      struct array_element : value {};

      struct kw_include : TAOCPP_JSON_PEGTL_STRING( "include" ) {};
      struct kw_delete : TAOCPP_JSON_PEGTL_STRING( "delete" ) {};

      struct include_file : seq< kw_include, ws, string > {};
      struct delete_keys : seq< kw_delete, ws, mkey > {};

      struct statement : sor< include_file, delete_keys, member > {};

      struct grammar : until< eof, sor< ws, must< rules::statement > > > {};
      // clang-format on

   }  // namespace rules

}  // namespace config

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      file_input<> in( argv[ i ] );
      parse< config::rules::grammar >( in );
   }
   return 0;
}
