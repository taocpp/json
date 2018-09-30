// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_SHARED_PTR_TRAITS_HPP
#define TAO_JSON_CONTRIB_SHARED_PTR_TRAITS_HPP

#include <memory>

#include <tao/json/consume.hpp>
#include <tao/json/forward.hpp>

#include "internal/indirect_traits.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T, typename U >
         struct shared_ptr_traits
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

      }  // namespace internal

      template< typename T, typename U = T >
      struct shared_ptr_traits
         : public internal::shared_ptr_traits< T, U >
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
      struct shared_ptr_traits< T, T >
         : public internal::shared_ptr_traits< T, T >,
           public internal::indirect_traits< std::shared_ptr< T > >
      {
         template< typename V >
         using with_base = shared_ptr_traits< T, V >;
      };

   }  // namespace json

}  // namespace tao

#endif
