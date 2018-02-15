// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BASIC_VALUE_HPP
#define TAO_JSON_BASIC_VALUE_HPP

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "events/virtual_base.hpp"

#include "external/byte.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

#include "internal/get_by_enum.hpp"
#include "internal/identity.hpp"
#include "internal/pair.hpp"
#include "internal/single.hpp"
#include "internal/type_traits.hpp"
#include "internal/value_union.hpp"

#include "byte_view.hpp"
#include "pointer.hpp"
#include "type.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // required work-around for GCC 6
         inline void rethrow()
         {
            throw;
         }

      }  // namespace internal

      template< template< typename... > class Traits, typename Base >
      class basic_value  // NOLINT
         : private Base
      {
      public:
         static_assert( std::is_nothrow_default_constructible< Base >::value, "Base must be nothrow default constructible" );
         static_assert( std::is_nothrow_move_constructible< Base >::value, "Base must be nothrow move constructible" );
         static_assert( std::is_nothrow_move_assignable< Base >::value, "Base must be nothrow move assignable" );

         using binary_t = std::vector< tao::byte >;
         using array_t = std::vector< basic_value >;
         using object_t = std::map< std::string, basic_value >;

         basic_value() noexcept = default;

         basic_value( const basic_value& r )
            : Base( static_cast< const Base& >( r ) ),
              m_type( json::type::DESTROYED )
         {
            embed( r );
            m_type = r.m_type;
         }

         basic_value( basic_value&& r ) noexcept
            : Base( static_cast< Base&& >( r ) ),
              m_type( r.m_type )
         {
            seize( std::move( r ) );
         }

         template< typename T, typename = typename std::enable_if< !std::is_convertible< T&&, const basic_value& >::value >::type >
         basic_value( T&& v )  // NOLINT
            noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_value& >(), std::forward< T >( v ) ) ) )
         {
            try {
               using D = typename std::decay< T >::type;
               Traits< D >::assign( *this, std::forward< T >( v ) );
            }
            catch( ... ) {
               unsafe_discard();
#ifndef NDEBUG
               static_cast< volatile json::type& >( m_type ) = json::type::DESTROYED;
#endif
               internal::rethrow();
            }
         }

         basic_value( std::initializer_list< internal::pair< Traits, Base > >&& l )
         {
            try {
               unsafe_assign( std::move( l ) );
            }
            catch( ... ) {
               unsafe_discard();
#ifndef NDEBUG
               static_cast< volatile json::type& >( m_type ) = json::type::DESTROYED;
#endif
               throw;
            }
         }

         basic_value( const std::initializer_list< internal::pair< Traits, Base > >& l )
         {
            try {
               unsafe_assign( l );
            }
            catch( ... ) {
               unsafe_discard();
#ifndef NDEBUG
               static_cast< volatile json::type& >( m_type ) = json::type::DESTROYED;
#endif
               throw;
            }
         }

         basic_value( std::initializer_list< internal::pair< Traits, Base > >& l )
            : basic_value( static_cast< const std::initializer_list< internal::pair< Traits, Base > >& >( l ) )
         {
         }

         ~basic_value() noexcept
         {
            unsafe_discard();
#ifndef NDEBUG
            static_cast< volatile json::type& >( m_type ) = json::type::DESTROYED;
#endif
         }

         static basic_value array( std::initializer_list< internal::single< Traits, Base > >&& l )
         {
            basic_value v;
            v.append( std::move( l ) );
            return v;
         }

         static basic_value array( const std::initializer_list< internal::single< Traits, Base > >& l )
         {
            basic_value v;
            v.append( l );
            return v;
         }

         basic_value& operator=( basic_value v ) noexcept
         {
            unsafe_discard();
            m_type = v.m_type;
            seize( std::move( v ) );
            Base::operator=( static_cast< Base&& >( v ) );
            return *this;
         }

         void swap( basic_value& r ) noexcept
         {
            basic_value t( std::move( r ) );
            r = std::move( *this );
            ( *this ) = ( std::move( t ) );
         }

         Base& base() noexcept
         {
            return static_cast< Base& >( *this );
         }

         const Base& base() const noexcept
         {
            return static_cast< const Base& >( *this );
         }

         json::type type() const noexcept
         {
            return m_type;
         }

         explicit operator bool() const noexcept
         {
            assert( m_type != json::type::DISCARDED );
            assert( m_type != json::type::DESTROYED );
            return m_type != json::type::UNINITIALIZED;
         }

         bool is_null() const noexcept
         {
            return m_type == json::type::NULL_;
         }

         bool is_boolean() const noexcept
         {
            return m_type == json::type::BOOLEAN;
         }

         bool is_signed() const noexcept
         {
            return m_type == json::type::SIGNED;
         }

         bool is_unsigned() const noexcept
         {
            return m_type == json::type::UNSIGNED;
         }

         bool is_integer() const noexcept
         {
            return is_signed() || is_unsigned();
         }

         bool is_double() const noexcept
         {
            return m_type == json::type::DOUBLE;
         }

         bool is_number() const noexcept
         {
            return is_integer() || is_double();
         }

         bool is_string() const noexcept
         {
            return m_type == json::type::STRING;
         }

         bool is_string_view() const noexcept
         {
            return m_type == json::type::STRING_VIEW;
         }

         bool is_string_type() const noexcept
         {
            return is_string() || is_string_view();
         }

         bool is_binary() const noexcept
         {
            return m_type == json::type::BINARY;
         }

         bool is_binary_view() const noexcept
         {
            return m_type == json::type::BINARY_VIEW;
         }

         bool is_binary_type() const noexcept
         {
            return is_binary() || is_binary_view();
         }

         bool is_array() const noexcept
         {
            return m_type == json::type::ARRAY;
         }

         bool is_object() const noexcept
         {
            return m_type == json::type::OBJECT;
         }

         bool is_raw_ptr() const noexcept
         {
            return m_type == json::type::RAW_PTR;
         }

         bool is_opaque_ptr() const noexcept
         {
            return m_type == json::type::OPAQUE_PTR;
         }

         // The unsafe_get_*() accessor functions MUST NOT be
         // called when the type of the value is not the one
         // corresponding to the type of the accessor!

         bool unsafe_get_boolean() const noexcept
         {
            return m_union.b;
         }

         std::int64_t unsafe_get_signed() const noexcept
         {
            return m_union.i;
         }

         std::uint64_t unsafe_get_unsigned() const noexcept
         {
            return m_union.u;
         }

         double unsafe_get_double() const noexcept
         {
            return m_union.d;
         }

         std::string& unsafe_get_string() noexcept
         {
            return m_union.s;
         }

         const std::string& unsafe_get_string() const noexcept
         {
            return m_union.s;
         }

         tao::string_view unsafe_get_string_view() const noexcept
         {
            return m_union.sv;
         }

         tao::string_view unsafe_get_string_type() const noexcept
         {
            return ( m_type == json::type::STRING ) ? m_union.s : m_union.sv;
         }

         binary_t& unsafe_get_binary() noexcept
         {
            return m_union.x;
         }

         const binary_t& unsafe_get_binary() const noexcept
         {
            return m_union.x;
         }

         tao::byte_view unsafe_get_binary_view() const noexcept
         {
            return m_union.xv;
         }

         array_t& unsafe_get_array() noexcept
         {
            return m_union.a;
         }

         const array_t& unsafe_get_array() const noexcept
         {
            return m_union.a;
         }

         object_t& unsafe_get_object() noexcept
         {
            return m_union.o;
         }

         const object_t& unsafe_get_object() const noexcept
         {
            return m_union.o;
         }

         const basic_value* unsafe_get_raw_ptr() const noexcept
         {
            return m_union.p;
         }

         const internal::opaque_ptr_t unsafe_get_opaque_ptr() const noexcept
         {
            return m_union.q;
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< basic_value >& >() ) ) unsafe_get() noexcept
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< basic_value >& >() ) ) unsafe_get() const noexcept
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         bool get_boolean() const
         {
            validate_json_type( json::type::BOOLEAN );
            return unsafe_get_boolean();
         }

         std::int64_t get_signed() const
         {
            validate_json_type( json::type::SIGNED );
            return unsafe_get_signed();
         }

         std::uint64_t get_unsigned() const
         {
            validate_json_type( json::type::UNSIGNED );
            return unsafe_get_unsigned();
         }

         double get_double() const
         {
            validate_json_type( json::type::DOUBLE );
            return unsafe_get_double();
         }

         std::string& get_string()
         {
            validate_json_type( json::type::STRING );
            return unsafe_get_string();
         }

         const std::string& get_string() const
         {
            validate_json_type( json::type::STRING );
            return unsafe_get_string();
         }

         tao::string_view get_string_view() const
         {
            validate_json_type( json::type::STRING_VIEW );
            return unsafe_get_string_view();
         }

         tao::string_view get_string_type() const noexcept
         {
            return ( m_type == json::type::STRING ) ? m_union.s : unsafe_get_string_view();
         }

         binary_t& get_binary()
         {
            validate_json_type( json::type::BINARY );
            return unsafe_get_binary();
         }

         const binary_t& get_binary() const
         {
            validate_json_type( json::type::BINARY );
            return unsafe_get_binary();
         }

         tao::byte_view get_binary_view() const
         {
            validate_json_type( json::type::BINARY_VIEW );
            return unsafe_get_binary_view();
         }

         array_t& get_array()
         {
            validate_json_type( json::type::ARRAY );
            return unsafe_get_array();
         }

         const array_t& get_array() const
         {
            validate_json_type( json::type::ARRAY );
            return unsafe_get_array();
         }

         object_t& get_object()
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_get_object();
         }

         const object_t& get_object() const
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_get_object();
         }

         const basic_value* get_raw_ptr() const
         {
            validate_json_type( json::type::RAW_PTR );
            return unsafe_get_raw_ptr();
         }

         // Does get_opaque_ptr() make sense?

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< basic_value >& >() ) ) get()
         {
            validate_json_type( E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< basic_value >& >() ) ) get() const
         {
            validate_json_type( E );
            return internal::get_by_enum< E >::get( m_union );
         }

         void unsafe_assign_null() noexcept
         {
            m_type = json::type::NULL_;
         }

         void unsafe_assign_bool( const bool b ) noexcept
         {
            m_union.b = b;
            m_type = json::type::BOOLEAN;
         }

         void unsafe_assign_signed( const std::int64_t i ) noexcept
         {
            m_union.i = i;
            m_type = json::type::SIGNED;
         }

         void unsafe_assign_unsigned( const std::uint64_t u ) noexcept
         {
            m_union.u = u;
            m_type = json::type::UNSIGNED;
         }

         void unsafe_assign_double( const double d ) noexcept
         {
            m_union.d = d;
            m_type = json::type::DOUBLE;
         }

         template< typename... Ts >
         void unsafe_emplace_string( Ts&&... ts ) noexcept( noexcept( std::string( std::forward< Ts >( ts )... ) ) )
         {
            new( &m_union.s ) std::string( std::forward< Ts >( ts )... );
            m_type = json::type::STRING;
         }

         void unsafe_assign_string( const std::string& s )
         {
            unsafe_emplace_string( s );
         }

         void unsafe_assign_string( std::string&& s ) noexcept
         {
            unsafe_emplace_string( std::move( s ) );
         }

         void unsafe_assign_string_view( const tao::string_view sv ) noexcept
         {
            m_union.sv = sv;
            m_type = json::type::STRING_VIEW;
         }

         template< typename... Ts >
         void unsafe_emplace_binary( Ts&&... ts ) noexcept( noexcept( binary_t( std::forward< Ts >( ts )... ) ) )
         {
            new( &m_union.x ) binary_t( std::forward< Ts >( ts )... );
            m_type = json::type::BINARY;
         }

         void unsafe_assign_binary( const binary_t& x )
         {
            unsafe_emplace_binary( x );
         }

         void unsafe_assign_binary( binary_t&& x ) noexcept
         {
            unsafe_emplace_binary( std::move( x ) );
         }

         void unsafe_assign_binary_view( const tao::byte_view xv ) noexcept
         {
            m_union.xv = xv;
            m_type = json::type::BINARY_VIEW;
         }

         template< typename... Ts >
         void unsafe_emplace_array( Ts&&... ts ) noexcept( noexcept( array_t( std::forward< Ts >( ts )... ) ) )
         {
            new( &m_union.a ) array_t( std::forward< Ts >( ts )... );
            m_type = json::type::ARRAY;
         }

         void unsafe_assign_array( const array_t& a )
         {
            unsafe_emplace_array( a );
         }

         void unsafe_assign_array( array_t&& a ) noexcept
         {
            unsafe_emplace_array( std::move( a ) );
         }

         void unsafe_push_back( const basic_value& v )
         {
            m_union.a.push_back( v );
         }

         void unsafe_push_back( basic_value&& v )
         {
            m_union.a.push_back( std::move( v ) );
         }

         template< typename... Ts >
         auto unsafe_emplace_back( Ts&&... ts ) -> decltype( std::declval< array_t >().emplace_back( std::forward< Ts >( ts )... ) )
         {
            return m_union.a.emplace_back( std::forward< Ts >( ts )... );
         }

         template< typename... Ts >
         void unsafe_emplace_object( Ts&&... ts ) noexcept( noexcept( object_t( std::forward< Ts >( ts )... ) ) )
         {
            new( &m_union.o ) object_t( std::forward< Ts >( ts )... );
            m_type = json::type::OBJECT;
         }

         void unsafe_assign_object( const object_t& o )
         {
            unsafe_emplace_object( o );
         }

         void unsafe_assign_object( object_t&& o ) noexcept
         {
            unsafe_emplace_object( std::move( o ) );
         }

         template< typename... Ts >
         std::pair< typename object_t::iterator, bool > unsafe_emplace( Ts&&... ts )
         {
            return m_union.o.emplace( std::forward< Ts >( ts )... );
         }

         void unsafe_assign_raw_ptr( const basic_value* p ) noexcept
         {
            assert( p );
            m_union.p = p;
            m_type = json::type::RAW_PTR;
         }

         template< typename T >
         void unsafe_assign_opaque_ptr( const T* data ) noexcept
         {
            unsafe_assign_opaque_ptr( data, &basic_value::producer_wrapper< T > );
         }

         template< typename T >
         void unsafe_assign_opaque_ptr( const T* data, const producer_t producer ) noexcept
         {
            assert( data );
            assert( producer );
            m_union.q.data = data;
            m_union.q.producer = producer;
            m_type = json::type::OPAQUE_PTR;
         }

         template< typename T >
         void unsafe_assign( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_value& >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( std::initializer_list< internal::pair< Traits, Base > >&& l )
         {
            unsafe_emplace_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );  // NOLINT
               }
            }
         }

         void unsafe_assign( const std::initializer_list< internal::pair< Traits, Base > >& l )
         {
            unsafe_emplace_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );  // NOLINT
               }
            }
         }

         void unsafe_assign( std::initializer_list< internal::pair< Traits, Base > >& l )
         {
            unsafe_assign( static_cast< const std::initializer_list< internal::pair< Traits, Base > >& >( l ) );
         }

         void assign_null() noexcept
         {
            unsafe_discard();
            unsafe_assign_null();
         }

         void assign_bool( const bool b ) noexcept
         {
            unsafe_discard();
            unsafe_assign_bool( b );
         }

         void assign_signed( const std::int64_t i ) noexcept
         {
            unsafe_discard();
            unsafe_assign_signed( i );
         }

         void assign_unsigned( const std::uint64_t u ) noexcept
         {
            unsafe_discard();
            unsafe_assign_unsigned( u );
         }

         void assign_double( const double d ) noexcept
         {
            unsafe_discard();
            unsafe_assign_double( d );
         }

         template< typename... Ts >
         void emplace_string( Ts&&... ts ) noexcept( noexcept( unsafe_emplace_string( std::forward< Ts >( ts )... ) ) )
         {
            discard();
            unsafe_emplace_string( std::forward< Ts >( ts )... );
         }

         void assign_string( const std::string& s )
         {
            discard();
            unsafe_assign_string( s );
         }

         void assign_string( std::string&& s ) noexcept
         {
            unsafe_discard();
            unsafe_assign_string( std::move( s ) );
         }

         void assign_string_view( const tao::string_view sv ) noexcept
         {
            unsafe_discard();
            unsafe_assign_string_view( sv );
         }

         template< typename... Ts >
         void emplace_binary( Ts&&... ts ) noexcept( noexcept( unsafe_emplace_binary( std::forward< Ts >( ts )... ) ) )
         {
            discard();
            unsafe_emplace_binary( std::forward< Ts >( ts )... );
         }

         void assign_binary( const binary_t& v )
         {
            discard();
            unsafe_assign_binary( v );
         }

         void assign_binary( binary_t&& v ) noexcept
         {
            unsafe_discard();
            unsafe_assign_binary( std::move( v ) );
         }

         void assign_binary_view( const tao::byte_view xv ) noexcept
         {
            unsafe_discard();
            unsafe_assign_binary_view( xv );
         }

         template< typename... Ts >
         void emplace_array( Ts&&... ts ) noexcept( noexcept( unsafe_emplace_array( std::forward< Ts >( ts )... ) ) )
         {
            discard();
            unsafe_emplace_array( std::forward< Ts >( ts )... );
         }

         void assign_array( const array_t& v )
         {
            discard();
            unsafe_assign_array( v );
         }

         void assign_array( array_t&& v ) noexcept
         {
            unsafe_discard();
            unsafe_assign_array( std::move( v ) );
         }

         void prepare_array()
         {
            switch( m_type ) {
               case json::type::UNINITIALIZED:
               case json::type::DISCARDED:
                  unsafe_emplace_array();
               case json::type::ARRAY:
                  break;
               default:
                  throw_invalid_json_type();
            }
         }

         void push_back( const basic_value& v )
         {
            prepare_array();
            unsafe_push_back( v );
         }

         void push_back( basic_value&& v )
         {
            prepare_array();
            unsafe_push_back( std::move( v ) );
         }

         template< typename... Ts >
         void emplace_back( Ts&&... ts )
         {
            prepare_array();
            unsafe_emplace_back( std::forward< Ts >( ts )... );
         }

         void append( std::initializer_list< internal::single< Traits, Base > >&& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( auto& e : l ) {
               unsafe_push_back( std::move( e.value ) );
            }
         }

         void append( const std::initializer_list< internal::single< Traits, Base > >& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( const auto& e : l ) {
               unsafe_push_back( e.value );
            }
         }

         template< typename... Ts >
         void emplace_object( Ts&&... ts ) noexcept( noexcept( unsafe_emplace_object( std::forward< Ts >( ts )... ) ) )
         {
            discard();
            unsafe_emplace_object( std::forward< Ts >( ts )... );
         }

         void assign_object( const object_t& o )
         {
            discard();
            unsafe_assign_object( o );
         }

         void assign_object( object_t&& o ) noexcept
         {
            unsafe_discard();
            unsafe_assign_object( std::move( o ) );
         }

         void prepare_object()
         {
            switch( m_type ) {
               case json::type::UNINITIALIZED:
               case json::type::DISCARDED:
                  unsafe_emplace_object();
               case json::type::OBJECT:
                  break;
               default:
                  throw_invalid_json_type();
            }
         }

         template< typename... Ts >
         std::pair< typename object_t::iterator, bool > emplace( Ts&&... ts )
         {
            prepare_object();
            return unsafe_emplace( std::forward< Ts >( ts )... );
         }

         void insert( std::initializer_list< internal::pair< Traits, Base > >&& l )
         {
            prepare_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );  // NOLINT
               }
            }
         }

         void insert( const std::initializer_list< internal::pair< Traits, Base > >& l )
         {
            prepare_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );  // NOLINT
               }
            }
         }

         void assign_raw_ptr( const basic_value* p ) noexcept
         {
            unsafe_discard();
            unsafe_assign_raw_ptr( p );
         }

         template< typename T >
         void assign_opaque_ptr( const T* data ) noexcept
         {
            assign_opaque_ptr( data, &basic_value::producer_wrapper< T > );
         }

         template< typename T >
         void assign_opaque_ptr( const T* data, const producer_t producer ) noexcept
         {
            unsafe_discard();
            unsafe_assign_opaque_ptr( data, producer );
         }

         const basic_value* skip_raw_ptr() const noexcept
         {
            const basic_value* p = this;
            while( p->is_raw_ptr() ) {
               p = p->unsafe_get_raw_ptr();
               assert( p );
            }
            return p;
         }

         basic_value* unsafe_find( const std::string& key ) noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         template< typename T >
         basic_value* unsafe_find( const T& key ) noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         const basic_value* unsafe_find( const std::string& key ) const noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         template< typename T >
         const basic_value* unsafe_find( const T& key ) const noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         basic_value* find( const std::string& key )
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_find( key );
         }

         template< typename T >
         basic_value* find( const T& key )
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_find( key );
         }

         const basic_value* find( const std::string& key ) const
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_find( key );
         }

         template< typename T >
         const basic_value* find( const T& key ) const
         {
            validate_json_type( json::type::OBJECT );
            return unsafe_find( key );
         }

         basic_value& unsafe_at( const std::size_t index ) noexcept
         {
            return m_union.a[ index ];
         }

         const basic_value& unsafe_at( const std::size_t index ) const noexcept
         {
            return m_union.a[ index ];
         }

         basic_value& unsafe_at( const std::string& key ) noexcept
         {
            return m_union.o.find( key )->second;
         }

         const basic_value& unsafe_at( const std::string& key ) const noexcept
         {
            return m_union.o.find( key )->second;
         }

         basic_value& at( const std::size_t index )
         {
            validate_json_type( json::type::ARRAY );
            return m_union.a.at( index );
         }

         const basic_value& at( const std::size_t index ) const
         {
            validate_json_type( json::type::ARRAY );
            return m_union.a.at( index );
         }

         basic_value& at( const std::string& key )
         {
            validate_json_type( json::type::OBJECT );
            return m_union.o.at( key );
         }

         const basic_value& at( const std::string& key ) const
         {
            validate_json_type( json::type::OBJECT );
            return m_union.o.at( key );
         }

         basic_value& at( const pointer& k )
         {
            return internal::pointer_at( this, k.begin(), k.end() );
         }

         const basic_value& at( const pointer& k ) const
         {
            return internal::pointer_at( this, k.begin(), k.end() );
         }

         basic_value& operator[]( const std::size_t index ) noexcept
         {
            assert( m_type == json::type::ARRAY );
            return m_union.a[ index ];
         }

         const basic_value& operator[]( const std::size_t index ) const noexcept
         {
            assert( m_type == json::type::ARRAY );
            return m_union.a[ index ];
         }

         basic_value& operator[]( const std::string& key )
         {
            prepare_object();
            return m_union.o[ key ];
         }

         basic_value& operator[]( std::string&& key )
         {
            prepare_object();
            return m_union.o[ std::move( key ) ];
         }

         basic_value& operator[]( const pointer& k )
         {
            if( !k ) {
               return *this;
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            basic_value& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY: {
                  if( e->key() == "-" ) {
                     basic_value d;
                     d.unsafe_assign_null();
                     v.unsafe_emplace_back( std::move( d ) );
                     return v.m_union.a.back();
                  }
                  return v.at( e->index() );
               } break;
               case json::type::OBJECT: {
                  const auto& key = e->key();
                  const auto it = v.m_union.o.find( key );
                  if( it == v.m_union.o.end() ) {
                     basic_value d;
                     d.unsafe_assign_null();
                     const auto r = v.unsafe_emplace( key, std::move( d ) );
                     assert( r.second );
                     return r.first->second;
                  }
                  return it->second;
               } break;
               default:
                  throw internal::invalid_type( b, std::next( e ) );  // NOLINT
            }
         }

         template< typename T >
         typename std::enable_if< internal::has_as_one< Traits< T >, basic_value >::value, T >::type as() const
         {
            return Traits< T >::as( *this );
         }

         template< typename T >
         typename std::enable_if< !internal::has_as_one< Traits< T >, basic_value >::value, T >::type as() const
         {
            T nrv;
            this->as( nrv );
            return nrv;
         }

         template< typename T >
         typename std::enable_if< internal::has_as_two< Traits< T >, basic_value, T >::value, void >::type as( T& v ) const
         {
            Traits< typename std::decay< T >::type >::as( *this, v );
         }

         template< typename T >
         typename std::enable_if< !internal::has_as_two< Traits< T >, basic_value, T >::value, void >::type as( T& v ) const
         {
            v = Traits< typename std::decay< T >::type >::as( *this );
         }

         template< typename T >
         tao::optional< T > optional() const
         {
            if( is_null() ) {
               return tao::nullopt;
            }
            return as< T >();
         }

         template< typename T, typename K >
         tao::optional< T > optional( const K& key ) const
         {
            if( const auto* p = find( key ) ) {
               return p->template as< T >();
            }
            return tao::nullopt;
         }

         template< typename T >
         tao::optional< T > optional( const std::string& key ) const
         {
            return optional< T, std::string >( key );
         }

         void erase( const std::size_t index )
         {
            validate_json_type( json::type::ARRAY );
            auto& a = m_union.a;
            if( index >= a.size() ) {
               throw std::out_of_range( "JSON array index out of bounds" );  // NOLINT
            }
            a.erase( a.begin() + index );
         }

         void erase( const std::string& key )
         {
            validate_json_type( json::type::OBJECT );
            if( m_union.o.erase( key ) == 0 ) {
               throw std::out_of_range( "JSON object key not found: " + key );  // NOLINT
            }
         }

         void erase( const pointer& k )
         {
            if( !k ) {
               throw std::runtime_error( "invalid root JSON Pointer for erase" );  // NOLINT
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            basic_value& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY:
                  v.erase( e->index() );
                  break;
               case json::type::OBJECT:
                  v.erase( e->key() );
                  break;
               default:
                  throw internal::invalid_type( b, std::next( e ) );  // NOLINT
            }
         }

         basic_value& insert( const pointer& k, basic_value value )
         {
            if( !k ) {
               *this = std::move( value );
               return *this;
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            basic_value& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY: {
                  if( e->key() == "-" ) {
                     v.unsafe_emplace_back( std::move( value ) );
                     return v.m_union.a.back();
                  }
                  const auto i = e->index();
                  if( i >= v.m_union.a.size() ) {
                     throw std::out_of_range( "invalid JSON Pointer '" + internal::tokens_to_string( b, std::next( e ) ) + "' -- array index out of bounds" );  // NOLINT
                  }
                  v.m_union.a.insert( v.m_union.a.begin() + i, std::move( value ) );
                  return v.m_union.a.at( i );
               } break;
               case json::type::OBJECT: {
                  const auto& key = e->key();
                  const auto it = v.m_union.o.find( key );
                  if( it == v.m_union.o.end() ) {
                     const auto r = v.unsafe_emplace( key, std::move( value ) );
                     assert( r.second );
                     return r.first->second;
                  }
                  it->second = std::move( value );
                  return it->second;
               } break;
               default:
                  throw internal::invalid_type( b, std::next( e ) );  // NOLINT
            }
         }

         bool empty() const noexcept
         {
            switch( m_type ) {
               case json::type::UNINITIALIZED:
                  return true;

               case json::type::DISCARDED: {  // LCOV_EXCL_START
                  assert( m_type != json::type::DISCARDED );
                  return true;
                  // LCOV_EXCL_STOP
               }

               case json::type::DESTROYED: {  // LCOV_EXCL_START
                  assert( m_type != json::type::DESTROYED );
                  return true;
                  // LCOV_EXCL_STOP
               }

               case json::type::NULL_:
               case json::type::BOOLEAN:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
               case json::type::RAW_PTR:
               case json::type::OPAQUE_PTR:
                  return false;

               case json::type::STRING:
                  return m_union.s.empty();

               case json::type::STRING_VIEW:
                  return m_union.sv.empty();

               case json::type::BINARY:
                  return m_union.x.empty();

               case json::type::BINARY_VIEW:
                  return m_union.xv.empty();

               case json::type::ARRAY:
                  return m_union.a.empty();

               case json::type::OBJECT:
                  return m_union.o.empty();
            }
            // LCOV_EXCL_START
            assert( false );
            return false;
            // LCOV_EXCL_STOP
         }

         void unsafe_discard() noexcept
         {
            assert( m_type != json::type::DESTROYED );

            switch( m_type ) {
               case json::type::UNINITIALIZED:
               case json::type::DISCARDED:
               case json::type::DESTROYED:
               case json::type::NULL_:
               case json::type::BOOLEAN:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
               case json::type::RAW_PTR:
               case json::type::OPAQUE_PTR:
                  return;

               case json::type::STRING:
                  m_union.s.~basic_string();
                  return;

               case json::type::STRING_VIEW:
                  m_union.sv.~string_view();
                  return;

               case json::type::BINARY:
                  m_union.x.~vector();
                  return;

               case json::type::BINARY_VIEW:
                  m_union.xv.~byte_view();
                  return;

               case json::type::ARRAY:
                  m_union.a.~vector();
                  return;

               case json::type::OBJECT:
                  m_union.o.~map();
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void discard() noexcept
         {
            unsafe_discard();
            m_type = json::type::DISCARDED;
         }

         void reset() noexcept
         {
            unsafe_discard();
            m_type = json::type::UNINITIALIZED;
         }

         void throw_invalid_json_type() const
         {
            throw std::logic_error( std::string( "invalid json type '" ) + to_string( m_type ) + '\'' );  // NOLINT
         }

         void validate_json_type( const json::type t ) const
         {
            if( m_type != t ) {
               throw std::logic_error( std::string( "invalid json type '" ) + to_string( m_type ) + "', expected '" + to_string( t ) + '\'' );  // NOLINT
            }
         }

      private:
         void seize( basic_value&& r ) noexcept
         {
            assert( m_type != json::type::DESTROYED );

            switch( r.m_type ) {
               case json::type::UNINITIALIZED:
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::DISCARDED: {  // LCOV_EXCL_START
                  assert( r.m_type != json::type::DISCARDED );
                  return;
                  // LCOV_EXCL_STOP
               }

               case json::type::DESTROYED: {  // LCOV_EXCL_START
                  assert( r.m_type != json::type::DESTROYED );
                  return;
                  // LCOV_EXCL_STOP
               }

               case json::type::NULL_:
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::BOOLEAN:
                  m_union.b = r.m_union.b;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::SIGNED:
                  m_union.i = r.m_union.i;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::UNSIGNED:
                  m_union.u = r.m_union.u;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::DOUBLE:
                  m_union.d = r.m_union.d;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::STRING:
                  new( &m_union.s ) std::string( std::move( r.m_union.s ) );
#ifndef NDEBUG
                  r.discard();
#endif
                  return;

               case json::type::STRING_VIEW:
                  new( &m_union.sv ) tao::string_view( r.m_union.sv );
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::BINARY:
                  new( &m_union.x ) binary_t( std::move( r.m_union.x ) );
#ifndef NDEBUG
                  r.discard();
#endif
                  return;

               case json::type::BINARY_VIEW:
                  new( &m_union.xv ) tao::byte_view( r.m_union.xv );
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::ARRAY:
                  new( &m_union.a ) array_t( std::move( r.m_union.a ) );
#ifndef NDEBUG
                  r.discard();
#endif
                  return;

               case json::type::OBJECT:
                  new( &m_union.o ) object_t( std::move( r.m_union.o ) );
#ifndef NDEBUG
                  r.discard();
#endif
                  return;

               case json::type::RAW_PTR:
                  m_union.p = r.m_union.p;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;

               case json::type::OPAQUE_PTR:
                  m_union.q = r.m_union.q;
#ifndef NDEBUG
                  r.m_type = json::type::DISCARDED;
#endif
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void embed( const basic_value& r )
         {
            switch( r.m_type ) {
               case json::type::UNINITIALIZED:
                  return;

               case json::type::DISCARDED:
                  throw std::logic_error( "attempt to use a discarded value" );  // NOLINT

               case json::type::DESTROYED:
                  throw std::logic_error( "attempt to use a destroyed value" );  // NOLINT

               case json::type::NULL_:
                  return;

               case json::type::BOOLEAN:
                  m_union.b = r.m_union.b;
                  return;

               case json::type::SIGNED:
                  m_union.i = r.m_union.i;
                  return;

               case json::type::UNSIGNED:
                  m_union.u = r.m_union.u;
                  return;

               case json::type::DOUBLE:
                  m_union.d = r.m_union.d;
                  return;

               case json::type::STRING:
                  new( &m_union.s ) std::string( r.m_union.s );
                  return;

               case json::type::STRING_VIEW:
                  new( &m_union.sv ) tao::string_view( r.m_union.sv );
                  return;

               case json::type::BINARY:
                  new( &m_union.x ) binary_t( r.m_union.x );
                  return;

               case json::type::BINARY_VIEW:
                  new( &m_union.xv ) tao::byte_view( r.m_union.xv );
                  return;

               case json::type::ARRAY:
                  new( &m_union.a ) array_t( r.m_union.a );
                  return;

               case json::type::OBJECT:
                  new( &m_union.o ) object_t( r.m_union.o );
                  return;

               case json::type::RAW_PTR:
                  m_union.p = r.m_union.p;
                  return;

               case json::type::OPAQUE_PTR:
                  m_union.q = r.m_union.q;
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         template< typename T >
         static void producer_wrapper( events::virtual_base& consumer, const void* raw )
         {
            Traits< T >::template produce< Traits >( consumer, *static_cast< const T* >( raw ) );
         }

         json::type m_type = json::type::UNINITIALIZED;
         internal::value_union< basic_value > m_union;
      };

   }  // namespace json

}  // namespace tao

#endif
