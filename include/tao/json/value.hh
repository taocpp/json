// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_VALUE_HH
#define TAOCPP_JSON_INCLUDE_VALUE_HH

#include <cstdint>
#include <cmath>
#include <cassert>
#include <utility>
#include <stdexcept>

#include "external/operators.hpp"
#include "external/optional.hpp"

#include "internal/totally_ordered.hh"
#include "internal/value_union.hh"
#include "internal/get_by_enum.hh"
#include "internal/throw.hh"

#include "type.hh"
#include "traits.hh"
#include "pair.hh"
#include "single.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      class basic_value
         : operators::totally_ordered< basic_value< Traits > >,
           internal::totally_ordered< basic_value< Traits >, null_t, type::NULL_ >,
           internal::totally_ordered< basic_value< Traits >, bool, type::BOOL >,
           internal::totally_ordered< basic_value< Traits >, signed char, type::SIGNED >,
           internal::totally_ordered< basic_value< Traits >, unsigned char, type::UNSIGNED >,
           internal::totally_ordered< basic_value< Traits >, signed short, type::SIGNED >,
           internal::totally_ordered< basic_value< Traits >, unsigned short, type::UNSIGNED >,
           internal::totally_ordered< basic_value< Traits >, signed int, type::SIGNED >,
           internal::totally_ordered< basic_value< Traits >, unsigned int, type::UNSIGNED >,
           internal::totally_ordered< basic_value< Traits >, signed long, type::SIGNED >,
           internal::totally_ordered< basic_value< Traits >, unsigned long, type::UNSIGNED >,
           internal::totally_ordered< basic_value< Traits >, signed long long, type::SIGNED >,
           internal::totally_ordered< basic_value< Traits >, unsigned long long, type::UNSIGNED >,
           internal::totally_ordered< basic_value< Traits >, float, type::DOUBLE >,
           internal::totally_ordered< basic_value< Traits >, double, type::DOUBLE >,
           internal::totally_ordered< basic_value< Traits >, std::string, type::STRING >,
           internal::totally_ordered< basic_value< Traits >, const char *, type::STRING >,
           internal::totally_ordered< basic_value< Traits >, std::vector< basic_value< Traits > >, type::ARRAY >,
           internal::totally_ordered< basic_value< Traits >, std::map< std::string, basic_value< Traits > >, type::OBJECT >
      {
      public:
         basic_value() noexcept
         { }

         basic_value( basic_value && r ) noexcept
              : m_type( r.m_type )
         {
            seize( std::move( r ) );
         }

         basic_value( const basic_value & r )
              : m_type( r.m_type )
         {
            embed( r );
         }

         basic_value( basic_value & r )
              : basic_value( static_cast< const basic_value & >( r ) )
         { }

         template< typename T >
         basic_value( T && v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_value & >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         basic_value( std::initializer_list< pair< basic_value > > && l )
         {
            unsafe_assign( std::move( l ) );
         }

         basic_value( const std::initializer_list< pair< basic_value > > & l )
         {
            unsafe_assign( l );
         }

         basic_value( std::initializer_list< pair< basic_value > > & l )
              : basic_value( static_cast< const std::initializer_list< pair< basic_value > > & >( l ) )
         { }

         ~basic_value() noexcept
         {
            unsafe_destroy();
         }

         static basic_value array( std::initializer_list< single< basic_value > > && l )
         {
            basic_value v;
            v.append( std::move( l ) );
            return v;
         }

         static basic_value array( const std::initializer_list< single< basic_value > > & l )
         {
            basic_value v;
            v.append( l );
            return v;
         }

         basic_value & operator= ( basic_value v ) noexcept
         {
            unsafe_destroy();
            seize( std::move( v ) );
            m_type = v.m_type;
            return * this;
         }

         void swap( basic_value & r ) noexcept
         {
            basic_value t( std::move( r ) );
            r = std::move( * this );
            ( * this ) = ( std::move( t ) );
         }

         json::type type() const noexcept
         {
            return m_type;
         }

         bool is_null() const noexcept
         {
            return m_type == json::type::NULL_;
         }

         bool is_bool() const noexcept
         {
            return m_type == json::type::BOOL;
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

         bool is_array() const noexcept
         {
            return m_type == json::type::ARRAY;
         }

         bool is_object() const noexcept
         {
            return m_type == json::type::OBJECT;
         }

         bool is_pointer() const noexcept
         {
            return m_type == json::type::POINTER;
         }

         null_t get_null() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::NULL_ );
            return unsafe_get_null();
         }

         optional< null_t > optional_null() const
         {
            return m_type == json::type::NULL_ ? unsafe_get_null() : nullopt;
         }

         bool get_bool() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::BOOL );
            return unsafe_get_bool();
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

         std::string & get_string()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         const std::string & get_string() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         std::vector< basic_value > & get_array()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         const std::vector< basic_value > & get_array() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         std::map< std::string, basic_value > & get_object()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const std::map< std::string, basic_value > & get_object() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const basic_value * get_pointer() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::POINTER );
            return unsafe_get_pointer();
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< basic_value > & >() ) ) get()
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< basic_value > & >() ) ) get() const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< typename T >
         T as_number() const
         {
            switch ( m_type ) {
               case json::type::SIGNED:
                  return T( unsafe_get_signed() );
               case json::type::UNSIGNED:
                  return T( unsafe_get_unsigned() );
               case json::type::DOUBLE:
                  return T( unsafe_get_double() );
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( m_type );
            }
         }

         // The unsafe_get_*() accessor functions MUST NOT be
         // called when the type of the value is not the one
         // corresponding to the type of the accessor!

         null_t unsafe_get_null() const noexcept
         {
            return null;
         }

         bool unsafe_get_bool() const noexcept
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

         std::string & unsafe_get_string() noexcept
         {
            return m_union.s;
         }

         const std::string & unsafe_get_string() const noexcept
         {
            return m_union.s;
         }

         std::vector< basic_value > & unsafe_get_array() noexcept
         {
            return m_union.a;
         }

         const std::vector< basic_value > & unsafe_get_array() const noexcept
         {
            return m_union.a;
         }

         std::map< std::string, basic_value > & unsafe_get_object() noexcept
         {
            return m_union.o;
         }

         const std::map< std::string, basic_value > & unsafe_get_object() const noexcept
         {
            return m_union.o;
         }

         const basic_value * unsafe_get_pointer() const noexcept
         {
            return m_union.p;
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< basic_value > & >() ) ) unsafe_get()
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< basic_value > & >() ) ) unsafe_get() const
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         // The following convenience functions operate on
         // the containers for arrays and objects and throw
         // an exception when the type of the value is wrong
         // or the index/key is out of range/not found.

         basic_value & at( const std::size_t index )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         const basic_value & at( const std::size_t index ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         basic_value & at( const std::string & key )
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         const basic_value & at( const std::string & key ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         // The following convenience functions operate on
         // the containers for arrays and result in undefined
         // behaviour when the type of the value is wrong or
         // the index is out of range.

         basic_value & operator[] ( const std::size_t index ) noexcept
         {
            return m_union.a[ index ];
         }

         const basic_value & operator[] ( const std::size_t index ) const noexcept
         {
            return m_union.a[ index ];
         }

         // The following convenience functions operate on
         // the containers for objects and insert a default-
         // constructed value when the key is not found.

         basic_value & operator[] ( const std::string & key )
         {
            return m_union.o[ key ];
         }

         basic_value & operator[] ( std::string && key )
         {
            return m_union.o[ std::move( key ) ];
         }

         const basic_value & operator* () const noexcept
         {
            return * unsafe_get_pointer();
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_destroy( v.type() ) is true!

         template< typename T >
         void unsafe_assign( T && v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_value & >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( std::initializer_list< pair< basic_value > > && l )
         {
            unsafe_emplace_object();
            for( auto & e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
         }

         void unsafe_assign( const std::initializer_list< pair< basic_value > > & l )
         {
            unsafe_emplace_object();
            for( auto & e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
         }

         void unsafe_assign( std::initializer_list< pair< basic_value > > & l )
         {
            unsafe_assign( static_cast< const std::initializer_list< pair< basic_value > > & >( l ) );
         }

         void unsafe_assign_null() noexcept
         {
            m_type = json::type::NULL_;
         }

         void unsafe_assign_bool( const bool b ) noexcept
         {
            m_union.b = b;
            m_type = json::type::BOOL;
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

         void unsafe_assign_double_unchecked( const double d ) noexcept
         {
            // d must be a finite value!
            m_union.d = d;
            m_type = json::type::DOUBLE;
         }

         void unsafe_assign_double( const double d )
         {
            if ( ! std::isfinite( d ) ) {
               throw std::runtime_error( "non-finite double value invalid for json" );
            }
            unsafe_assign_double_unchecked( d );
         }

         template< typename ... Ts >
         void unsafe_emplace_string( Ts && ... ts )
         {
            new ( & m_union.s ) std::string( std::forward< Ts >( ts ) ... );
            m_type = json::type::STRING;
         }

         template< typename ... Ts >
         void emplace_string( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_string( std::forward< Ts >( ts ) ... );
         }

         template< typename ... Ts >
         void unsafe_emplace_array( Ts && ... ts )
         {
            new ( & m_union.a ) std::vector< basic_value >( std::forward< Ts >( ts ) ... );
            m_type = json::type::ARRAY;
         }

         template< typename ... Ts >
         void emplace_array( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_array( std::forward< Ts >( ts ) ... );
         }

         void prepare_array()
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  unsafe_emplace_array();
               case json::type::ARRAY:
                  break;
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename V >
         void unsafe_emplace_back( V && v )
         {
            m_union.a.emplace_back( std::forward< V >( v ) );
         }

         template< typename V >
         void emplace_back( V && v )
         {
            prepare_array();
            unsafe_emplace_back( std::forward< V >( v ) );
         }

         template< typename ... Ts >
         void unsafe_emplace_object( Ts && ... ts )
         {
            new ( & m_union.o ) std::map< std::string, basic_value >( std::forward< Ts >( ts ) ... );
            m_type = json::type::OBJECT;
         }

         template< typename ... Ts >
         void emplace_object( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_object( std::forward< Ts >( ts ) ... );
         }

         void prepare_object()
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  unsafe_emplace_object();
               case json::type::OBJECT:
                  break;
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename K, typename V >
         std::pair< typename std::map< std::string, basic_value >::iterator, bool > unsafe_emplace( K && k, V && v )
         {
            return m_union.o.emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         template< typename K, typename V >
         std::pair< typename std::map< std::string, basic_value >::iterator, bool > emplace( K && k, V && v )
         {
            prepare_object();
            return unsafe_emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         void unsafe_assign_pointer( const basic_value * p ) noexcept
         {
            m_union.p = p;
            m_type = json::type::POINTER;
         }

         void append( std::initializer_list< single< basic_value > > && l )
         {
            prepare_array();
            auto & v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( auto & e : l ) {
               unsafe_emplace_back( std::move( e.value ) );
            }
         }

         void append( const std::initializer_list< single< basic_value > > & l )
         {
            prepare_array();
            auto & v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( const auto & e : l ) {
               unsafe_emplace_back( e.value );
            }
         }

         void insert( std::initializer_list< pair< basic_value > > && l )
         {
            prepare_object();
            for( auto & e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
         }

         void insert( const std::initializer_list< pair< basic_value > > & l )
         {
            prepare_object();
            for( auto & e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
         }

         bool empty() const noexcept
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  return true;
               case json::type::BOOL:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
                  return false;
               case json::type::STRING:
                  return m_union.s.empty();
               case json::type::ARRAY:
                  return m_union.a.empty();
               case json::type::OBJECT:
                  return m_union.o.empty();
               case json::type::POINTER:
                  return !m_union.p;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

      private:
         void seize( basic_value && r ) noexcept
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
                  return;
               case json::type::BOOL:
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
                  new ( & m_union.s ) std::string( std::move( r.m_union.s ) );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< basic_value >( std::move( r.m_union.a ) );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, basic_value >( std::move( r.m_union.o ) );
                  return;
               case json::type::POINTER:
                  m_union.p = r.m_union.p;
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void embed( const basic_value & r )
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
                  return;
               case json::type::BOOL:
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
                  new ( & m_union.s ) std::string( r.m_union.s );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< basic_value >( r.m_union.a );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, basic_value >( r.m_union.o );
                  return;
               case json::type::POINTER:
                  m_union.p = r.m_union.p;
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void unsafe_destroy() noexcept
         {
            switch ( m_type ) {
               case json::type::NULL_:
               case json::type::BOOL:
               case json::type::SIGNED:
               case json::type::UNSIGNED:
               case json::type::DOUBLE:
               case json::type::POINTER:
                  return;
               case json::type::STRING:
                  m_union.s.~basic_string();
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

         void destroy() noexcept
         {
            unsafe_destroy();
            m_type = json::type::NULL_;
         }

         internal::value_union< basic_value > m_union;
         json::type m_type = json::type::NULL_;
      };

      template< template< typename ... > class Traits >
      bool operator== ( const basic_value< Traits > & lhs, const basic_value< Traits > & rhs ) noexcept
      {
         if ( lhs.type() == type::POINTER ) {
            if ( const auto * p = lhs.unsafe_get_pointer() ) {
               return * p == rhs;
            }
            else {
               return null == rhs;
            }
         }
         if ( lhs.type() != rhs.type() ) {
            if ( rhs.type() == type::POINTER ) {
               if ( const auto * p = rhs.unsafe_get_pointer() ) {
                  return lhs == * p;
               }
               else {
                  return lhs == null;
               }
            }
            if ( lhs.type() == type::SIGNED ) {
               if ( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs.unsafe_get_unsigned() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED ) {
               if ( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() == static_cast< std::uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_signed();
               }
               if ( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_unsigned();
               }
            }
            return false;
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return true;
            case type::BOOL:
               return lhs.unsafe_get_bool() == rhs.unsafe_get_bool();
            case type::SIGNED:
               return lhs.unsafe_get_signed() == rhs.unsafe_get_signed();
            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() == rhs.unsafe_get_unsigned();
            case type::DOUBLE:
               return lhs.unsafe_get_double() == rhs.unsafe_get_double();
            case type::STRING:
               return lhs.unsafe_get_string() == rhs.unsafe_get_string();
            case type::ARRAY:
               return lhs.unsafe_get_array() == rhs.unsafe_get_array();
            case type::OBJECT:
               return lhs.unsafe_get_object() == rhs.unsafe_get_object();
            case type::POINTER:
               break;  // LCOV_EXCL_LINE
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      template< template< typename ... > class Traits >
      bool operator< ( const basic_value< Traits > & lhs, const basic_value< Traits > & rhs ) noexcept
      {
         if ( lhs.type() == type::POINTER ) {
            if ( const auto * p = lhs.unsafe_get_pointer() ) {
               return * p < rhs;
            }
            else {
               return null < rhs;
            }
         }
         if ( lhs.type() != rhs.type() ) {
            if ( rhs.type() == type::POINTER ) {
               if ( const auto * p = rhs.unsafe_get_pointer() ) {
                  return lhs < * p;
               }
               else {
                  return lhs < null;
               }
            }
            if ( lhs.type() == type::SIGNED ) {
               if ( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs.unsafe_get_unsigned() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED ) {
               if ( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() < static_cast< std::uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_signed();
               }
               if ( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_unsigned();
               }
            }
            return lhs.type() < rhs.type();
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return false;
            case type::BOOL:
               return lhs.unsafe_get_bool() < rhs.unsafe_get_bool();
            case type::SIGNED:
               return lhs.unsafe_get_signed() < rhs.unsafe_get_signed();
            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() < rhs.unsafe_get_unsigned();
            case type::DOUBLE:
               return lhs.unsafe_get_double() < rhs.unsafe_get_double();
            case type::STRING:
               return lhs.unsafe_get_string() < rhs.unsafe_get_string();
            case type::ARRAY:
               return lhs.unsafe_get_array() < rhs.unsafe_get_array();
            case type::OBJECT:
               return lhs.unsafe_get_object() < rhs.unsafe_get_object();
            case type::POINTER:
               break;  // LCOV_EXCL_LINE
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      using value = basic_value< traits >;

   } // json

} // tao

#endif
