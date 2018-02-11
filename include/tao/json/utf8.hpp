// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_UTF8_HPP
#define TAO_JSON_UTF8_HPP

#include "external/pegtl/memory_input.hpp"
#include "external/pegtl/parse.hpp"
#include "external/pegtl/rules.hpp"
#include "external/pegtl/utf8.hpp"

namespace tao
{
   namespace json
   {
      enum class utf8_mode : bool
      {
         CHECK,
         TRUST
      };

      namespace internal
      {
         template< utf8_mode >
         struct utf8_todo
         {
            std::size_t todo;
         };

         template< typename Input >
         void consume_utf8_impl( Input& in, const utf8_todo< utf8_mode::CHECK > todo )
         {
            std::size_t i = 0;
            while( i < todo.todo ) {
               const auto p = json_pegtl::internal::peek_utf8::peek( in ).size;
               if( ( p == 0 ) || ( ( i += p ) > todo.todo ) ) {
                  throw json_pegtl::parse_error( "validate utf8 failed", in );
               }
               in.bump( p );
            }
         }

         template< typename Input >
         void consume_utf8_impl( Input& in, const utf8_todo< utf8_mode::TRUST > todo )
         {
            in.bump_in_this_line( todo.todo );
         }

         template< utf8_mode M, typename Input >
         void consume_utf8( Input& in, const std::size_t todo )
         {
            consume_utf8_impl( in, utf8_todo< M >{ todo } );
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
