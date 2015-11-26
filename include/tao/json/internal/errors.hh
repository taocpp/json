// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ERRORS_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ERRORS_HH

#include <string>

#include "../external/pegtl/normal.hh"
#include "../external/pegtl/parse_error.hh"

#include "grammar.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Rule >
         struct errors
               : public tao_json_pegtl::normal< Rule >
         {
            static const std::string error_message;

            template< typename Input, typename ... States >
            static void raise( const Input & in, States && ... )
            {
               throw tao_json_pegtl::parse_error( error_message, in );
            }
         };

         template<> const std::string errors< rules::text >::error_message = "no valid JSON";

         template<> const std::string errors< rules::end_array >::error_message = "incomplete array, expected ']'";
         template<> const std::string errors< rules::end_object >::error_message = "incomplete object, expected '}'";
         template<> const std::string errors< rules::member >::error_message = "expected member";
         template<> const std::string errors< rules::name_separator >::error_message = "expected ':'";
         template<> const std::string errors< rules::array_element >::error_message = "expected value";
         template<> const std::string errors< rules::value >::error_message = "expected value";

         template<> const std::string errors< rules::edigits >::error_message = "expected at least one exponent digit";
         template<> const std::string errors< rules::fdigits >::error_message = "expected at least one fraction digit";
         template<> const std::string errors< rules::xdigit >::error_message = "incomplete universal character name";
         template<> const std::string errors< rules::escaped >::error_message = "unknown escape sequence";
         template<> const std::string errors< rules::chars >::error_message = "invalid character in string";
         template<> const std::string errors< rules::string::content >::error_message = "unterminated string";
         template<> const std::string errors< rules::key::content >::error_message = "unterminated key";

         template<> const std::string errors< tao_json_pegtl::eof >::error_message = "unexpected character after JSON value";

      } // internal

   } // json

} // tao

#endif
