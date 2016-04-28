// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_VALUE_HH
#define TAOCPP_JSON_INCLUDE_VALUE_HH

#include <cmath>
#include <cassert>
#include <utility>
#include <stdexcept>

#include "external/operators.hpp"

#include "internal/totally_ordered.hh"
#include "internal/value_union.hh"
#include "internal/pair.hh"
#include "internal/single.hh"
#include "internal/get_by_enum.hh"
#include "internal/throw.hh"

#include "type.hh"
#include "traits.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      class value_base
         : operators::totally_ordered< value_base< Traits > >,
           internal::totally_ordered< value_base< Traits >, null_t, type::NULL_ >,
           internal::totally_ordered< value_base< Traits >, bool, type::BOOL_ >,
           internal::totally_ordered< value_base< Traits >, signed char, type::SIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, unsigned char, type::UNSIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, signed short, type::SIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, unsigned short, type::UNSIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, signed int, type::SIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, unsigned int, type::UNSIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, signed long, type::SIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, unsigned long, type::UNSIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, signed long long, type::SIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, unsigned long long, type::UNSIGNED_INTEGER >,
           internal::totally_ordered< value_base< Traits >, float, type::DOUBLE >,
           internal::totally_ordered< value_base< Traits >, double, type::DOUBLE >,
           internal::totally_ordered< value_base< Traits >, std::string, type::STRING >,
           internal::totally_ordered< value_base< Traits >, const char *, type::STRING >,
           internal::totally_ordered< value_base< Traits >, std::vector< value_base< Traits > >, type::ARRAY >,
           internal::totally_ordered< value_base< Traits >, std::map< std::string, value_base< Traits > >, type::OBJECT >
      {
      public:
         value_base() noexcept
         { }

         value_base( value_base && r ) noexcept
         {
            seize( std::move( r ) );
            m_type = r.m_type;
         }

         value_base( const value_base & r )
         {
            embed( r );
            m_type = r.m_type;
         }

         value_base( value_base & r )
         {
            embed( r );
            m_type = r.m_type;
         }

         template< typename T >
         value_base( T && v ) noexcept( noexcept( std::declval< value_base & >().unsafe_assign( std::forward< T >( v ) ) ) )
         {
            unsafe_assign( std::forward< T >( v ) );
         }

         value_base( std::initializer_list< internal::pair< value_base > > && l )
         {
            unsafe_assign( std::move( l ) );
         }

         value_base( const std::initializer_list< internal::pair< value_base > > & l )
         {
            unsafe_assign( l );
         }

         value_base( std::initializer_list< internal::pair< value_base > > & l )
         {
            unsafe_assign( l );
         }

         ~value_base() noexcept
         {
            unsafe_destroy();
         }

         static value_base array( std::initializer_list< internal::single< value_base > > && l )
         {
            value_base v;
            v.append( std::move( l ) );
            return v;
         }

         static value_base array( const std::initializer_list< internal::single< value_base > > & l )
         {
            value_base v;
            v.append( l );
            return v;
         }

         value_base & operator= ( value_base v ) noexcept
         {
            unsafe_destroy();
            seize( std::move( v ) );
            m_type = v.m_type;
            return * this;
         }

         void swap( value_base & r ) noexcept
         {
            value_base t( std::move( r ) );
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
            return m_type == json::type::BOOL_;
         }

         bool is_signed_integer() const noexcept
         {
            return m_type == json::type::SIGNED_INTEGER;
         }

         bool is_unsigned_integer() const noexcept
         {
            return m_type == json::type::UNSIGNED_INTEGER;
         }

         bool is_integer() const noexcept
         {
            return is_signed_integer() || is_unsigned_integer();
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
            CHECK_TYPE_ERROR( m_type, json::type::NULL_ );
            return unsafe_get_null();
         }

         bool get_bool() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::BOOL_ );
            return unsafe_get_bool();
         }

         int64_t get_signed_integer() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::SIGNED_INTEGER );
            return unsafe_get_signed_integer();
         }

         uint64_t get_unsigned_integer() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::UNSIGNED_INTEGER );
            return unsafe_get_unsigned_integer();
         }

         double get_double() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::DOUBLE );
            return unsafe_get_double();
         }

         std::string & get_string()
         {
            CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         const std::string & get_string() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_get_string();
         }

         std::vector< value_base > & get_array()
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         const std::vector< value_base > & get_array() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_get_array();
         }

         std::map< std::string, value_base > & get_object()
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const std::map< std::string, value_base > & get_object() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_get_object();
         }

         const value_base * get_pointer() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::POINTER );
            return unsafe_get_pointer();
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< value_base > & >() ) ) get()
         {
            CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< value_base > & >() ) ) get() const
         {
            CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         template< typename T >
         T as_number() const
         {
            switch ( m_type ) {
               case json::type::SIGNED_INTEGER:
                  return T( unsafe_get_signed_integer() );
               case json::type::UNSIGNED_INTEGER:
                  return T( unsafe_get_unsigned_integer() );
               case json::type::DOUBLE:
                  return T( unsafe_get_double() );
               default:
                  THROW_TYPE_ERROR( m_type );
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

         int64_t unsafe_get_signed_integer() const noexcept
         {
            return m_union.i;
         }

         uint64_t unsafe_get_unsigned_integer() const noexcept
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

         std::vector< value_base > & unsafe_get_array() noexcept
         {
            return m_union.a;
         }

         const std::vector< value_base > & unsafe_get_array() const noexcept
         {
            return m_union.a;
         }

         std::map< std::string, value_base > & unsafe_get_object() noexcept
         {
            return m_union.o;
         }

         const std::map< std::string, value_base > & unsafe_get_object() const noexcept
         {
            return m_union.o;
         }

         const value_base * unsafe_get_pointer() const noexcept
         {
            return m_union.p;
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< internal::value_union< value_base > & >() ) ) unsafe_get()
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         template< json::type E >
         decltype( internal::get_by_enum< E >::get( std::declval< const internal::value_union< value_base > & >() ) ) unsafe_get() const
         {
            return internal::get_by_enum< E >::get( m_union );
         }

         // The following convenience functions operate on
         // the containers for arrays and objects and throw
         // an exception when the type of the value is wrong
         // or the index/key is out of range/not found.

         value_base & at( const std::size_t index )
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         const value_base & at( const std::size_t index ) const
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         value_base & at( const std::string & key )
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         const value_base & at( const std::string & key ) const
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( key );
         }

         // The following convenience functions operate on
         // the containers for arrays and result in undefined
         // behaviour when the type of the value is wrong or
         // the index is out of range.

         value_base & operator[] ( const std::size_t index ) noexcept
         {
            return m_union.a[ index ];
         }

         const value_base & operator[] ( const std::size_t index ) const noexcept
         {
            return m_union.a[ index ];
         }

         // The following convenience functions operate on
         // the containers for objects and insert a default-
         // constructed value when the key is not found.

         value_base & operator[] ( const std::string & key )
         {
            return m_union.o[ key ];
         }

         value_base & operator[] ( std::string && key )
         {
            return m_union.o[ std::move( key ) ];
         }

         const value_base & operator* () const noexcept
         {
            return * unsafe_get_pointer();
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_destroy( v.type() ) is true!

         template< typename T >
         void unsafe_assign( T && v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< value_base & >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( std::initializer_list< internal::pair< value_base > > && l )
         {
            unsafe_emplace_object();
            *this += std::move( l );
         }

         void unsafe_assign( const std::initializer_list< internal::pair< value_base > > & l )
         {
            unsafe_emplace_object();
            *this += l;
         }

         void unsafe_assign( std::initializer_list< internal::pair< value_base > > & l )
         {
            unsafe_emplace_object();
            *this += l;
         }

         void unsafe_assign_null() noexcept
         {
            m_type = json::type::NULL_;
         }

         void unsafe_assign_bool( const bool b ) noexcept
         {
            m_union.b = b;
            m_type = json::type::BOOL_;
         }

         void unsafe_assign_signed_integer( const int64_t i ) noexcept
         {
            m_union.i = i;
            m_type = json::type::SIGNED_INTEGER;
         }

         void unsafe_assign_unsigned_integer( const uint64_t u ) noexcept
         {
            m_union.u = u;
            m_type = json::type::UNSIGNED_INTEGER;
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
            new ( & m_union.a ) std::vector< value_base >( std::forward< Ts >( ts ) ... );
            m_type = json::type::ARRAY;
         }

         template< typename ... Ts >
         void emplace_array( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_array( std::forward< Ts >( ts ) ... );
         }

         void unsafe_emplace_back_prepare()
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  unsafe_emplace_array();
               case json::type::ARRAY:
                  break;
               default:
                  THROW_TYPE_ERROR( m_type );
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
            unsafe_emplace_back_prepare();
            unsafe_emplace_back( std::forward< V >( v ) );
         }

         template< typename ... Ts >
         void unsafe_emplace_object( Ts && ... ts )
         {
            new ( & m_union.o ) std::map< std::string, value_base >( std::forward< Ts >( ts ) ... );
            m_type = json::type::OBJECT;
         }

         template< typename ... Ts >
         void emplace_object( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_object( std::forward< Ts >( ts ) ... );
         }

         void unsafe_emplace_prepare()
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  unsafe_emplace_object();
               case json::type::OBJECT:
                  break;
               default:
                  THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename K, typename V >
         std::pair< typename std::map< std::string, value_base >::iterator, bool > unsafe_emplace( K && k, V && v )
         {
            return m_union.o.emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         template< typename K, typename V >
         std::pair< typename std::map< std::string, value_base >::iterator, bool > emplace( K && k, V && v )
         {
            unsafe_emplace_prepare();
            return unsafe_emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         void unsafe_assign_pointer( const value_base * p ) noexcept
         {
            m_union.p = p;
            m_type = json::type::POINTER;
         }

         void append( std::initializer_list< internal::single< value_base > > && l )
         {
            unsafe_emplace_back_prepare();
            auto & v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( auto & e : l ) {
               unsafe_emplace_back( std::move( e.e ) );
            }
         }

         void append( const std::initializer_list< internal::single< value_base > > & l )
         {
            unsafe_emplace_back_prepare();
            auto & v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( const auto & e : l ) {
               unsafe_emplace_back( e.e );
            }
         }

         value_base & operator+= ( std::initializer_list< internal::pair< value_base > > && l )
         {
            unsafe_emplace_prepare();
            for( auto & e : l ) {
               const auto r = unsafe_emplace( std::move( e.e.first ), std::move( e.e.second ) );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
            return *this;
         }

         value_base & operator+= ( const std::initializer_list< internal::pair< value_base > > & l )
         {
            unsafe_emplace_prepare();
            for( const auto & e : l ) {
               const auto r = unsafe_emplace( e.e.first, e.e.second );
               if ( ! r.second ) {
                  throw std::runtime_error( "duplicate key detected: " + r.first->first );
               }
            }
            return *this;
         }

         value_base & operator-= ( const std::string & k )
         {
            if ( get_object().erase( k ) == 0 ) {
               throw std::runtime_error( "key not found: " + k );
            }
            return *this;
         }

         value_base & operator-= ( std::initializer_list< std::string > l )
         {
            auto & v = get_object();
            for ( const auto & k : l ) {
               if ( v.erase( k ) == 0 ) {
                  throw std::runtime_error( "key not found: " + k );
               }
            }
            return *this;
         }

         bool empty() const noexcept
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  return true;
               case json::type::BOOL_:
               case json::type::SIGNED_INTEGER:
               case json::type::UNSIGNED_INTEGER:
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
         void seize( value_base && r ) noexcept
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
                  return;
               case json::type::BOOL_:
                  m_union.b = r.m_union.b;
                  return;
               case json::type::SIGNED_INTEGER:
                  m_union.i = r.m_union.i;
                  return;
               case json::type::UNSIGNED_INTEGER:
                  m_union.u = r.m_union.u;
                  return;
               case json::type::DOUBLE:
                  m_union.d = r.m_union.d;
                  return;
               case json::type::STRING:
                  new ( & m_union.s ) std::string( std::move( r.m_union.s ) );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< value_base >( std::move( r.m_union.a ) );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, value_base >( std::move( r.m_union.o ) );
                  return;
               case json::type::POINTER:
                  m_union.p = r.m_union.p;
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void embed( const value_base & r )
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
                  return;
               case json::type::BOOL_:
                  m_union.b = r.m_union.b;
                  return;
               case json::type::SIGNED_INTEGER:
                  m_union.i = r.m_union.i;
                  return;
               case json::type::UNSIGNED_INTEGER:
                  m_union.u = r.m_union.u;
                  return;
               case json::type::DOUBLE:
                  m_union.d = r.m_union.d;
                  return;
               case json::type::STRING:
                  new ( & m_union.s ) std::string( r.m_union.s );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< value_base >( r.m_union.a );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, value_base >( r.m_union.o );
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
               case json::type::BOOL_:
               case json::type::SIGNED_INTEGER:
               case json::type::UNSIGNED_INTEGER:
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

         internal::value_union< value_base > m_union;
         json::type m_type = json::type::NULL_;
      };

      template< template< typename ... > class Traits >
      bool operator== ( const value_base< Traits > & lhs, const value_base< Traits > & rhs ) noexcept
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
            if ( lhs.type() == type::SIGNED_INTEGER ) {
               if ( rhs.type() == type::UNSIGNED_INTEGER ) {
                  const auto v = lhs.unsafe_get_signed_integer();
                  return ( v >= 0 ) && ( static_cast< uint64_t >( v ) == rhs.unsafe_get_unsigned_integer() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed_integer() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED_INTEGER ) {
               if ( rhs.type() == type::SIGNED_INTEGER ) {
                  const auto v = rhs.unsafe_get_signed_integer();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned_integer() == static_cast< uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned_integer() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_signed_integer();
               }
               if ( rhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_unsigned_integer();
               }
            }
            return false;
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return true;
            case type::BOOL_:
               return lhs.unsafe_get_bool() == rhs.unsafe_get_bool();
            case type::SIGNED_INTEGER:
               return lhs.unsafe_get_signed_integer() == rhs.unsafe_get_signed_integer();
            case type::UNSIGNED_INTEGER:
               return lhs.unsafe_get_unsigned_integer() == rhs.unsafe_get_unsigned_integer();
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
      bool operator< ( const value_base< Traits > & lhs, const value_base< Traits > & rhs ) noexcept
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
            if ( lhs.type() == type::SIGNED_INTEGER ) {
               if ( rhs.type() == type::UNSIGNED_INTEGER ) {
                  const auto v = lhs.unsafe_get_signed_integer();
                  return ( v < 0 ) || ( static_cast< uint64_t >( v ) < rhs.unsafe_get_unsigned_integer() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed_integer() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED_INTEGER ) {
               if ( rhs.type() == type::SIGNED_INTEGER ) {
                  const auto v = rhs.unsafe_get_signed_integer();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned_integer() < static_cast< uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned_integer() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_signed_integer();
               }
               if ( rhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_unsigned_integer();
               }
            }
            return lhs.type() < rhs.type();
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return false;
            case type::BOOL_:
               return lhs.unsafe_get_bool() < rhs.unsafe_get_bool();
            case type::SIGNED_INTEGER:
               return lhs.unsafe_get_signed_integer() < rhs.unsafe_get_signed_integer();
            case type::UNSIGNED_INTEGER:
               return lhs.unsafe_get_unsigned_integer() < rhs.unsafe_get_unsigned_integer();
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

      using value = value_base< traits >;

   } // json

} // tao

#endif
