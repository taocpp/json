// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_GET_HPP
#define TAO_JSON_CONTRIB_GET_HPP

#include <tao/json/basic_value.hpp>

namespace tao
{
   namespace json
   {
      namespace get
      {
         namespace internal
         {
            template< template< typename... > class Traits, typename Base >
            const basic_value< Traits, Base >* find( const basic_value< Traits, Base >& v, const std::string& key )
            {
               return v.skip_value_ptr().find( key );
            }

            template< template< typename... > class Traits, typename Base >
            const basic_value< Traits, Base >* find( const basic_value< Traits, Base >& v, const std::size_t index )
            {
               const auto& a = v.skip_value_ptr().get_array();
               return ( index < a.size() ) ? ( a.data() + index ) : nullptr;
            }

         }  // namespace internal

         template< template< typename... > class Traits, typename Base, typename K >
         basic_value< Traits, Base > value( const basic_value< Traits, Base >& v, const K& key )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return basic_value< Traits, Base >( p );  // Returns a Value with tao::json::type::VALUE_PTR that is invalidated when v is destroyed!
               }
            }
            return basic_value< Traits, Base >();
         }

         template< template< typename... > class Traits, typename Base, typename K, typename... Ks >
         basic_value< Traits, Base > value( const basic_value< Traits, Base >& v, const K& key, const Ks&... ks )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return get::value( *p, ks... );
               }
            }
            return basic_value< Traits, Base >();
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base >
         T as( const basic_value< Traits, Base >& v )
         {
            return v.skip_value_ptr().template as< U >();
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base, typename K, typename... Ks >
         T as( const basic_value< Traits, Base >& v, const K& key, Ks&&... ks )
         {
            return get::as< T, U >( v.skip_value_ptr().at( key ), ks... );
         }

         template< typename T, template< typename... > class Traits, typename Base >
         void to( const basic_value< Traits, Base >& v, T& t )
         {
            v.skip_value_ptr().to( t );
         }

         template< typename T, template< typename... > class Traits, typename Base, typename K, typename... Ks >
         void to( const basic_value< Traits, Base >& v, T& t, const K& key, Ks&&... ks )
         {
            get::to( v.skip_value_ptr().at( key ), t, ks... );
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base  >
         tao::optional< T > optional( const basic_value< Traits, Base >& v )
         {
            if( v ) {
               return tao::optional< T >( tao::in_place, get::as< U >( v ) );
            }
            return tao::nullopt;
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base, typename K, typename... Ks >
         tao::optional< T > optional( const basic_value< Traits, Base >& v, const K& key, const Ks&... ks )
         {
            if( v ) {
               if( const auto* p = internal::find( v, key ) ) {
                  return get::optional< T, U >( *p, ks... );
               }
            }
            return tao::nullopt;
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base >
         T defaulted( const T& t, const basic_value< Traits, Base >& v )
         {
            if( v ) {
               return get::as< T, U >( v );
            }
            return t;
         }

         template< typename T, typename U = T, template< typename... > class Traits, typename Base, typename K, typename... Ks >
         T defaulted( const T& t, const basic_value< Traits, Base >& v, const K& key, const Ks&... ks )
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
