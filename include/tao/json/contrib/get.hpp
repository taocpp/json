// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_GET_HPP
#define TAO_JSON_CONTRIB_GET_HPP

#include "../basic_value.hpp"

namespace tao
{
   namespace json
   {
      namespace get
      {
         namespace internal
         {
            template< template< typename... > class Traits >
            const basic_value< Traits >* find( const basic_value< Traits >& v, const std::string& key )
            {
               return v.skip_value_ptr().find( key );
            }

            template< template< typename... > class Traits >
            const basic_value< Traits >* find( const basic_value< Traits >& v, const std::size_t index )
            {
               const auto& a = v.skip_value_ptr().get_array();
               return ( index < a.size() ) ? ( a.data() + index ) : nullptr;
            }

         }  // namespace internal

         template< template< typename... > class Traits, typename K >
         basic_value< Traits > value( const basic_value< Traits >& v, const K& key )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return basic_value< Traits >( p );  // Returns a Value with tao::json::type::VALUE_PTR that is invalidated when v is destroyed!
               }
            }
            return basic_value< Traits >();
         }

         template< template< typename... > class Traits, typename K, typename... Ks >
         basic_value< Traits > value( const basic_value< Traits >& v, const K& key, const Ks&... ks )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return get::value( *p, ks... );
               }
            }
            return basic_value< Traits >();
         }

         template< typename T, typename U = T, template< typename... > class Traits >
         T as( const basic_value< Traits >& v )
         {
            return v.skip_value_ptr().template as< U >();
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename K, typename... Ks >
         T as( const basic_value< Traits >& v, const K& key, Ks&&... ks )
         {
            return get::as< T, U >( v.skip_value_ptr().at( key ), ks... );
         }

         template< typename T, template< typename... > class Traits >
         void to( const basic_value< Traits >& v, T& t )
         {
            v.skip_value_ptr().to( t );
         }

         template< typename T, template< typename... > class Traits, typename K, typename... Ks >
         void to( const basic_value< Traits >& v, T& t, const K& key, Ks&&... ks )
         {
            get::to( v.skip_value_ptr().at( key ), t, ks... );
         }

         template< typename T, typename U = T, template< typename... > class Traits >
         std::optional< T > optional( const basic_value< Traits >& v )
         {
            if( v ) {
               return std::optional< T >( std::in_place, get::as< U >( v ) );
            }
            return std::nullopt;
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename K, typename... Ks >
         std::optional< T > optional( const basic_value< Traits >& v, const K& key, const Ks&... ks )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return get::optional< T, U >( *p, ks... );
               }
            }
            return std::nullopt;
         }

         template< typename T, typename U = T, template< typename... > class Traits >
         T defaulted( const T& t, const basic_value< Traits >& v )
         {
            if( v ) {
               return get::as< T, U >( v );
            }
            return t;
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename K, typename... Ks >
         T defaulted( const T& t, const basic_value< Traits >& v, const K& key, const Ks&... ks )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return get::defaulted< T, U >( t, *p, ks... );
               }
            }
            return t;
         }

      }  // namespace get

   }  // namespace json

}  // namespace tao

#endif
