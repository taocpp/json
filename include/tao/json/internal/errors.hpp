// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_ERRORS_HPP
#define TAO_JSON_INTERNAL_ERRORS_HPP

#include <string>

#include "../external/pegtl/normal.hpp"
#include "../external/pegtl/parse_error.hpp"

#include "grammar.hpp"

#if defined( _MSC_VER )
#define TAO_JSON_WEAK_PREFIX __declspec( selectany )
#define TAO_JSON_WEAK_SUFFIX
#else
#define TAO_JSON_WEAK_PREFIX
// clang-format off
#define TAO_JSON_WEAK_SUFFIX __attribute__(( weak ))
// clang-format on
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
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::text >::error_message TAO_JSON_WEAK_SUFFIX = "no valid JSON";  // NOLINT

         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::end_array >::error_message TAO_JSON_WEAK_SUFFIX = "incomplete array, expected ']'";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::end_object >::error_message TAO_JSON_WEAK_SUFFIX = "incomplete object, expected '}'";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::member >::error_message TAO_JSON_WEAK_SUFFIX = "expected member";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::name_separator >::error_message TAO_JSON_WEAK_SUFFIX = "expected ':'";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::array_element >::error_message TAO_JSON_WEAK_SUFFIX = "expected value";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::value >::error_message TAO_JSON_WEAK_SUFFIX = "expected value";  // NOLINT

         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::edigits >::error_message TAO_JSON_WEAK_SUFFIX = "expected at least one exponent digit";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::fdigits >::error_message TAO_JSON_WEAK_SUFFIX = "expected at least one fraction digit";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::xdigit >::error_message TAO_JSON_WEAK_SUFFIX = "incomplete universal character name";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::escaped >::error_message TAO_JSON_WEAK_SUFFIX = "unknown escape sequence";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::unescaped >::error_message TAO_JSON_WEAK_SUFFIX = "invalid unescaped character";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::string::content >::error_message TAO_JSON_WEAK_SUFFIX = "unterminated string";  // NOLINT
         template<> TAO_JSON_WEAK_PREFIX const std::string errors< rules::key::content >::error_message TAO_JSON_WEAK_SUFFIX = "unterminated key";  // NOLINT

         template<> TAO_JSON_WEAK_PREFIX const std::string errors< json_pegtl::eof >::error_message TAO_JSON_WEAK_SUFFIX = "unexpected character after JSON value";  // NOLINT
         // clang-format on

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#undef TAO_JSON_WEAK_PREFIX
#undef TAO_JSON_WEAK_SUFFIX

#endif
