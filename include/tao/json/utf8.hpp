// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UTF8_HPP
#define TAO_JSON_UTF8_HPP

#include "external/pegtl/memory_input.hpp"
#include "external/pegtl/parse.hpp"
#include "external/pegtl/rules.hpp"
#include "external/pegtl/utf8.hpp"

namespace tao::json
{
   enum class utf8_mode : bool
   {
      check,
      trust
   };

   namespace internal
   {
      template< utf8_mode >
      struct utf8_todo
      {
         std::size_t todo;
      };

      template< typename Input >
      void consume_utf8_impl( Input& in, const utf8_todo< utf8_mode::check > todo )
      {
         std::size_t i = 0;
         while( i < todo.todo ) {
            const auto p = pegtl::internal::peek_utf8::peek( in, todo.todo - i ).size;
            if( ( p == 0 ) || ( ( i += p ) > todo.todo ) ) {
               throw pegtl::parse_error( "invalid utf8", in );
            }
            in.bump( p );
         }
      }

      template< typename Input >
      void consume_utf8_impl( Input& in, const utf8_todo< utf8_mode::trust > todo )
      {
         in.bump_in_this_line( todo.todo );
      }

      template< utf8_mode M, typename Input >
      void consume_utf8( Input& in, const std::size_t todo )
      {
         consume_utf8_impl( in, utf8_todo< M >{ todo } );
      }

      template< typename Input >
      bool validate_utf8( Input& in )
      {
         while( !in.empty() ) {
            if( const auto t = pegtl::internal::peek_utf8::peek( in, in.size( 4 ) ) ) {
               in.bump_in_this_line( t.size );
            }
            else {
               return false;
            }
         }
         return true;
      }

      inline bool validate_utf8( const std::string_view sv ) noexcept
      {
         pegtl::memory_input in( sv, "validate_utf8" );
         return validate_utf8( in );
      }

   }  // namespace internal

}  // namespace tao::json

#endif
