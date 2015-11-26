// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_TAOCPP_JSON_EMBEDDED_PEGTL_STRING_HH
#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_TAOCPP_JSON_EMBEDDED_PEGTL_STRING_HH

#include <type_traits>
#include <cstddef>

#include "../ascii.hh"

namespace tao_json_pegtl
{
   // Inspired by https://github.com/irrequietus/typestring
   // Rewritten and reduced to what is needed for the PEGTL
   // and to work with Visual Studio 2015.

   namespace internal
   {
      template< std::size_t N, std::size_t M >
      constexpr char string_at( const char(&c)[ M ] ) noexcept
      {
         static_assert( M <= 101, "String longer than 100 (excluding terminating \\0)!" );
         return ( N < M ) ? c[ N ] : 0;
      }

      template< typename, char ... >
      struct string_builder;

      template< typename T >
      struct string_builder< T >
      {
         using type = T;
      };

      template< template< char ... > class S, char ... Hs, char C, char ... Cs >
      struct string_builder< S< Hs ... >, C, Cs ... >
            : std::conditional< C == '\0',
                                string_builder< S< Hs ... > >,
                                string_builder< S< Hs ..., C >, Cs ... > >::type
      { };

   } // internal

} // tao_json_pegtl

#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(n,x)           \
   tao_json_pegtl::internal::string_at< n##0 >( x ),     \
   tao_json_pegtl::internal::string_at< n##1 >( x ),     \
   tao_json_pegtl::internal::string_at< n##2 >( x ),     \
   tao_json_pegtl::internal::string_at< n##3 >( x ),     \
   tao_json_pegtl::internal::string_at< n##4 >( x ),     \
   tao_json_pegtl::internal::string_at< n##5 >( x ),     \
   tao_json_pegtl::internal::string_at< n##6 >( x ),     \
   tao_json_pegtl::internal::string_at< n##7 >( x ),     \
   tao_json_pegtl::internal::string_at< n##8 >( x ),     \
   tao_json_pegtl::internal::string_at< n##9 >( x )

#define TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_100(x)            \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(,x),                \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(1,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(2,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(3,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(4,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(5,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(6,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(7,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(8,x),               \
   TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_10(9,x)

#define pegtl_string_t(x) \
   tao_json_pegtl::internal::string_builder< tao_json_pegtl::ascii::string<>, TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_100(x) >::type

#define pegtl_istring_t(x) \
   tao_json_pegtl::internal::string_builder< tao_json_pegtl::ascii::istring<>, TAOCPP_JSON_EMBEDDED_PEGTL_INTERNAL_STRING_100(x) >::type

#endif
