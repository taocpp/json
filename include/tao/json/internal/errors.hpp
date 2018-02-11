// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_ERRORS_HPP
#define TAO_JSON_INTERNAL_ERRORS_HPP

#include <string>

#include "../external/pegtl/normal.hpp"
#include "../external/pegtl/parse_error.hpp"

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
         template<> WEAK_PREFIX const std::string errors< rules::text >::error_message WEAK_SUFFIX = "no valid JSON";  // NOLINT

         template<> WEAK_PREFIX const std::string errors< rules::end_array >::error_message WEAK_SUFFIX = "incomplete array, expected ']'";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::end_object >::error_message WEAK_SUFFIX = "incomplete object, expected '}'";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::member >::error_message WEAK_SUFFIX = "expected member";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::name_separator >::error_message WEAK_SUFFIX = "expected ':'";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::array_element >::error_message WEAK_SUFFIX = "expected value";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::value >::error_message WEAK_SUFFIX = "expected value";  // NOLINT

         template<> WEAK_PREFIX const std::string errors< rules::edigits >::error_message WEAK_SUFFIX = "expected at least one exponent digit";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::fdigits >::error_message WEAK_SUFFIX = "expected at least one fraction digit";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::xdigit >::error_message WEAK_SUFFIX = "incomplete universal character name";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::escaped >::error_message WEAK_SUFFIX = "unknown escape sequence";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::chars >::error_message WEAK_SUFFIX = "invalid character in string";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::string::content >::error_message WEAK_SUFFIX = "unterminated string";  // NOLINT
         template<> WEAK_PREFIX const std::string errors< rules::key::content >::error_message WEAK_SUFFIX = "unterminated key";  // NOLINT

         template<> WEAK_PREFIX const std::string errors< json_pegtl::eof >::error_message WEAK_SUFFIX = "unexpected character after JSON value";  // NOLINT
         // clang-format on

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
