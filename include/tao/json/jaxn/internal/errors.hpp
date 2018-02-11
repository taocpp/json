// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_ERRORS_HPP
#define TAO_JSON_JAXN_INTERNAL_ERRORS_HPP

#include <string>

#include "../../external/pegtl/normal.hpp"
#include "../../external/pegtl/parse_error.hpp"

#include "grammar.hpp"

#if defined( _MSC_VER )
#define WEAK_PREFIX __declspec( selectany )
#define WEAK_SUFFIX
#else
#define WEAK_PREFIX
#define WEAK_SUFFIX __attribute__( ( weak ) )
#endif

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            template< typename Rule >
            struct errors
               : json_pegtl::normal< Rule >
            {
               static const std::string error_message;

               template< typename Input, typename... States >
               static void raise( const Input& in, States&&... /*unused*/ )
               {
                  throw json_pegtl::parse_error( error_message, in );
               }

               template< template< typename... > class Action, typename Input, typename... States >
               static void apply0( const Input& in, States&&... st )
               {
                  try {
                     Action< Rule >::apply0( st... );
                  }
                  catch( const json_pegtl::parse_error& ) {
                     throw;
                  }
                  catch( const std::exception& e ) {
                     throw json_pegtl::parse_error( e.what(), in );
                  }
               }

               template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
               static void apply( const Iterator& begin, const Input& in, States&&... st )
               {
                  try {
                     using action_t = typename Input::action_t;
                     const action_t action_input( begin, in );
                     Action< Rule >::apply( action_input, st... );
                  }
                  catch( const json_pegtl::parse_error& ) {
                     throw;
                  }
                  catch( const std::exception& e ) {
                     throw json_pegtl::parse_error( e.what(), in );
                  }
               }
            };

            // clang-format off
            template<> WEAK_PREFIX const std::string errors< rules::text >::error_message WEAK_SUFFIX = "no valid JAXN";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< rules::comment >::error_message WEAK_SUFFIX = "expected '*' or '/'";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::line_comment_char >::error_message WEAK_SUFFIX = "invalid character in line comment";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::block_comment_char >::error_message WEAK_SUFFIX = "invalid character in block comment";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::end_block_comment >::error_message WEAK_SUFFIX = "unterminated multi-line comment";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< rules::end_array >::error_message WEAK_SUFFIX = "incomplete array, expected value or ']'";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::end_object >::error_message WEAK_SUFFIX = "incomplete object, expected key or '}'";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::name_separator >::error_message WEAK_SUFFIX = "expected ':'";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::value >::error_message WEAK_SUFFIX = "expected value";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< rules::edigits >::error_message WEAK_SUFFIX = "expected at least one exponent digit";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::fdigits >::error_message WEAK_SUFFIX = "expected at least one fraction digit";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::nan >::error_message WEAK_SUFFIX = "invalid numeric literal";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::infinity< false > >::error_message WEAK_SUFFIX = "invalid numeric literal";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::infinity< true > >::error_message WEAK_SUFFIX = "invalid numeric literal";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::hexnum< false > >::error_message WEAK_SUFFIX = "expected at least one hexadecimal digit";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::hexnum< true > >::error_message WEAK_SUFFIX = "expected at least one hexadecimal digit";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< rules::xdigit >::error_message WEAK_SUFFIX = "incomplete universal character name";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::escaped >::error_message WEAK_SUFFIX = "unknown escape sequence in string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::chars< '"' > >::error_message WEAK_SUFFIX = "invalid character in string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::chars< '\'' > >::error_message WEAK_SUFFIX = "invalid character in string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::qstring_content< '"' > >::error_message WEAK_SUFFIX = "unterminated string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::qstring_content< '\'' > >::error_message WEAK_SUFFIX = "unterminated string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::string_fragment >::error_message WEAK_SUFFIX = "expected string";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< rules::bescaped >::error_message WEAK_SUFFIX = "unknown escape sequence in binary string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bchars< '"' > >::error_message WEAK_SUFFIX = "invalid character in binary string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bchars< '\'' > >::error_message WEAK_SUFFIX = "invalid character in binary string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bqstring_content< '"' > >::error_message WEAK_SUFFIX = "unterminated binary string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bqstring_content< '\'' > >::error_message WEAK_SUFFIX = "unterminated binary string";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bvalue >::error_message WEAK_SUFFIX = "expected binary";  // NOLINT
            template<> WEAK_PREFIX const std::string errors< rules::bpart >::error_message WEAK_SUFFIX = "expected a pair of hexadecimal digits";  // NOLINT

            template<> WEAK_PREFIX const std::string errors< json_pegtl::eof >::error_message WEAK_SUFFIX = "unexpected character after JAXN value";  // NOLINT
            // clang-format on

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
