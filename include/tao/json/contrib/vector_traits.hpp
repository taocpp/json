// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_VECTOR_TRAITS_HPP
#define TAO_JSON_CONTRIB_VECTOR_TRAITS_HPP

#include <vector>

#include <tao/json/consume.hpp>
#include <tao/json/forward.hpp>

#include "internal/array_traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, typename... Ts >
      struct vector_traits
         : public internal::array_traits< std::vector< T, Ts... > >
      {
         template< template< typename... > class Traits, typename Base >
         static void to( const basic_value< Traits, Base >& v, std::vector< T, Ts... >& r )
         {
            const auto& a = v.get_array();
            for( const auto& i : a ) {
               r.emplace_back( i.template as< T >() );
            }
         }

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4127 )
#endif
         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& parser, std::vector< T, Ts... >& v )
         {
            auto s = parser.begin_array();
            if( s.size ) {
               v.reserve( *s.size );
            }
            while( parser.element_or_end_array( s ) ) {
               v.emplace_back( json::consume< T, Traits >( parser ) );
            }
         }
#ifdef _MSC_VER
#pragma warning( pop )
#endif
      };

   }  // namespace json

}  // namespace tao

#endif
