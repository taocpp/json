// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "external/byte.hpp"
#include "external/operators.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

#include "internal/const_proxy.hpp"
#include "internal/proxy.hpp"

#include "byte_view.hpp"
#include "data.hpp"
#include "pair.hpp"
#include "pointer.hpp"
#include "single.hpp"
#include "traits.hpp"
#include "type.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      class basic_custom_value
         : public data
      {
      private:
         using proxy_t = internal::proxy< Traits >;
         using const_proxy_t = internal::const_proxy< Traits >;

      public:
         using data::data;

         basic_custom_value() noexcept = default;

         basic_custom_value( const basic_custom_value& ) = default;
         basic_custom_value( basic_custom_value&& ) noexcept = default;

         basic_custom_value( basic_custom_value& v )
            : basic_custom_value( static_cast< const basic_custom_value& >( v ) )
         {
         }

         basic_custom_value( const basic_custom_value&& v )
            : basic_custom_value( static_cast< const basic_custom_value& >( v ) )
         {
         }

         basic_custom_value( const data& v )
            : data( v )
         {
         }

         basic_custom_value( data&& v ) noexcept
            : data( std::move( v ) )
         {
         }

         basic_custom_value( proxy_t v )
            : data( static_cast< const data& >( v ) )
         {
         }

         basic_custom_value( const_proxy_t v )
            : data( static_cast< const data& >( v ) )
         {
         }

         template< typename T >
         basic_custom_value( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_custom_value& >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         basic_custom_value( std::initializer_list< pair< Traits > >&& l )
         {
            unsafe_assign( std::move( l ) );
         }

         basic_custom_value( const std::initializer_list< pair< Traits > >& l )
         {
            unsafe_assign( l );
         }

         basic_custom_value( std::initializer_list< pair< Traits > >& l )
            : basic_custom_value( static_cast< const std::initializer_list< pair< Traits > >& >( l ) )
         {
         }

         static basic_custom_value array( std::initializer_list< single< Traits > >&& l )
         {
            basic_custom_value v;
            v.append( std::move( l ) );
            return v;
         }

         static basic_custom_value array( const std::initializer_list< single< Traits > >& l )
         {
            basic_custom_value v;
            v.append( l );
            return v;
         }

         using data::operator=;

         basic_custom_value& operator=( basic_custom_value v ) noexcept
         {
            data::operator=( static_cast< data&& >( v ) );
            return *this;
         }

         basic_custom_value& operator=( proxy_t v )
         {
            data::operator=( static_cast< const data& >( v ) );
            return *this;
         }

         basic_custom_value& operator=( const_proxy_t v )
         {
            data::operator=( static_cast< const data& >( v ) );
            return *this;
         }

         proxy_t proxy() noexcept
         {
            return proxy_t( *this );
         }

         const_proxy_t proxy() const noexcept
         {
            return const_proxy_t( *this );
         }

         template< typename T >
         void extract( T& v ) const
         {
            proxy().extract( v );
         }

         template< typename T >
         T as() const
         {
            return proxy().template as< T >();
         }

         template< typename T >
         tao::optional< T > optional() const
         {
            if( this->is_null() ) {
               return tao::nullopt;
            }
            else {
               return this->as< T >();
            }
         }

         data& insert( const pointer& k, basic_custom_value value )
         {
            return data::insert( k, std::move( value ) );
         }

         template< typename T, typename K >
         tao::optional< T > optional( const K& key ) const
         {
            return const_proxy_t( *this ).template optional< T >( key );
         }

         template< typename T >
         tao::optional< T > optional( const std::string& key ) const
         {
            return optional< T, std::string >( key );
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_discard( v.type() ) is true!

         template< typename T >
         void unsafe_assign( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_custom_value& >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( std::initializer_list< pair< Traits > >&& l )
         {
            unsafe_emplace_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void unsafe_assign( const std::initializer_list< pair< Traits > >& l )
         {
            unsafe_emplace_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void unsafe_assign( std::initializer_list< pair< Traits > >& l )
         {
            unsafe_assign( static_cast< const std::initializer_list< pair< Traits > >& >( l ) );
         }

         template< typename V >
         void unsafe_emplace_back( V&& v )
         {
            data::unsafe_emplace_back( basic_custom_value( std::forward< V >( v ) ) );
         }

         template< typename K, typename V >
         std::pair< typename object_t::iterator, bool > unsafe_emplace( K&& k, V&& v )
         {
            return data::unsafe_emplace( std::forward< K >( k ), basic_custom_value( std::forward< V >( v ) ) );
         }

         template< typename V >
         void emplace_back( V&& v )
         {
            prepare_array();
            unsafe_emplace_back( std::forward< V >( v ) );
         }

         template< typename K, typename V >
         std::pair< typename object_t::iterator, bool > emplace( K&& k, V&& v )
         {
            prepare_object();
            return unsafe_emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         void append( std::initializer_list< single< Traits > >&& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( auto& e : l ) {
               unsafe_emplace_back( std::move( e.value ) );
            }
         }

         void append( const std::initializer_list< single< Traits > >& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( const auto& e : l ) {
               unsafe_emplace_back( e.value );
            }
         }

         void insert( std::initializer_list< pair< Traits > >&& l )
         {
            prepare_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void insert( const std::initializer_list< pair< Traits > >& l )
         {
            prepare_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         proxy_t unsafe_at( const std::size_t index ) noexcept
         {
            return proxy_t( data::unsafe_at( index ) );
         }

         const_proxy_t unsafe_at( const std::size_t index ) const noexcept
         {
            return const_proxy_t( data::unsafe_at( index ) );
         }

         proxy_t unsafe_at( const std::string& key ) noexcept
         {
            return proxy_t( data::unsafe_at( key ) );
         }

         const_proxy_t unsafe_at( const std::string& key ) const noexcept
         {
            return const_proxy_t( data::unsafe_at( key ) );
         }

         proxy_t at( const std::size_t index )
         {
            return proxy_t( data::at( index ) );
         }

         const_proxy_t at( const std::size_t index ) const
         {
            return const_proxy_t( data::at( index ) );
         }

         proxy_t at( const std::string& key )
         {
            return proxy_t( data::at( key ) );
         }

         const_proxy_t at( const std::string& key ) const
         {
            return const_proxy_t( data::at( key ) );
         }

         proxy_t at( const pointer& k )
         {
            return proxy_t( data::at( k ) );
         }

         const_proxy_t at( const pointer& k ) const
         {
            return const_proxy_t( data::at( k ) );
         }

         proxy_t operator[]( const std::size_t index ) noexcept
         {
            return proxy_t( data::operator[]( index ) );
         }

         const_proxy_t operator[]( const std::size_t index ) const noexcept
         {
            return const_proxy_t( data::operator[]( index ) );
         }

         proxy_t operator[]( const std::string& key )
         {
            return proxy_t( data::operator[]( key ) );
         }

         proxy_t operator[]( std::string&& key )
         {
            return proxy_t( data::operator[]( std::move( key ) ) );
         }

         proxy_t operator[]( const pointer& k )
         {
            return proxy_t( data::operator[]( k ) );
         }
      };

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator+=( basic_custom_value< Traits >& v, std::initializer_list< pair< Traits > >&& l )
      {
         v.insert( std::move( l ) );
         return v;
      }

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator+=( basic_custom_value< Traits >& v, const std::initializer_list< pair< Traits > >& l )
      {
         v.insert( l );
         return v;
      }

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator-=( basic_custom_value< Traits >& v, std::initializer_list< std::string > l )
      {
         auto& o = v.get_object();
         for( const auto& k : l ) {
            if( o.erase( k ) == 0 ) {
               throw std::runtime_error( "JSON object key not found: " + k );
            }
         }
         return v;
      }

      using custom_value = basic_custom_value< traits >;

   }  // namespace json

}  // namespace tao

#endif
