// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_PARSE_UTIL_HPP
#define TAO_JSON_INTERNAL_PARSE_UTIL_HPP

#include "../external/pegtl/parse_error.hpp"

#include "endian.hpp"
#include "format.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename Input >
         void throw_on_empty( Input& in, const std::size_t required = 1 )
         {
            const auto available = in.size( required );
            if( available < required ) {
               throw json_pegtl::parse_error( format( "unexpected end of input -- required ", required, " available ", available ), in );
            }
         }

         template< typename Input >
         std::uint8_t peek_byte_safe( Input& in )
         {
            throw_on_empty( in );
            return in.peek_byte();
         }

         template< typename Input >
         char peek_char_safe( Input& in )
         {
            throw_on_empty( in );
            return in.peek_char();
         }

         template< typename Input >
         char read_char( Input& in )
         {
            throw_on_empty( in );
            const auto r = in.peek_char();
            in.bump_in_this_line( 1 );
            return r;
         }

         template< typename Input >
         char read_char_unsafe( Input& in )
         {
            const auto r = in.peek_char();
            in.bump_in_this_line( 1 );
            return r;
         }

         template< typename Result, typename Number = Result, typename Input >
         Result read_be_number_safe( Input& in, const std::size_t extra = 0 )
         {
            throw_on_empty( in, extra + sizeof( Number ) );
            const auto result = static_cast< Result >( be_to_h< Number >( in.current() + extra ) );
            in.bump_in_this_line( extra + sizeof( Number ) );
            return result;
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
