// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_PROXY_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_PROXY_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "../external/byte.hpp"
#include "../external/optional.hpp"
#include "../external/string_view.hpp"

#include "../byte_view.hpp"
#include "../data.hpp"
#include "../pair.hpp"
#include "../pointer.hpp"
#include "../single.hpp"
#include "../stream.hpp"
#include "../type.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename, typename, typename = void >
         struct has_as : std::false_type
         {
         };

         template< typename T, typename V >
         struct has_as< T, V, decltype( T::as( std::declval< const V& >() ), void() ) > : std::true_type
         {
         };

         template< typename B, template< typename... > class Traits >
         class proxy;

         class data_holder
         {
         protected:
            data v_;

         public:
            data_holder() = default;

            data_holder( const data& v )
               : v_( v )
            {
            }

            data_holder( data&& v )
               : v_( std::move( v ) )
            {
            }

            data_holder( null_t ) noexcept
               : v_( null )
            {
            }

            data_holder( empty_binary_t ) noexcept
               : v_( empty_binary )
            {
            }

            data_holder( empty_array_t ) noexcept
               : v_( empty_array )
            {
            }

            data_holder( empty_object_t ) noexcept
               : v_( empty_object )
            {
            }
         };

         struct data_reference
         {
         protected:
            data& v_;

         public:
            data_reference( data& v )
               : v_( v )
            {
            }
         };

         struct const_data_reference
         {
         protected:
            const data& v_;

         public:
            const_data_reference( const data& v )
               : v_( v )
            {
            }
         };

         template< typename B, template< typename... > class Traits >
         class proxy : B
         {
         private:
            using binary_t = data::binary_t;
            using array_t = data::array_t;
            using object_t = data::object_t;

         public:
            using B::B;

            proxy() = default;

            template< typename T, typename = typename std::enable_if< !std::is_convertible< T&&, const data& >::value >::type >
            proxy( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< proxy& >(), std::forward< T >( v ) ) ) )
            {
               using D = typename std::decay< T >::type;
               Traits< D >::assign( *this, std::forward< T >( v ) );
            }

            proxy( std::initializer_list< pair< Traits > >&& l )
            {
               unsafe_assign( std::move( l ) );
            }

            proxy( const std::initializer_list< pair< Traits > >& l )
            {
               unsafe_assign( l );
            }

            proxy( std::initializer_list< pair< Traits > >& l )
               : proxy( static_cast< const std::initializer_list< pair< Traits > >& >( l ) )
            {
            }

            static proxy array( std::initializer_list< single< Traits > >&& l )
            {
               proxy v;
               v.append( std::move( l ) );
               return v;
            }

            static proxy array( const std::initializer_list< single< Traits > >& l )
            {
               proxy v;
               v.append( l );
               return v;
            }

            operator data&() noexcept
            {
               return this->v_;
            }

            operator const data&() const noexcept
            {
               return this->v_;
            }

            proxy& operator=( data&& v ) noexcept
            {
               this->v_ = std::move( v );
               return *this;
            }

            proxy& operator=( const data& v )
            {
               this->v_ = v;
               return *this;
            }

            proxy& operator=( null_t ) noexcept
            {
               this->v_ = null;
               return *this;
            }

            proxy& operator=( empty_binary_t ) noexcept
            {
               this->v_ = empty_binary;
               return *this;
            }

            proxy& operator=( empty_array_t ) noexcept
            {
               this->v_ = empty_array;
               return *this;
            }

            proxy& operator=( empty_object_t ) noexcept
            {
               this->v_ = empty_object;
               return *this;
            }

            template< typename T >
            proxy& operator=( T&& v ) noexcept( noexcept( proxy< data_holder, Traits >( std::forward< T >( v ) ) ) )
            {
               this->v_ = std::move( static_cast< data& >( proxy< data_holder, Traits >( std::forward< T >( v ) ) ) );
               return *this;
            }

            void swap( data& r ) noexcept
            {
               this->v_.swap( r );
            }

            json::type type() const noexcept
            {
               return this->v_.type();
            }

            explicit operator bool() const noexcept
            {
               return static_cast< bool >( this->v_ );
            }

            bool is_null() const noexcept
            {
               return this->v_.is_null();
            }

            bool is_boolean() const noexcept
            {
               return this->v_.is_boolean();
            }

            bool is_signed() const noexcept
            {
               return this->v_.is_signed();
            }

            bool is_unsigned() const noexcept
            {
               return this->v_.is_unsigned();
            }

            bool is_integer() const noexcept
            {
               return this->v_.is_integer();
            }

            bool is_double() const noexcept
            {
               return this->v_.is_double();
            }

            bool is_number() const noexcept
            {
               return this->v_.is_number();
            }

            bool is_string() const noexcept
            {
               return this->v_.is_string();
            }

            bool is_string_view() const noexcept
            {
               return this->v_.is_string_view();
            }

            bool is_string_type() const noexcept
            {
               return this->v_.is_string_type();
            }

            bool is_binary() const noexcept
            {
               return this->v_.is_binary();
            }

            bool is_binary_view() const noexcept
            {
               return this->v_.is_binary_view();
            }

            bool is_binary_type() const noexcept
            {
               return this->v_.is_binary_type();
            }

            bool is_array() const noexcept
            {
               return this->v_.is_array();
            }

            bool is_object() const noexcept
            {
               return this->v_.is_object();
            }

            bool is_raw_ptr() const noexcept
            {
               return this->v_.is_raw_ptr();
            }

            bool unsafe_get_boolean() const noexcept
            {
               return this->v_.unsafe_get_boolean();
            }

            std::int64_t unsafe_get_signed() const noexcept
            {
               return this->v_.unsafe_get_signed();
            }

            std::uint64_t unsafe_get_unsigned() const noexcept
            {
               return this->v_.unsafe_get_unsigned();
            }

            double unsafe_get_double() const noexcept
            {
               return this->v_.unsafe_get_double();
            }

            std::string& unsafe_get_string() noexcept
            {
               return this->v_.unsafe_get_string();
            }

            const std::string& unsafe_get_string() const noexcept
            {
               return this->v_.unsafe_get_string();
            }

            tao::string_view unsafe_get_string_view() const noexcept
            {
               return this->v_.unsafe_get_string_view();
            }

            binary_t& unsafe_get_binary() noexcept
            {
               return this->v_.unsafe_get_binary();
            }

            const binary_t& unsafe_get_binary() const noexcept
            {
               return this->v_.unsafe_get_binary();
            }

            tao::byte_view unsafe_get_binary_view() const noexcept
            {
               return this->v_.unsafe_get_binary_view();
            }

            array_t& unsafe_get_array() noexcept
            {
               return this->v_.unsafe_get_array();
            }

            const array_t& unsafe_get_array() const noexcept
            {
               return this->v_.unsafe_get_array();
            }

            object_t& unsafe_get_object() noexcept
            {
               return this->v_.unsafe_get_object();
            }

            const object_t& unsafe_get_object() const noexcept
            {
               return this->v_.unsafe_get_object();
            }

            const data* unsafe_get_raw_ptr() const noexcept
            {
               return this->v_.unsafe_get_raw_ptr();
            }

            template< json::type E >
            decltype( std::declval< const data& >().template unsafe_get< E >() ) unsafe_get() noexcept
            {
               return this->v_.template unsafe_get< E >();
            }

            template< json::type E >
            decltype( std::declval< const data& >().template unsafe_get< E >() ) unsafe_get() const noexcept
            {
               return this->v_.template unsafe_get< E >();
            }

            bool get_boolean() const
            {
               return this->v_.get_boolean();
            }

            std::int64_t get_signed() const
            {
               return this->v_.get_signed();
            }

            std::uint64_t get_unsigned() const
            {
               return this->v_.get_unsigned();
            }

            double get_double() const
            {
               return this->v_.get_double();
            }

            std::string& get_string()
            {
               return this->v_.get_string();
            }

            const std::string& get_string() const
            {
               return this->v_.get_string();
            }

            tao::string_view get_string_view() const
            {
               return this->v_.get_string_view();
            }

            binary_t& get_binary()
            {
               return this->v_.get_binary();
            }

            const binary_t& get_binary() const
            {
               return this->v_.get_binary();
            }

            tao::byte_view get_binary_view() const
            {
               return this->v_.get_binary_view();
            }

            array_t& get_array()
            {
               return this->v_.get_array();
            }

            const array_t& get_array() const
            {
               return this->v_.get_array();
            }

            object_t& get_object()
            {
               return this->v_.get_object();
            }

            const object_t& get_object() const
            {
               return this->v_.get_object();
            }

            const data* get_raw_ptr() const
            {
               return this->v_.get_raw_ptr();
            }

            template< json::type E >
            decltype( std::declval< const data& >().template get< E >() ) get() noexcept
            {
               return this->v_.template get< E >();
            }

            template< json::type E >
            decltype( std::declval< const data& >().template get< E >() ) get() const noexcept
            {
               return this->v_.template get< E >();
            }

            void unsafe_assign_null() noexcept
            {
               this->v_.unsafe_assign_null();
            }

            void unsafe_assign_bool( const bool b ) noexcept
            {
               this->v_.unsafe_assign_bool( b );
            }

            void unsafe_assign_signed( const std::int64_t i ) noexcept
            {
               this->v_.unsafe_assign_signed( i );
            }

            void unsafe_assign_unsigned( const std::uint64_t u ) noexcept
            {
               this->v_.unsafe_assign_unsigned( u );
            }

            void unsafe_assign_double( const double d ) noexcept
            {
               this->v_.unsafe_assign_double( d );
            }

            template< typename... Ts >
            void unsafe_emplace_string( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().unsafe_emplace_string( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.unsafe_emplace_string( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_string( const std::string& s )
            {
               this->v_.unsafe_assign_string( s );
            }

            void unsafe_assign_string( std::string&& s ) noexcept
            {
               this->v_.unsafe_assign_string( std::move( s ) );
            }

            void unsafe_assign_string_view( const tao::string_view sv ) noexcept
            {
               this->v_.unsafe_assign_string_view( sv );
            }

            template< typename... Ts >
            void unsafe_emplace_binary( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().unsafe_emplace_binary( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.unsafe_emplace_binary( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_binary( const binary_t& x )
            {
               this->v_.unsafe_assign_binary( x );
            }

            void unsafe_assign_binary( binary_t&& x ) noexcept
            {
               this->v_.unsafe_assign_binary( std::move( x ) );
            }

            void unsafe_assign_binary_view( const tao::byte_view xv ) noexcept
            {
               this->v_.unsafe_assign_binary_view( xv );
            }

            template< typename... Ts >
            void unsafe_emplace_array( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().unsafe_emplace_array( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.unsafe_emplace_array( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_array( const array_t& a )
            {
               this->v_.unsafe_assign_array( a );
            }

            void unsafe_assign_array( array_t&& a ) noexcept
            {
               this->v_.unsafe_assign_array( std::move( a ) );
            }

            void unsafe_push_back( proxy< data_holder, Traits > v )
            {
               this->v_.unsafe_emplace_back( std::move( v ) );
            }

            void unsafe_emplace_back( proxy< data_holder, Traits > v )
            {
               this->v_.unsafe_emplace_back( std::move( v ) );
            }

            template< typename... Ts >
            void unsafe_emplace_object( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().unsafe_emplace_object( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.unsafe_emplace_object( std::forward< Ts >( ts )... );
            }

            void unsafe_assign_object( const object_t& o )
            {
               this->v_.unsafe_assign_object( o );
            }

            void unsafe_assign_object( object_t&& o ) noexcept
            {
               this->v_.unsafe_assign_object( std::move( o ) );
            }

            std::pair< typename object_t::iterator, bool > unsafe_emplace( const std::string& k, const proxy< data_holder, Traits >& v )
            {
               return this->v_.unsafe_emplace( k, v );
            }

            std::pair< typename object_t::iterator, bool > unsafe_emplace( const std::string& k, proxy< data_holder, Traits >&& v )
            {
               return this->v_.unsafe_emplace( k, std::move( v ) );
            }

            std::pair< typename object_t::iterator, bool > unsafe_emplace( std::string&& k, const proxy< data_holder, Traits >& v )
            {
               return this->v_.unsafe_emplace( std::move( k ), v );
            }

            std::pair< typename object_t::iterator, bool > unsafe_emplace( std::string&& k, proxy< data_holder, Traits >&& v )
            {
               return this->v_.unsafe_emplace( std::move( k ), std::move( v ) );
            }

            void unsafe_assign_raw_ptr( const data* p ) noexcept
            {
               this->v_.unsafe_assign_raw_ptr( p );
            }

            template< typename T >
            void unsafe_assign( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< proxy& >(), std::forward< T >( v ) ) ) )
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

            void assign_null() noexcept
            {
               this->v_.assign_null();
            }

            void assign_bool( const bool b ) noexcept
            {
               this->v_.assign_bool( b );
            }

            void assign_signed( const std::int64_t i ) noexcept
            {
               this->v_.assign_signed( i );
            }

            void assign_unsigned( const std::uint64_t u ) noexcept
            {
               this->v_.assign_unsigned( u );
            }

            void assign_double( const double d ) noexcept
            {
               this->v_.assign_double( d );
            }

            template< typename... Ts >
            void emplace_string( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().emplace_string( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.emplace_string( std::forward< Ts >( ts )... );
            }

            void assign_string( const std::string& s )
            {
               this->v_.assign_string( s );
            }

            void assign_string( std::string&& s ) noexcept
            {
               this->v_.assign_string( std::move( s ) );
            }

            void assign_string_view( const tao::string_view sv ) noexcept
            {
               this->v_.assign_string_view( sv );
            }

            template< typename... Ts >
            void emplace_binary( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().emplace_binary( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.emplace_binary( std::forward< Ts >( ts )... );
            }

            void assign_binary( const binary_t& v )
            {
               this->v_.assign_binary( v );
            }

            void assign_binary( binary_t&& v ) noexcept
            {
               this->v_.assign_binary( std::move( v ) );
            }

            void assign_binary_view( const tao::byte_view xv ) noexcept
            {
               this->v_.assign_binary_view( xv );
            }

            template< typename... Ts >
            void emplace_array( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().emplace_array( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.emplace_array( std::forward< Ts >( ts )... );
            }

            void assign_array( const array_t& v )
            {
               this->v_.assign_array( v );
            }

            void assign_array( array_t&& v ) noexcept
            {
               this->v_.assign_array( std::move( v ) );
            }

            void prepare_array()
            {
               this->v_.prepare_array();
            }

            void push_back( proxy< data_holder, Traits > v )
            {
               prepare_array();
               unsafe_push_back( std::move( v ) );
            }

            void emplace_back( proxy< data_holder, Traits > v )
            {
               prepare_array();
               unsafe_emplace_back( std::move( v ) );
            }

            void append( std::initializer_list< single< Traits > >&& l )
            {
               prepare_array();
               auto& v = unsafe_get_array();
               v.reserve( v.size() + l.size() );
               for( auto& e : l ) {
                  this->v_.unsafe_push_back( std::move( e.value ) );
               }
            }

            void append( const std::initializer_list< single< Traits > >& l )
            {
               prepare_array();
               auto& v = unsafe_get_array();
               v.reserve( v.size() + l.size() );
               for( const auto& e : l ) {
                  this->v_.unsafe_push_back( e.value );
               }
            }

            template< typename... Ts >
            void emplace_object( Ts&&... ts ) noexcept( noexcept( std::declval< data& >().emplace_object( std::forward< Ts >( ts )... ) ) )
            {
               this->v_.emplace_object( std::forward< Ts >( ts )... );
            }

            void assign_object( const object_t& o )
            {
               this->v_.assign_object( o );
            }

            void assign_object( object_t&& o ) noexcept
            {
               this->v_.assign_object( std::move( o ) );
            }

            void prepare_object()
            {
               this->v_.prepare_object();
            }

            std::pair< typename object_t::iterator, bool > emplace( const std::string& k, const proxy< data_holder, Traits >& v )
            {
               prepare_object();
               return unsafe_emplace( k, v );
            }

            std::pair< typename object_t::iterator, bool > emplace( const std::string& k, proxy< data_holder, Traits >&& v )
            {
               prepare_object();
               return unsafe_emplace( k, std::move( v ) );
            }

            std::pair< typename object_t::iterator, bool > emplace( std::string&& k, const proxy< data_holder, Traits >& v )
            {
               prepare_object();
               return unsafe_emplace( std::move( k ), v );
            }

            std::pair< typename object_t::iterator, bool > emplace( std::string&& k, proxy< data_holder, Traits >&& v )
            {
               prepare_object();
               return unsafe_emplace( std::move( k ), std::move( v ) );
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

            void assign_raw_ptr( const data* p ) noexcept
            {
               this->v_.assign_raw_ptr( p );
            }

            const data* skip_raw_ptr() const noexcept
            {
               return this->v_.skip_raw_ptr();
            }

            data* unsafe_find( const std::string& key ) noexcept
            {
               return this->v_.unsafe_find( key );
            }

            template< typename T >
            data* unsafe_find( const T& key ) noexcept
            {
               return this->v_.unsafe_find( key );
            }

            const data* unsafe_find( const std::string& key ) const noexcept
            {
               return this->v_.unsafe_find( key );
            }

            template< typename T >
            const data* unsafe_find( const T& key ) const noexcept
            {
               return this->v_.unsafe_find( key );
            }

            data* find( const std::string& key )
            {
               return this->v_.find( key );
            }

            template< typename T >
            data* find( const T& key )
            {
               return this->v_.find( key );
            }

            const data* find( const std::string& key ) const
            {
               return this->v_.find( key );
            }

            template< typename T >
            const data* find( const T& key ) const
            {
               return this->v_.find( key );
            }

            proxy< data_reference, Traits > unsafe_at( const std::size_t index ) noexcept
            {
               return this->v_.unsafe_at( index );
            }

            const proxy< const_data_reference, Traits > unsafe_at( const std::size_t index ) const noexcept
            {
               return this->v_.unsafe_at( index );
            }

            proxy< data_reference, Traits > unsafe_at( const std::string& key ) noexcept
            {
               return this->v_.unsafe_at( key );
            }

            const proxy< const_data_reference, Traits > unsafe_at( const std::string& key ) const noexcept
            {
               return this->v_.unsafe_at( key );
            }

            proxy< data_reference, Traits > at( const std::size_t index )
            {
               return this->v_.at( index );
            }

            const proxy< const_data_reference, Traits > at( const std::size_t index ) const
            {
               return this->v_.at( index );
            }

            proxy< data_reference, Traits > at( const std::string& key )
            {
               return this->v_.at( key );
            }

            const proxy< const_data_reference, Traits > at( const std::string& key ) const
            {
               return this->v_.at( key );
            }

            proxy< data_reference, Traits > at( const pointer& k )
            {
               return this->v_.at( k );
            }

            const proxy< const_data_reference, Traits > at( const pointer& k ) const
            {
               return this->v_.at( k );
            }

            proxy< data_reference, Traits > operator[]( const std::size_t index ) noexcept
            {
               return this->v_[ index ];
            }

            const proxy< const_data_reference, Traits > operator[]( const std::size_t index ) const noexcept
            {
               return this->v_[ index ];
            }

            proxy< data_reference, Traits > operator[]( const std::string& key )
            {
               return this->v_[ key ];
            }

            proxy< data_reference, Traits > operator[]( std::string&& key )
            {
               return this->v_[ std::move( key ) ];
            }

            proxy< data_reference, Traits > operator[]( const pointer& k )
            {
               return this->v_[ k ];
            }

            void erase( const std::size_t index )
            {
               this->v_.erase( index );
            }

            void erase( const std::string& key )
            {
               this->v_.erase( key );
            }

            void erase( const pointer& k )
            {
               this->v_.erase( k );
            }

            proxy< data_reference, Traits > insert( const pointer& k, data value )
            {
               return this->v_.insert( k, std::move( value ) );
            }

            bool empty() const noexcept
            {
               return this->v_.empty();
            }

            void unsafe_discard() noexcept
            {
               this->v_.unsafe_discard();
            }

            void discard() noexcept
            {
               this->v_.discard();
            }

            void reset() noexcept
            {
               this->v_.reset();
            }

            template< typename T >
            void extract( T& v ) const
            {
               Traits< typename std::decay< T >::type >::extract( *this, v );
            }

            template< typename T >
            typename std::enable_if< has_as< Traits< T >, proxy >::value, T >::type as() const
            {
               return Traits< T >::as( *this );
            }

            template< typename T >
            typename std::enable_if< !has_as< Traits< T >, proxy >::value, T >::type as() const
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
                  return proxy< const_data_reference, Traits >( *p ).template as< T >();
               }
            }

            template< typename T >
            tao::optional< T > optional( const std::string& key ) const
            {
               return optional< T, std::string >( key );
            }
         };

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator==( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator!=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) != static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) > static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) <= static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) >= static_cast< const data& >( rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator==( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator!=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs != *rhs ) : ( lhs != nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs <= *rhs ) : ( lhs <= nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>=( const proxy< BL, TraitsL >& lhs, const proxy< BR, TraitsR >* rhs ) noexcept
         {
            return rhs ? ( lhs >= *rhs ) : ( lhs >= nullptr );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator==( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs == rhs ) : ( nullptr == rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator!=( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs != rhs ) : ( nullptr != rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs < rhs ) : ( nullptr < rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs > rhs ) : ( nullptr > rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator<=( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs <= rhs ) : ( nullptr <= rhs );
         }

         template< typename BL, template< typename... > class TraitsL, typename BR, template< typename... > class TraitsR >
         bool operator>=( const proxy< BL, TraitsL >* lhs, const proxy< BR, TraitsR >& rhs ) noexcept
         {
            return lhs ? ( *lhs >= rhs ) : ( nullptr >= rhs );
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator==( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == rhs;
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator!=( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) != rhs;
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator<( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < rhs;
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator>( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) > rhs;
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator<=( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) <= rhs;
         }

         template< typename B, template< typename... > class Traits, typename T >
         bool operator>=( const proxy< B, Traits >& lhs, const T& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) >= rhs;
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator==( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs == static_cast< const data& >( rhs );
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator!=( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs != static_cast< const data& >( rhs );
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator<( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs < static_cast< const data& >( rhs );
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator>( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs > static_cast< const data& >( rhs );
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator<=( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs <= static_cast< const data& >( rhs );
         }

         template< typename T, typename B, template< typename... > class Traits >
         bool operator>=( const T& lhs, const proxy< B, Traits >& rhs ) noexcept
         {
            return lhs >= static_cast< const data& >( rhs );
         }

         template< typename B, template< typename... > class Traits >
         std::ostream& operator<<( std::ostream& o, const proxy< B, Traits >& v )
         {
            return o << static_cast< const data& >( v );
         }

         template< typename B, template< typename... > class Traits >
         proxy< B, Traits >& operator+=( proxy< B, Traits >& v, std::initializer_list< pair< Traits > >&& l )
         {
            v.insert( std::move( l ) );
            return v;
         }

         template< typename B, template< typename... > class Traits >
         proxy< B, Traits >& operator+=( proxy< B, Traits >& v, const std::initializer_list< pair< Traits > >& l )
         {
            v.insert( l );
            return v;
         }

         template< typename B, template< typename... > class Traits >
         proxy< B, Traits >& operator-=( proxy< B, Traits >& v, std::initializer_list< std::string > l )
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
