// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_INDIRECT_TRAITS_HPP
#define TAO_JSON_INTERNAL_INDIRECT_TRAITS_HPP

#include "../forward.hpp"
#include "../type.hpp"

#include "../events/produce.hpp"

#include "type_traits.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct indirect_traits
         {
            template< template< typename... > class Traits >
            static bool is_nothing( const T& o )
            {
               return ( !bool( o ) ) || internal::is_nothing< Traits >( *o );
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T& o )
            {
               if( o ) {
                  json::events::produce< Traits >( c, *o );
               }
               else {
                  json::events::produce< Traits >( c, null );
               }
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T& o )
            {
               if( o ) {
                  v = *o;
               }
               else {
                  v = null;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
            {
               return rhs ? ( lhs == *rhs ) : ( lhs == null );
            }

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
            {
               return rhs ? ( lhs < *rhs ) : ( lhs < null );
            }

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
            {
               return rhs ? ( lhs > *rhs ) : ( lhs > null );
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
