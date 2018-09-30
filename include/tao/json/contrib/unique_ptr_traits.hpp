// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_UNIQUE_PTR_TRAITS_HPP
#define TAO_JSON_CONTRIB_UNIQUE_PTR_TRAITS_HPP

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
         struct unique_ptr_traits
         {
            template< template< typename... > class Traits, typename Base >
            static std::unique_ptr< U > as( const basic_value< Traits, Base >& v )
            {
               if( v == null ) {
                  return std::unique_ptr< U >();
               }
               std::unique_ptr< U > t( new T() );  // TODO: More control?
               v.to( static_cast< T& >( *t ) );
               return t;
            }

            template< template< typename... > class Traits, typename Producer >
            static std::unique_ptr< U > consume( Producer& parser )
            {
               if( parser.null() ) {
                  return std::unique_ptr< U >();
               }
               std::unique_ptr< U > t( new T() );  // TODO: More control?
               json::consume< Traits >( parser, static_cast< T& >( *t ) );
               return t;
            }
         };

      }  // namespace internal

      template< typename T, typename U = T >
      struct unique_ptr_traits
         : public internal::unique_ptr_traits< T, U >
      {
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::unique_ptr< U >& o )
         {
            v = static_cast< const T& >( *o );  // Unconditional dereference for use in binding::factory.
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::unique_ptr< U >& o )
         {
            json::events::produce< Traits >( c, static_cast< const T& >( *o ) );  // Unconditional dereference for use in binding::factory.
         }
      };

      template< typename T >
      struct unique_ptr_traits< T, T >
         : public internal::unique_ptr_traits< T, T >,
           public internal::indirect_traits< std::unique_ptr< T > >
      {
         template< typename V >
         using with_base = unique_ptr_traits< T, V >;
      };

   }  // namespace json

}  // namespace tao

#endif
