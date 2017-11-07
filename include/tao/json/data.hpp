// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_DATA_HPP
#define TAOCPP_JSON_INCLUDE_DATA_HPP

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "external/byte.hpp"
#include "external/operators.hpp"
#include "external/string_view.hpp"

#include "internal/data_union.hpp"
#include "internal/get_by_enum.hpp"
#include "internal/throw.hpp"
#include "internal/totally_ordered.hpp"

#include "byte_view.hpp"
#include "pointer.hpp"
#include "type.hpp"

namespace tao
{
   namespace json
   {
      class data
         : operators::totally_ordered< data >,
           internal::totally_ordered< data, null_t, type::NULL_ >,
           internal::totally_ordered< data, bool, type::BOOLEAN >,
           internal::totally_ordered< data, signed char, type::SIGNED >,
           internal::totally_ordered< data, unsigned char, type::UNSIGNED >,
           internal::totally_ordered< data, signed short, type::SIGNED >,
           internal::totally_ordered< data, unsigned short, type::UNSIGNED >,
           internal::totally_ordered< data, signed int, type::SIGNED >,
           internal::totally_ordered< data, unsigned int, type::UNSIGNED >,
           internal::totally_ordered< data, signed long, type::SIGNED >,
           internal::totally_ordered< data, unsigned long, type::UNSIGNED >,
           internal::totally_ordered< data, signed long long, type::SIGNED >,
           internal::totally_ordered< data, unsigned long long, type::UNSIGNED >,
           internal::totally_ordered< data, float, type::DOUBLE >,
           internal::totally_ordered< data, double, type::DOUBLE >,
           internal::totally_ordered< data, std::string, type::STRING >,
           internal::totally_ordered< data, tao::string_view, type::STRING >,
           internal::totally_ordered< data, const char*, type::STRING >,
           internal::totally_ordered< data, std::vector< tao::byte >, type::BINARY >,
           internal::totally_ordered< data, tao::byte_view, type::BINARY_VIEW >,
           internal::totally_ordered< data, empty_binary_t, type::BINARY >,
           internal::totally_ordered< data, std::vector< data >, type::ARRAY >,
           internal::totally_ordered< data, empty_array_t, type::ARRAY >,
           internal::totally_ordered< data, std::map< std::string, data >, type::OBJECT >,
           internal::totally_ordered< data, empty_object_t, type::OBJECT >,
           internal::totally_ordered< data, const data*, type::RAW_PTR >,
           internal::totally_ordered< data, data*, type::RAW_PTR >,
           internal::totally_ordered< data, std::nullptr_t, type::RAW_PTR >
      {
      public:
         using binary_t = std::vector< tao::byte >;
         using array_t = std::vector< data >;
         using object_t = std::map< std::string, data >;

         data() noexcept = default;

         data( const data& r )
            : m_type( r.m_type )
         {
            embed( r );
         }

         data( data&& r ) noexcept
            : m_type( r.m_type )
         {
            seize( std::move( r ) );
         }

         data( null_t ) noexcept
         {
            unsafe_assign_null();
         }

         data( empty_binary_t ) noexcept
         {
            unsafe_emplace_binary();
         }

         data( empty_array_t ) noexcept
         {
            unsafe_emplace_array();
         }

         data( empty_object_t ) noexcept
         {
            unsafe_emplace_object();
         }

         ~data() noexcept
         {
            unsafe_discard();
            assert( ( const_cast< volatile json::type& >( m_type ) = json::type::DESTROYED, true ) );
         }

         data& operator=( data v ) noexcept
         {
            unsafe_discard();
            m_type = v.m_type;
            seize( std::move( v ) );
            return *this;
         }

         data& operator=( null_t ) noexcept
         {
            unsafe_discard();
            unsafe_assign_null();
            return *this;
         }

         data& operator=( empty_binary_t ) noexcept
         {
            unsafe_discard();
            unsafe_emplace_binary();
            return *this;
         }

         data& operator=( empty_array_t ) noexcept
         {
            unsafe_discard();
            unsafe_emplace_array();
            return *this;
         }

         data& operator=( empty_object_t ) noexcept
         {
            unsafe_discard();
            unsafe_emplace_object();
            return *this;
         }

         void swap( data& r ) noexcept
         {
            data t( std::move( r ) );
            r = std::move( *this );
            ( *this ) = ( std::move( t ) );
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

         const data* unsafe_get_raw_ptr() const noexcept
         {
            return m_union.p;
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::data_union& >() ) ) unsafe_get() noexcept
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::data_union& >() ) ) unsafe_get() const noexcept
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         bool get_boolean() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::BOOLEAN );
            return unsafe_get_boolean();
         }

         std::int64_t get_signed() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::SIGNED );
            return unsafe_get_signed();
         }

         std::uint64_t get_unsigned() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::UNSIGNED );
            return unsafe_get_unsigned();
         }

         double get_double() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::DOUBLE );
            return unsafe_get_double();
         }

         std::string& get_string()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         const std::string& get_string() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         tao::string_view get_string_view() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::STRING_VIEW );
            return unsafe_get_string_view();
         }

         binary_t& get_binary()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::BINARY );
            return unsafe_get_binary();
         }

         const binary_t& get_binary() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::BINARY );
            return unsafe_get_binary();
         }

         tao::byte_view get_binary_view() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::BINARY_VIEW );
            return unsafe_get_binary_view();
         }

         array_t& get_array()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         const array_t& get_array() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         object_t& get_object()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const object_t& get_object() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const data* get_raw_ptr() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::RAW_PTR );
            return unsafe_get_raw_ptr();
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::data_union& >() ) ) get()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::data_union& >() ) ) get() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, E );
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

         void unsafe_push_back( const data& v )
         {
            m_union.a.push_back( v );
         }

         void unsafe_push_back( data&& v )
         {
            m_union.a.push_back( std::move( v ) );
         }

         template< typename... Args >
         auto unsafe_emplace_back( Args&&... args ) -> decltype( std::declval< array_t >().emplace_back( std::forward< Args >( args )... ) )
         {
            return m_union.a.emplace_back( std::forward< Args >( args )... );
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

         template< typename... Args >
         std::pair< typename object_t::iterator, bool > unsafe_emplace( Args&&... args )
         {
            return m_union.o.emplace( std::forward< Args >( args )... );
         }

         void unsafe_assign_raw_ptr( const data* p ) noexcept
         {
            m_union.p = p;
            m_type = json::type::RAW_PTR;
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
                  TAOCPP_JSON_THROW_TYPE_ERROR( m_type );
            }
         }

         void push_back( const data& v )
         {
            prepare_array();
            unsafe_push_back( v );
         }

         void push_back( data&& v )
         {
            prepare_array();
            unsafe_push_back( std::move( v ) );
         }

         template< typename... Args >
         void emplace_back( Args&&... args )
         {
            prepare_array();
            unsafe_emplace_back( std::forward< Args... >( args )... );
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
                  TAOCPP_JSON_THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename... Args >
         std::pair< typename object_t::iterator, bool > emplace( Args&&... args )
         {
            prepare_object();
            return unsafe_emplace( std::forward< Args >( args )... );
         }

         void assign_raw_ptr( const data* p ) noexcept
         {
            unsafe_discard();
            unsafe_assign_raw_ptr( p );
         }

         const data* skip_raw_ptr() const noexcept
         {
            const data* p = this;
            while( p && p->is_raw_ptr() ) {
               p = p->unsafe_get_raw_ptr();
            }
            return p;
         }

         data* unsafe_find( const std::string& key ) noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         template< typename T >
         data* unsafe_find( const T& key ) noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         const data* unsafe_find( const std::string& key ) const noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         template< typename T >
         const data* unsafe_find( const T& key ) const noexcept
         {
            const auto it = m_union.o.find( key );
            return ( it != m_union.o.end() ) ? ( &it->second ) : nullptr;
         }

         data* find( const std::string& key )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_find( key );
         }

         template< typename T >
         data* find( const T& key )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_find( key );
         }

         const data* find( const std::string& key ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_find( key );
         }

         template< typename T >
         const data* find( const T& key ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_find( key );
         }

         data& unsafe_at( const std::size_t index ) noexcept
         {
            return m_union.a[ index ];
         }

         const data& unsafe_at( const std::size_t index ) const noexcept
         {
            return m_union.a[ index ];
         }

         data& unsafe_at( const std::string& key ) noexcept
         {
            return m_union.o.find( key )->second;
         }

         const data& unsafe_at( const std::string& key ) const noexcept
         {
            return m_union.o.find( key )->second;
         }

         data& at( const std::size_t index )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         const data& at( const std::size_t index ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         data& at( const std::string& key )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         const data& at( const std::string& key ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         data& at( const pointer& k )
         {
            return internal::pointer_at( this, k.begin(), k.end() );
         }

         const data& at( const pointer& k ) const
         {
            return internal::pointer_at( this, k.begin(), k.end() );
         }

         data& operator[]( const std::size_t index ) noexcept
         {
            assert( m_type == json::type::ARRAY );
            return m_union.a[ index ];
         }

         const data& operator[]( const std::size_t index ) const noexcept
         {
            assert( m_type == json::type::ARRAY );
            return m_union.a[ index ];
         }

         data& operator[]( const std::string& key )
         {
            prepare_object();
            return m_union.o[ key ];
         }

         data& operator[]( std::string&& key )
         {
            prepare_object();
            return m_union.o[ std::move( key ) ];
         }

         data& operator[]( const pointer& k )
         {
            if( !k ) {
               return *this;
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            data& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY: {
                  if( e->key() == "-" ) {
                     data d;
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
                     data d;
                     d.unsafe_assign_null();
                     const auto r = v.unsafe_emplace( key, std::move( d ) );
                     assert( r.second );
                     return r.first->second;
                  }
                  return it->second;
               } break;
               default:
                  throw internal::invalid_type( b, std::next( e ) );
            }
         }

         void erase( const std::size_t index )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            auto& a = m_union.a;
            if( index >= a.size() ) {
               throw std::out_of_range( "JSON array index out of bounds" );
            }
            a.erase( a.begin() + index );
         }

         void erase( const std::string& key )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            if( m_union.o.erase( key ) == 0 ) {
               throw std::out_of_range( "JSON object key not found: " + key );
            }
         }

         void erase( const pointer& k )
         {
            if( !k ) {
               throw std::runtime_error( "invalid root JSON Pointer for erase" );
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            data& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY:
                  v.erase( e->index() );
                  break;
               case json::type::OBJECT:
                  v.erase( e->key() );
                  break;
               default:
                  throw internal::invalid_type( b, std::next( e ) );
            }
         }

         data& insert( const pointer& k, data value )
         {
            if( !k ) {
               *this = std::move( value );
               return *this;
            }
            const auto b = k.begin();
            const auto e = std::prev( k.end() );
            data& v = internal::pointer_at( this, b, e );
            switch( v.m_type ) {
               case json::type::ARRAY: {
                  if( e->key() == "-" ) {
                     v.unsafe_emplace_back( std::move( value ) );
                     return v.m_union.a.back();
                  }
                  const auto i = e->index();
                  if( i >= v.m_union.a.size() ) {
                     throw std::out_of_range( "invalid JSON Pointer '" + internal::tokens_to_string( b, std::next( e ) ) + "' -- array index out of bounds" );
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
                  throw internal::invalid_type( b, std::next( e ) );
            }
         }

         bool empty() const noexcept
         {
            switch( m_type ) {
               case json::type::UNINITIALIZED:
                  return true;

               // LCOV_EXCL_START
               case json::type::DISCARDED:
                  assert( m_type != json::type::DISCARDED );
                  return true;

               case json::type::DESTROYED:
                  assert( m_type != json::type::DESTROYED );
                  return true;
               // LCOV_EXCL_STOP

               case json::type::NULL_:
               case json::type::BOOLEAN:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
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

               case json::type::RAW_PTR:
                  return !m_union.p;
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

      private:
         void seize( data&& r ) noexcept
         {
            assert( m_type != json::type::DESTROYED );

            switch( r.m_type ) {
               case json::type::UNINITIALIZED:
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               // LCOV_EXCL_START
               case json::type::DISCARDED:
                  assert( r.m_type != json::type::DISCARDED );
                  return;

               case json::type::DESTROYED:
                  assert( r.m_type != json::type::DESTROYED );
                  return;
               // LCOV_EXCL_STOP

               case json::type::NULL_:
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::BOOLEAN:
                  m_union.b = r.m_union.b;
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::SIGNED:
                  m_union.i = r.m_union.i;
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::UNSIGNED:
                  m_union.u = r.m_union.u;
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::DOUBLE:
                  m_union.d = r.m_union.d;
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::STRING:
                  new( &m_union.s ) std::string( std::move( r.m_union.s ) );
                  assert( ( r.discard(), true ) );
                  return;

               case json::type::STRING_VIEW:
                  new( &m_union.sv ) tao::string_view( r.m_union.sv );
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::BINARY:
                  new( &m_union.x ) binary_t( std::move( r.m_union.x ) );
                  assert( ( r.discard(), true ) );
                  return;

               case json::type::BINARY_VIEW:
                  new( &m_union.xv ) tao::byte_view( r.m_union.xv );
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;

               case json::type::ARRAY:
                  new( &m_union.a ) array_t( std::move( r.m_union.a ) );
                  assert( ( r.discard(), true ) );
                  return;

               case json::type::OBJECT:
                  new( &m_union.o ) object_t( std::move( r.m_union.o ) );
                  assert( ( r.discard(), true ) );
                  return;

               case json::type::RAW_PTR:
                  m_union.p = r.m_union.p;
                  assert( ( r.m_type = json::type::DISCARDED, true ) );
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void embed( const data& r )
         {
            assert( m_type != json::type::DESTROYED );

            switch( r.m_type ) {
               case json::type::UNINITIALIZED:
                  return;

               case json::type::DISCARDED:
                  throw std::logic_error( "attempt to use a discarded value" );

               case json::type::DESTROYED:
                  throw std::logic_error( "attempt to use a destroyed value" );

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
            }
            assert( false );  // LCOV_EXCL_LINE
         }

      protected:
         json::type m_type = json::type::UNINITIALIZED;
         internal::data_union m_union;
      };

      inline bool operator==( const data& lhs, const data& rhs ) noexcept
      {
         if( lhs.type() == type::RAW_PTR ) {
            if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
               return *p == rhs;
            }
            else {
               return null == rhs;
            }
         }
         if( lhs.type() != rhs.type() ) {
            if( rhs.type() == type::RAW_PTR ) {
               if( const auto* p = rhs.unsafe_get_raw_ptr() ) {
                  return lhs == *p;
               }
               else {
                  return lhs == null;
               }
            }
            if( lhs.type() == type::SIGNED ) {
               if( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs.unsafe_get_unsigned() );
               }
               if( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() == rhs.unsafe_get_double();
               }
            }
            else if( lhs.type() == type::UNSIGNED ) {
               if( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() == static_cast< std::uint64_t >( v ) );
               }
               if( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() == rhs.unsafe_get_double();
               }
            }
            else if( lhs.type() == type::DOUBLE ) {
               if( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_signed();
               }
               if( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_unsigned();
               }
            }
            else if( lhs.type() == type::STRING ) {
               if( rhs.type() == type::STRING_VIEW ) {
                  return lhs.unsafe_get_string() == rhs.unsafe_get_string_view();
               }
            }
            else if( lhs.type() == type::STRING_VIEW ) {
               if( rhs.type() == type::STRING ) {
                  return lhs.unsafe_get_string_view() == rhs.unsafe_get_string();
               }
            }
            else if( lhs.type() == type::BINARY ) {
               if( rhs.type() == type::BINARY_VIEW ) {
                  return lhs.unsafe_get_binary() == rhs.unsafe_get_binary_view();
               }
            }
            else if( lhs.type() == type::BINARY_VIEW ) {
               if( rhs.type() == type::BINARY ) {
                  return lhs.unsafe_get_binary_view() == rhs.unsafe_get_binary();
               }
            }
            assert( lhs.type() != type::DISCARDED );
            assert( lhs.type() != type::DESTROYED );
            assert( rhs.type() != type::DISCARDED );
            assert( rhs.type() != type::DESTROYED );
            return false;
         }
         switch( lhs.type() ) {
            case type::UNINITIALIZED:
               return true;

            // LCOV_EXCL_START
            case type::DISCARDED:
               assert( lhs.type() != type::DISCARDED );
               break;

            case type::DESTROYED:
               assert( lhs.type() != type::DESTROYED );
               break;
            // LCOV_EXCL_STOP

            case type::NULL_:
               return true;

            case type::BOOLEAN:
               return lhs.unsafe_get_boolean() == rhs.unsafe_get_boolean();

            case type::SIGNED:
               return lhs.unsafe_get_signed() == rhs.unsafe_get_signed();

            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() == rhs.unsafe_get_unsigned();

            case type::DOUBLE:
               return lhs.unsafe_get_double() == rhs.unsafe_get_double();

            case type::STRING:
               return lhs.unsafe_get_string() == rhs.unsafe_get_string();

            case type::STRING_VIEW:
               return lhs.unsafe_get_string_view() == rhs.unsafe_get_string_view();

            case type::BINARY:
               return lhs.unsafe_get_binary() == rhs.unsafe_get_binary();

            case type::BINARY_VIEW:
               return lhs.unsafe_get_binary_view() == rhs.unsafe_get_binary_view();

            case type::ARRAY:
               return lhs.unsafe_get_array() == rhs.unsafe_get_array();

            case type::OBJECT:
               return lhs.unsafe_get_object() == rhs.unsafe_get_object();

            case type::RAW_PTR:
               break;  // LCOV_EXCL_LINE
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

      inline bool operator<( const data& lhs, const data& rhs ) noexcept
      {
         if( lhs.type() == type::RAW_PTR ) {
            if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
               return *p < rhs;
            }
            else {
               return null < rhs;
            }
         }
         if( lhs.type() != rhs.type() ) {
            if( rhs.type() == type::RAW_PTR ) {
               if( const auto* p = rhs.unsafe_get_raw_ptr() ) {
                  return lhs < *p;
               }
               else {
                  return lhs < null;
               }
            }
            if( lhs.type() == type::SIGNED ) {
               if( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs.unsafe_get_unsigned() );
               }
               if( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() < rhs.unsafe_get_double();
               }
            }
            else if( lhs.type() == type::UNSIGNED ) {
               if( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() < static_cast< std::uint64_t >( v ) );
               }
               if( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() < rhs.unsafe_get_double();
               }
            }
            else if( lhs.type() == type::DOUBLE ) {
               if( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_signed();
               }
               if( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_unsigned();
               }
            }
            else if( lhs.type() == type::STRING ) {
               if( rhs.type() == type::STRING_VIEW ) {
                  return lhs.unsafe_get_string() < rhs.unsafe_get_string_view();
               }
            }
            else if( lhs.type() == type::STRING_VIEW ) {
               if( rhs.type() == type::STRING ) {
                  return lhs.unsafe_get_string_view() < rhs.unsafe_get_string();
               }
            }
            else if( lhs.type() == type::BINARY ) {
               if( rhs.type() == type::BINARY_VIEW ) {
                  return lhs.unsafe_get_binary() < rhs.unsafe_get_binary_view();
               }
            }
            else if( lhs.type() == type::BINARY_VIEW ) {
               if( rhs.type() == type::BINARY ) {
                  return lhs.unsafe_get_binary_view() < rhs.unsafe_get_binary();
               }
            }
            assert( lhs.type() != type::DISCARDED );
            assert( lhs.type() != type::DESTROYED );
            assert( rhs.type() != type::DISCARDED );
            assert( rhs.type() != type::DESTROYED );
            return lhs.type() < rhs.type();
         }
         switch( lhs.type() ) {
            case type::UNINITIALIZED:
               return false;

            // LCOV_EXCL_START
            case type::DISCARDED:
               assert( lhs.type() != type::DISCARDED );
               break;

            case type::DESTROYED:
               assert( lhs.type() != type::DESTROYED );
               break;
            // LCOV_EXCL_STOP

            case type::NULL_:
               return false;

            case type::BOOLEAN:
               return lhs.unsafe_get_boolean() < rhs.unsafe_get_boolean();

            case type::SIGNED:
               return lhs.unsafe_get_signed() < rhs.unsafe_get_signed();

            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() < rhs.unsafe_get_unsigned();

            case type::DOUBLE:
               return lhs.unsafe_get_double() < rhs.unsafe_get_double();

            case type::STRING:
               return lhs.unsafe_get_string() < rhs.unsafe_get_string();

            case type::STRING_VIEW:
               return lhs.unsafe_get_string_view() < rhs.unsafe_get_string_view();

            case type::BINARY:
               return lhs.unsafe_get_binary() < rhs.unsafe_get_binary();

            case type::BINARY_VIEW:
               return lhs.unsafe_get_binary_view() < rhs.unsafe_get_binary_view();

            case type::ARRAY:
               return lhs.unsafe_get_array() < rhs.unsafe_get_array();

            case type::OBJECT:
               return lhs.unsafe_get_object() < rhs.unsafe_get_object();

            case type::RAW_PTR:
               break;  // LCOV_EXCL_LINE
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

   }  // namespace json

}  // namespace tao

#endif
