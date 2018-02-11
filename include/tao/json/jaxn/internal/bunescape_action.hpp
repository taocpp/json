// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_INTERNAL_BUNESCAPE_ACTION_HPP
#define TAO_JSON_JAXN_INTERNAL_BUNESCAPE_ACTION_HPP

#include "../../external/byte.hpp"
#include "../../external/pegtl/contrib/unescape.hpp"
#include "../../external/pegtl/nothing.hpp"

#include "grammar.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         namespace internal
         {
            template< typename Rule >
            struct bunescape_action : json_pegtl::nothing< Rule >
            {
            };

            template<>
            struct bunescape_action< rules::bescaped_char >
            {
               template< typename Input, typename State >
               static void apply( const Input& in, State& st )
               {
                  switch( *in.begin() ) {
                     case '"':
                        st.value.push_back( tao::byte( '"' ) );
                        break;

                     case '\'':
                        st.value.push_back( tao::byte( '\'' ) );
                        break;

                     case '\\':
                        st.value.push_back( tao::byte( '\\' ) );
                        break;

                     case '/':
                        st.value.push_back( tao::byte( '/' ) );
                        break;

                     case 'b':
                        st.value.push_back( tao::byte( '\b' ) );
                        break;

                     case 'f':
                        st.value.push_back( tao::byte( '\f' ) );
                        break;

                     case 'n':
                        st.value.push_back( tao::byte( '\n' ) );
                        break;

                     case 'r':
                        st.value.push_back( tao::byte( '\r' ) );
                        break;

                     case 't':
                        st.value.push_back( tao::byte( '\t' ) );
                        break;

                     case 'v':
                        st.value.push_back( tao::byte( '\v' ) );
                        break;

                     case '0':
                        st.value.push_back( tao::byte( '\0' ) );
                        break;

                     default:
                        throw std::runtime_error( "invalid character in unescape" );  // NOLINT, LCOV_EXCL_LINE
                  }
               }
            };

            template<>
            struct bunescape_action< rules::bescaped_hexcode >
            {
               template< typename Input, typename State >
               static void apply( const Input& in, State& st )
               {
                  assert( !in.empty() );  // First character MUST be present, usually 'x'.
                  st.value.push_back( static_cast< tao::byte >( json_pegtl::unescape::unhex_string< char >( in.begin() + 1, in.end() ) ) );
               }
            };

            template< char D >
            struct bunescape_action< rules::bunescaped< D > >
            {
               template< typename Input, typename State >
               static void apply( const Input& in, State& st )
               {
                  const auto begin = static_cast< const tao::byte* >( static_cast< const void* >( in.begin() ) );
                  const auto end = begin + in.size();
                  st.value.insert( st.value.end(), begin, end );
               }
            };

            template<>
            struct bunescape_action< rules::bbyte >
            {
               template< typename Input, typename State >
               static void apply( const Input& in, State& st )
               {
                  st.value.push_back( static_cast< tao::byte >( json_pegtl::unescape::unhex_string< char >( in.begin(), in.end() ) ) );
               }
            };

         }  // namespace internal

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
