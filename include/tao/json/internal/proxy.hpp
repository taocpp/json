// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_PROXY_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_PROXY_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "const_proxy.hpp"

#include "../external/byte.hpp"
#include "../external/operators.hpp"
#include "../external/optional.hpp"
#include "../external/string_view.hpp"

#include "../byte_view.hpp"
#include "../data.hpp"
#include "../pair.hpp"
#include "../pointer.hpp"
#include "../type.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      class basic_custom_value;

      namespace internal
      {
         template< template< typename... > class Traits >
         class proxy
         {
         private:
            using binary_t = data::binary_t;
            using array_t = data::array_t;
            using object_t = data::object_t;

            data& v_;

         public:
            explicit proxy( data& v ) noexcept
               : v_( v )
            {
            }

            operator data&() noexcept
            {
               return v_;
            }

            operator const data&() const noexcept
            {
               return v_;
            }

            proxy& operator=( data v ) noexcept
            {
               v_ = v;
               return *this;
            }

            proxy& operator=( null_t ) noexcept
            {
               v_ = null;
               return *this;
            }

            proxy& operator=( empty_binary_t ) noexcept
            {
               v_ = empty_binary;
               return *this;
            }

            proxy& operator=( empty_array_t ) noexcept
            {
               v_ = empty_array;
               return *this;
            }

            proxy& operator=( empty_object_t ) noexcept
            {
               v_ = empty_object;
               return *this;
            }

            proxy& operator=( basic_custom_value< Traits > v ) noexcept
            {
               v_ = std::move( v );
               return *this;
            }

            void swap( data& r ) noexcept
            {
               v_.swap( r );
            }

            json::type type() const noexcept
            {
               return v_.type();
            }

            explicit operator bool() const noexcept
            {
               return v_.empty();
            }

            bool is_null() const noexcept
            {
               return v_.is_null();
            }

            bool is_boolean() const noexcept
            {
               return v_.is_boolean();
            }

            bool is_signed() const noexcept
            {
               return v_.is_signed();
            }

            bool is_unsigned() const noexcept
            {
               return v_.is_unsigned();
            }

            bool is_integer() const noexcept
            {
               return is_integer();
            }

            bool is_double() const noexcept
            {
               return v_.is_double();
            }

            bool is_number() const noexcept
            {
               return v_.is_number();
            }

            bool is_string() const noexcept
            {
               return v_.is_string();
            }

            bool is_string_view() const noexcept
            {
               return v_.is_string_view();
            }

            bool is_string_type() const noexcept
            {
               return v_.is_string_type();
            }

            bool is_binary() const noexcept
            {
               return v_.is_binary();
            }

            bool is_binary_view() const noexcept
            {
               return v_.is_binary_view();
            }

            bool is_binary_type() const noexcept
            {
               return v_.is_binary_type();
            }

            bool is_array() const noexcept
            {
               return v_.is_array();
            }

            bool is_object() const noexcept
            {
               return v_.is_object();
            }

            bool is_raw_ptr() const noexcept
            {
               return v_.is_raw_ptr();
            }

            bool unsafe_get_boolean() const noexcept
            {
               return v_.unsafe_get_boolean();
            }

            std::int64_t unsafe_get_signed() const noexcept
            {
               return v_.unsafe_get_signed();
            }

            std::uint64_t unsafe_get_unsigned() const noexcept
            {
               return v_.unsafe_get_unsigned();
            }

            double unsafe_get_double() const noexcept
            {
               return v_.unsafe_get_double();
            }

            std::string& unsafe_get_string() noexcept
            {
               return v_.unsafe_get_string();
            }

            const std::string& unsafe_get_string() const noexcept
            {
               return v_.unsafe_get_string();
            }

            tao::string_view unsafe_get_string_view() const noexcept
            {
               return v_.unsafe_get_string_view();
            }

            binary_t& unsafe_get_binary() noexcept
            {
               return v_.unsafe_get_binary();
            }

            const binary_t& unsafe_get_binary() const noexcept
            {
               return v_.unsafe_get_binary();
            }

            tao::byte_view unsafe_get_binary_view() const noexcept
            {
               return v_.unsafe_get_binary_view();
            }

            array_t& unsafe_get_array() noexcept
            {
               return v_.unsafe_get_array();
            }

            const array_t& unsafe_get_array() const noexcept
            {
               return v_.unsafe_get_array();
            }

            object_t& unsafe_get_object() noexcept
            {
               return v_.unsafe_get_object();
            }

            const object_t& unsafe_get_object() const noexcept
            {
               return v_.unsafe_get_object();
            }

            const data* unsafe_get_raw_ptr() const noexcept
            {
               return v_.unsafe_get_raw_ptr();
            }

            template< json::type E >
            decltype( v_.unsafe_get< E >() ) unsafe_get() noexcept
            {
               return v_.unsafe_get< E >();
            }

            template< json::type E >
            decltype( v_.unsafe_get< E >() ) unsafe_get() const noexcept
            {
               return v_.unsafe_get< E >();
            }

            bool get_boolean() const
            {
               return v_.get_boolean();
            }

            std::int64_t get_signed() const
            {
               return v_.get_signed();
            }

            std::uint64_t get_unsigned() const
            {
               return v_.get_unsigned();
            }

            double get_double() const
            {
               return v_.get_double();
            }

            std::string& get_string()
            {
               return v_.get_string();
            }

            const std::string& get_string() const
            {
               return v_.get_string();
            }

            tao::string_view get_string_view() const
            {
               return v_.get_string_view();
            }

            binary_t& get_binary()
            {
               return v_.get_binary();
            }

            const binary_t& get_binary() const
            {
               return v_.get_binary();
            }

            tao::byte_view get_binary_view() const
            {
               return v_.get_binary_view();
            }

            array_t& get_array()
            {
               return v_.get_array();
            }

            const array_t& get_array() const
            {
               return v_.get_array();
            }

            object_t& get_object()
            {
               return v_.get_object();
            }

            const object_t& get_object() const
            {
               return v_.get_object();
            }

            const data* get_raw_ptr() const
            {
               return v_.get_raw_ptr();
            }

            template< json::type E >
            decltype( v_.get< E >() ) get() noexcept
            {
               return v_.get< E >();
            }

            template< json::type E >
            decltype( v_.get< E >() ) get() const noexcept
            {
               return v_.get< E >();
            }

            void unsafe_assign_null() noexcept
            {
               v_.unsafe_assign_null();
            }

            void unsafe_assign_bool( const bool b ) noexcept
            {
               v_.unsafe_assign_bool( b );
            }

            void unsafe_assign_signed( const std::int64_t i ) noexcept
            {
               v_.unsafe_assign_signed( i );
            }

            void unsafe_assign_unsigned( const std::uint64_t u ) noexcept
            {
               v_.unsafe_assign_unsigned( u );
            }

            void unsafe_assign_double( const double d ) noexcept
            {
               v_.unsafe_assign_double( d );
            }

            void unsafe_assign_string( const std::string& s )
            {
               v_.unsafe_assign_string( s );
            }

            void unsafe_assign_string( std::string&& s ) noexcept
            {
               v_.unsafe_assign_string( std::move( s ) );
            }

            template< typename... Ts >
            void unsafe_emplace_string( Ts&&... ts ) noexcept( noexcept( v_.unsafe_emplace_string( std::forward< Ts >( ts )... ) ) )
            {
               v_.unsafe_emplace_string( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_string_view( const tao::string_view sv ) noexcept
            {
               v_.unsafe_assign_string_view( sv );
            }

            void unsafe_assign_binary( const binary_t& x )
            {
               v_.unsafe_assign_binary( x );
            }

            void unsafe_assign_binary( binary_t&& x ) noexcept
            {
               v_.unsafe_assign_binary( std::move( x ) );
            }

            template< typename... Ts >
            void unsafe_emplace_binary( Ts&&... ts ) noexcept( noexcept( v_.unsafe_emplace_binary( std::forward< Ts >( ts )... ) ) )
            {
               v_.unsafe_emplace_binary( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_binary_view( const tao::byte_view xv ) noexcept
            {
               v_.unsafe_assign_binary_view( xv );
            }

            void unsafe_assign_array( const array_t& a )
            {
               v_.unsafe_assign_array( a );
            }

            void unsafe_assign_array( array_t&& a ) noexcept
            {
               v_.unsafe_assign_array( std::move( a ) );
            }

            template< typename... Ts >
            void unsafe_emplace_array( Ts&&... ts ) noexcept( noexcept( v_.unsafe_emplace_array( std::forward< Ts >( ts )... ) ) )
            {
               v_.unsafe_emplace_array( std::forward< Ts >( ts )... );
            }

            template< typename V >
            void unsafe_emplace_back( V&& v )
            {
               v_.unsafe_emplace_back( std::forward< V >( v ) );
            }

            void unsafe_assign_object( const object_t& o )
            {
               v_.unsafe_assign_object( o );
            }

            void unsafe_assign_object( object_t&& o ) noexcept
            {
               v_.unsafe_assign_object( std::move( o ) );
            }

            template< typename... Ts >
            void unsafe_emplace_object( Ts&&... ts ) noexcept( noexcept( v_.unsafe_emplace_object( std::forward< Ts >( ts )... ) ) )
            {
               v_.unsafe_emplace_object( std::forward< Ts >( ts )... );
            }

            template< typename K, typename V >
            std::pair< typename object_t::iterator, bool > unsafe_emplace( K&& k, V&& v )
            {
               return v_.unsafe_emplace( std::forward< K >( k ), std::forward< V >( v ) );
            }

            void unsafe_assign_raw_ptr( const data* p ) noexcept
            {
               v_.unsafe_assign_raw_ptr( p );
            }

            void assign_null() noexcept
            {
               v_.assign_null();
            }

            void assign_bool( const bool b ) noexcept
            {
               v_.assign_bool( b );
            }

            void assign_signed( const std::int64_t i ) noexcept
            {
               v_.assign_signed( i );
            }

            void assign_unsigned( const std::uint64_t u ) noexcept
            {
               v_.assign_unsigned( u );
            }

            void assign_double( const double d ) noexcept
            {
               v_.assign_double( d );
            }

            void assign_string( const std::string& s )
            {
               v_.assign_string( s );
            }

            void assign_string( std::string&& s ) noexcept
            {
               v_.assign_string( std::move( s ) );
            }

            template< typename... Ts >
            void emplace_string( Ts&&... ts ) noexcept( noexcept( v_.emplace_string( std::forward< Ts >( ts )... ) ) )
            {
               v_.emplace_string( std::forward< Ts >( ts )... );
            }

            void assign_string_view( const tao::string_view sv ) noexcept
            {
               v_.assign_string_view( sv );
            }

            void assign_binary( const binary_t& v )
            {
               v_.assign_binary( v );
            }

            void assign_binary( binary_t&& v ) noexcept
            {
               v_.assign_binary( std::move( v ) );
            }

            template< typename... Ts >
            void emplace_binary( Ts&&... ts ) noexcept( noexcept( v_.emplace_binary( std::forward< Ts >( ts )... ) ) )
            {
               v_.emplace_binary( std::forward< Ts >( ts )... );
            }

            void assign_binary_view( const tao::byte_view xv ) noexcept
            {
               v_.assign_binary_view( xv );
            }

            void assign_array( const array_t& v )
            {
               v_.assign_array( v );
            }

            void assign_array( array_t&& v ) noexcept
            {
               v_.assign_array( std::move( v ) );
            }

            template< typename... Ts >
            void emplace_array( Ts&&... ts ) noexcept( noexcept( v_.emplace_array( std::forward< Ts >( ts )... ) ) )
            {
               v_.emplace_array( std::forward< Ts >( ts )... );
            }

            void prepare_array()
            {
               v_.prepare_array();
            }

            template< typename V >
            void emplace_back( V&& v )
            {
               v_.emplace_back( std::forward< V >( v ) );
            }

            void assign_object( const object_t& o )
            {
               v_.assign_object( o );
            }

            void assign_object( object_t&& o ) noexcept
            {
               v_.assign_object( std::move( o ) );
            }

            template< typename... Ts >
            void emplace_object( Ts&&... ts ) noexcept( noexcept( v_.emplace_object( std::forward< Ts >( ts )... ) ) )
            {
               v_.emplace_object( std::forward< Ts >( ts )... );
            }

            void prepare_object()
            {
               v_.prepare_object();
            }

            template< typename K, typename V >
            std::pair< typename object_t::iterator, bool > emplace( K&& k, V&& v )
            {
               return v_.emplace( std::forward< K >( k ), std::forward< V >( v ) );
            }

            void assign_raw_ptr( const data* p ) noexcept
            {
               v_.assign_raw_ptr( p );
            }

            const data* skip_raw_ptr() const noexcept
            {
               return v_.skip_raw_ptr();
            }

            data* unsafe_find( const std::string& key ) noexcept
            {
               return v_.unsafe_find( key );
            }

            template< typename T >
            data* unsafe_find( const T& key ) noexcept
            {
               return v_.unsafe_find( key );
            }

            const data* unsafe_find( const std::string& key ) const noexcept
            {
               return v_.unsafe_find( key );
            }

            template< typename T >
            const data* unsafe_find( const T& key ) const noexcept
            {
               return v_.unsafe_find( key );
            }

            data* find( const std::string& key )
            {
               return v_.find( key );
            }

            template< typename T >
            data* find( const T& key )
            {
               return v_.find( key );
            }

            const data* find( const std::string& key ) const
            {
               return v_.find( key );
            }

            template< typename T >
            const data* find( const T& key ) const
            {
               return v_.find( key );
            }

            proxy unsafe_at( const std::size_t index ) noexcept
            {
               return proxy( v_.unsafe_at( index ) );
            }

            const_proxy< Traits > unsafe_at( const std::size_t index ) const noexcept
            {
               return const_proxy< Traits >( v_.unsafe_at( index ) );
            }

            proxy unsafe_at( const std::string& key ) noexcept
            {
               return proxy( v_.unsafe_at( key ) );
            }

            const_proxy< Traits > unsafe_at( const std::string& key ) const noexcept
            {
               return const_proxy< Traits >( v_.unsafe_at( key ) );
            }

            proxy at( const std::size_t index )
            {
               return proxy( v_.at( index ) );
            }

            const_proxy< Traits > at( const std::size_t index ) const
            {
               return const_proxy< Traits >( v_.at( index ) );
            }

            proxy at( const std::string& key )
            {
               return proxy( v_.at( key ) );
            }

            const_proxy< Traits > at( const std::string& key ) const
            {
               return const_proxy< Traits >( v_.at( key ) );
            }

            proxy at( const pointer& k )
            {
               return proxy( v_.at( k ) );
            }

            const_proxy< Traits > at( const pointer& k ) const
            {
               return const_proxy< Traits >( v_.at( k ) );
            }

            proxy operator[]( const std::size_t index ) noexcept
            {
               return proxy( v_[ index ] );
            }

            const_proxy< Traits > operator[]( const std::size_t index ) const noexcept
            {
               return const_proxy< Traits >( v_[ index ] );
            }

            proxy operator[]( const std::string& key )
            {
               return proxy( v_[ key ] );
            }

            proxy operator[]( std::string&& key )
            {
               return proxy( v_[ std::move( key ) ] );
            }

            proxy operator[]( const pointer& k )
            {
               return proxy( v_[ k ] );
            }

            void erase( const std::size_t index )
            {
               v_.erase( index );
            }

            void erase( const std::string& key )
            {
               v_.erase( key );
            }

            void erase( const pointer& k )
            {
               v_.erase( k );
            }

            proxy insert( const pointer& k, data value )
            {
               return proxy( v_.insert( k, std::move( value ) ) );
            }

            bool empty() const noexcept
            {
               return v_.empty();
            }

            void unsafe_discard() noexcept
            {
               v_.unsafe_discard();
            }

            void discard() noexcept
            {
               v_.discard();
            }

            void reset() noexcept
            {
               v_.reset();
            }

            template< typename T >
            void extract( T& v ) const
            {
               Traits< typename std::decay< T >::type >::extract( *this, v );
            }

            template< typename T >
            typename std::enable_if< internal::has_as< Traits< T >, proxy >::value, T >::type as() const
            {
               return Traits< T >::as( *this );
            }

            template< typename T >
            typename std::enable_if< !internal::has_as< Traits< T >, proxy >::value, T >::type as() const
            {
               T nrv;
               this->extract( nrv );
               return nrv;
            }

            template< typename T >
            tao::optional< T > optional() const
            {
               if( is_null() ) {
                  return tao::nullopt;
               }
               else {
                  return as< T >();
               }
            }

            template< typename T, typename K >
            tao::optional< T > optional( const K& key ) const
            {
               if( const auto* p = find( key ) ) {
                  return tao::nullopt;
               }
               else {
                  return const_proxy< Traits >( *p ).template as< T >();
               }
            }

            template< typename T >
            tao::optional< T > optional( const std::string& key ) const
            {
               return optional< T, std::string >( key );
            }
         };

         template< template< typename... > class TraitsL, template< typename... > class TraitsR >
         bool operator==( proxy< TraitsL > lhs, proxy< TraitsR > rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == static_cast< const data& >( rhs );
         }

         template< template< typename... > class TraitsL, template< typename... > class TraitsR >
         bool operator<( proxy< TraitsL > lhs, proxy< TraitsR > rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < static_cast< const data& >( rhs );
         }

         template< template< typename... > class TraitsL, typename R >
         bool operator==( proxy< TraitsL > lhs, R&& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == std::forward< R >( rhs );
         }

         template< template< typename... > class TraitsL, typename R >
         bool operator<( proxy< TraitsL > lhs, R&& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < std::forward< R >( rhs );
         }

         template< typename L, template< typename... > class TraitsR >
         bool operator==( L&& lhs, proxy< TraitsR > rhs ) noexcept
         {
            return std::forward< L >( lhs ) == static_cast< const data& >( rhs );
         }

         template< typename L, template< typename... > class TraitsR >
         bool operator<( L&& lhs, proxy< TraitsR > rhs ) noexcept
         {
            return std::forward< L >( lhs ) < static_cast< const data& >( rhs );
         }

         template< template< typename... > class Traits >
         proxy< Traits >& operator+=( proxy< Traits >& v, std::initializer_list< pair< Traits > >&& l )
         {
            v.insert( std::move( l ) );
            return v;
         }

         template< template< typename... > class Traits >
         proxy< Traits >& operator+=( proxy< Traits >& v, const std::initializer_list< pair< Traits > >& l )
         {
            v.insert( l );
            return v;
         }

         template< template< typename... > class Traits >
         proxy< Traits >& operator-=( proxy< Traits >& v, std::initializer_list< std::string > l )
         {
            auto& o = v.get_object();
            for( const auto& k : l ) {
               if( o.erase( k ) == 0 ) {
                  throw std::runtime_error( "JSON object key not found: " + k );
               }
            }
            return v;
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
