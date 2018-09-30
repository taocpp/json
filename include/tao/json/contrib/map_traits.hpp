// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_MAP_TRAITS_HPP
#define TAO_JSON_CONTRIB_MAP_TRAITS_HPP

#include <map>
#include <string>

#include <tao/json/consume.hpp>
#include <tao/json/forward.hpp>

#include "internal/object_traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, typename... Ts >
      struct map_traits
         : public internal::object_traits< std::map< std::string, T, Ts... > >
      {
         template< template< typename... > class Traits, typename Base >
         static void to( const basic_value< Traits, Base >& v, std::map< std::string, T, Ts... >& r )
         {
            const auto& o = v.get_object();
            for( const auto& i : o ) {
               r.emplace( i.first, i.second.template as< T >() );
            }
         }

         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& parser, std::map< std::string, T, Ts... >& v )
         {
            auto s = parser.begin_object();
            while( parser.member_or_end_object( s ) ) {
               auto k = parser.key();
               v.emplace( std::move( k ), json::consume< T, Traits >( parser ) );
            }
         }
      };

   }  // namespace json

}  // namespace tao

#endif
