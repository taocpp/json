// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_SHARED_TRAITS_HPP
#define TAO_JSON_INTERNAL_SHARED_TRAITS_HPP

#include <memory>

#include "indirect_traits.hpp"

#include "../consume.hpp"
#include "../forward.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T, typename U >
         struct shared_partial_traits
         {
            template< template< typename... > class Traits, typename Base >
            static std::shared_ptr< U > as( const basic_value< Traits, Base >& v )
            {
               if( v == null ) {
                  return std::shared_ptr< U >();
               }
               auto t = std::make_shared< T >();  // TODO: More control?
               v.to( *t );
               return t;
            }

            template< template< typename... > class Traits, typename Producer >
            static std::shared_ptr< U > consume( Producer& parser )
            {
               if( parser.null() ) {
                  return std::shared_ptr< U >();
               }
               auto t = std::make_shared< T >();  // TODO: More control?
               json::consume< Traits >( parser, *t );
               return t;
            }
         };

         template< typename T, typename U = T >
         struct shared_traits
            : public shared_partial_traits< T, U >
         {
            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const std::shared_ptr< U >& o )
            {
               v = static_cast< const T& >( *o );  // Unconditional dereference for use in binding::factory.
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const std::shared_ptr< U >& o )
            {
               json::events::produce< Traits >( c, static_cast< const T& >( *o ) );  // Unconditional dereference for use in binding::factory.
            }
         };

         template< typename T >
         struct shared_traits< T, T >
            : public shared_partial_traits< T, T >,
              public indirect_traits< std::shared_ptr< T > >
         {
            template< typename V >
            using with_base = shared_traits< T, V >;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
