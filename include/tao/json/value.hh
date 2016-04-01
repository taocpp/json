// Copyright (c) 2015-2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_VALUE_HH
#define TAOCPP_JSON_INCLUDE_VALUE_HH

#include <cmath>
#include <cassert>
#include <utility>
#include <stdexcept>

#include "external/sequences/make_integer_sequence.hpp"
#include "external/operators.hpp"

#include "internal/value_union.hh"
#include "internal/get_by_enum.hh"
#include "internal/throw.hh"

#include "type.hh"

namespace tao
{
   namespace json
   {
      class value
         : operators::totally_ordered< value >,
           operators::totally_ordered< value, std::nullptr_t >, // null
           operators::totally_ordered< value, bool >, // bool
           operators::totally_ordered< value, signed char >, // int64
           operators::totally_ordered< value, unsigned char >, // int64
           operators::totally_ordered< value, signed short >, // int64
           operators::totally_ordered< value, unsigned short >, // int64
           operators::totally_ordered< value, signed int >, // int64
           operators::totally_ordered< value, unsigned int >, // int64
           operators::totally_ordered< value, signed long >, // int64
           operators::totally_ordered< value, signed long long >, // int64
           operators::totally_ordered< value, double >, // double
           operators::totally_ordered< value, std::string >, // string
           operators::totally_ordered< value, std::vector< value > >, // array
           operators::totally_ordered< value, std::map< std::string, value > > // object
      {
      public:
         value() noexcept
               : m_type( json::type::NULL_ )
         { }

         value( value && r ) noexcept
               : m_type( r.m_type )
         {
            seize( std::move( r ) );
            r.m_type = json::type::NULL_;
         }

         value( const value & r )
               : m_type( r.m_type )
         {
            embed( r );
         }

         value( value & r )
               : m_type( r.m_type )
         {
            embed( r );
         }

         template< typename... Ts >
         value( Ts && ... ts ) // TODO: noexcept( noexcept( unsafe_assign( std::forward< T >( v ) ) ) )
               : m_type( json::type::NULL_ )
         {
            unsafe_assign( std::forward< Ts >( ts ) ... );
         }

         value( std::initializer_list< std::pair< const std::string, value > > l )
               : m_type( json::type::OBJECT )
         {
            new ( & m_union.o ) std::map< std::string, value >( l );
            assert( m_union.o.size() == l.size() ); // if this fires, we found duplicate keys
         }

         template< typename... Ts >
         static value array( Ts && ... ts )
         {
            return std::vector< value >( { std::forward< Ts >( ts )... } );
         }

         // template< typename T >
         // explicit
         // value( std::initializer_list< T > && l )
         //       : m_type( json::type::ARRAY )
         // {
         //    create_array( std::move( l ) );
         // }

         // template< typename T >
         // explicit
         // value( const std::initializer_list< T > & l )
         //       : m_type( json::type::ARRAY )
         // {
         //    create_array( l );
         // }

         // template< typename ... Ts >
         // explicit
         // value( std::tuple< Ts ... > && t )
         //       : value( json_seq::index_sequence_for< Ts ... >(), std::move( t ) )
         // { }

         // template< typename ... Ts >
         // explicit
         // value( const std::tuple< Ts ... > & t )
         //       : value( json_seq::index_sequence_for< Ts ... >(), t )
         // { }

         // template< typename I, typename = typename std::enable_if< std::is_constructible< value, decltype( * std::declval< I >() ) >::value >::type >
         // value( const I & begin, const I & end )
         //       : m_type( json::type::ARRAY )
         // {
         //    new ( & m_union.a ) std::vector< value >( begin, end );
         // }

         // template< typename I, typename = typename std::enable_if< std::is_constructible< std::pair< const std::string, value >, decltype( * std::declval< I >() ) >::value >::type >
         // value( const I & begin, const I & end )
         //       : m_type( json::type::OBJECT )
         // {
         //    new ( & m_union.o ) std::map< std::string, value >( begin, end );
         // }

         ~value() noexcept
         {
            destroy();
         }

         template< typename T >
         value& operator= ( T && v ) // noexcept( noexcept( assign( std::forward< T >( v ) ) ) )
         {
            assign( std::forward< T >( v ) );
            return *this;
         }

         value& operator= ( std::initializer_list< std::pair< const std::string, value > > l )
         {
            value( l ).swap( *this );
            return *this;
         }

         // template< typename ... Ts >
         // void operator= ( std::tuple< Ts ... > && t )
         // {
         //    destroy();
         //    unsafe_assign( json_seq::index_sequence_for< Ts ... >(), std::move( t ) );
         // }

         // template< typename ... Ts >
         // void operator= ( const std::tuple< Ts ... > & t )
         // {
         //    destroy();
         //    unsafe_assign( json_seq::index_sequence_for< Ts ... >(), t );
         // }

         const value & operator= ( value && r ) noexcept
         {
            if ( this != & r ) {
               destroy();
               m_type = r.m_type;
               seize( std::move( r ) );
               r.m_type = json::type::NULL_;
            }
            return * this;
         }

         const value & operator= ( const value & r )
         {
            if ( this != & r ) {
               destroy();
               m_type = r.m_type;
               embed( r );
            }
            return * this;
         }

         void swap( value & r ) noexcept
         {
            value t( std::move( r ) );
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

         bool is_int64() const noexcept
         {
            return m_type == json::type::INT64;
         }

         bool is_double() const noexcept
         {
            return m_type == json::type::DOUBLE;
         }

         bool is_number() const noexcept
         {
            return is_int64() || is_double();
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

         std::nullptr_t get_null() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::NULL_ );
            return unsafe_null();
         }

         bool get_bool() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::BOOL_ );
            return unsafe_bool();
         }

         int64_t get_int64() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::INT64 );
            return unsafe_int64();
         }

         double get_double() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::DOUBLE );
            return unsafe_double();
         }

         template< typename T >
         T as_number() const
         {
            switch ( m_type ) {
               case json::type::INT64:
                  return T( unsafe_int64() );
               case json::type::DOUBLE:
                  return T( unsafe_double() );
               default:
                  THROW_TYPE_ERROR( m_type );
            }
         }

         std::string & get_string()
         {
            CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_string();
         }

         const std::string & get_string() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::STRING );
            return unsafe_string();
         }

         std::vector< value > & get_array()
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_array();
         }

         const std::vector< value > & get_array() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return unsafe_array();
         }

         std::map< std::string, value > & get_object()
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_object();
         }

         const std::map< std::string, value > & get_object() const
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return unsafe_object();
         }

         template< json::type E >
         typename internal::get_by_enum< E >::type get_by_enum() const
         {
            CHECK_TYPE_ERROR( m_type, E );
            return internal::get_by_enum< E >::get( m_union );
         }

         // The unsafe_foo() accessor functions MUST NOT be
         // called when the type of the value is not the one
         // corresponding to the type of the accessor!

         std::nullptr_t unsafe_null() const noexcept
         {
            return nullptr;
         }

         bool unsafe_bool() const noexcept
         {
            return m_union.b;
         }

         int64_t unsafe_int64() const noexcept
         {
            return m_union.i;
         }

         double unsafe_double() const noexcept
         {
            return m_union.d;
         }

         std::string & unsafe_string() noexcept
         {
            return m_union.s;
         }

         const std::string & unsafe_string() const noexcept
         {
            return m_union.s;
         }

         std::vector< value > & unsafe_array() noexcept
         {
            return m_union.a;
         }

         const std::vector< value > & unsafe_array() const noexcept
         {
            return m_union.a;
         }

         std::map< std::string, value > & unsafe_object() noexcept
         {
            return m_union.o;
         }

         const std::map< std::string, value > & unsafe_object() const noexcept
         {
            return m_union.o;
         }

         // The following convenience functions operate on
         // the containers for arrays and objects and throw
         // an exception when the type of the value is wrong.

         value & operator[] ( const size_t index )
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         value & operator[] ( const std::string & index )
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( index );
         }

         const value & operator[] ( const size_t index ) const
         {
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         const value & operator[] ( const std::string & index ) const
         {
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( index );
         }

      private:
         value & operator() ()
         {
            return * this;
         }

         const value & operator() () const
         {
            return * this;
         }

      public:
         template< typename K, typename ... Ks >
         value & operator() ( const K & k, const Ks & ... ks )
         {
            return ( this->operator[] ( k ) )( ks ... );
         }

         template< typename K, typename ... Ks >
         const value & operator() ( const K & k, const Ks & ... ks ) const
         {
            return ( this->operator[] ( k ) )( ks ... );
         }

         template< typename V >
         void emplace_back( V && v )
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  new ( & m_union.a ) std::vector< value >();
                  m_type = json::type::ARRAY;
               case json::type::ARRAY:
                  m_union.a.emplace_back( std::forward< V >( v ) );
                  break;
               default:
                  THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename K, typename V >
         void emplace( K && k, V && v )
         {
            switch ( m_type ) {
               case json::type::NULL_:
                  new ( & m_union.o ) std::map< std::string, value >();
                  m_type = json::type::OBJECT;
               case json::type::OBJECT:
                  m_union.o.emplace( std::forward< K >( k ), std::forward< V >( v ) );
                  break;
               default:
                  THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename ... Ts >
         void assign( Ts && ... ts )
         {
            destroy();
            unsafe_assign( std::forward< Ts >( ts ) ... );
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_destroy( v.type() ) is true!

         void unsafe_assign( const std::nullptr_t ) noexcept
         {
            m_type = json::type::NULL_;
         }

         void unsafe_assign( const bool b ) noexcept
         {
            m_type = json::type::BOOL_;
            m_union.b = b;
         }

         void unsafe_assign( const signed char i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const unsigned char i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const signed short i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const unsigned short i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const signed int i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const unsigned int i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const signed long i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const signed long long i ) noexcept
         {
            m_type = json::type::INT64;
            m_union.i = i;
         }

         void unsafe_assign( const double d )
         {
            check_finite( d );
            m_type = json::type::DOUBLE;
            m_union.d = d;
         }

         void unsafe_assign( const empty_array_t ) noexcept
         {
            m_type = json::type::ARRAY;
            new ( & m_union.a ) std::vector< value >();
         }

         void unsafe_assign( const empty_object_t ) noexcept
         {
            m_type = json::type::OBJECT;
            new ( & m_union.o ) std::map< std::string, value >();
         }

         void unsafe_assign( std::string && s ) noexcept
         {
            m_type = json::type::STRING;
            new ( & m_union.s ) std::string( std::move( s ) );
         }

         void unsafe_assign( const char * s )
         {
            m_type = json::type::STRING;
            new ( & m_union.s ) std::string( s );
         }

         void unsafe_assign( const char * s, const std::size_t l )
         {
            m_type = json::type::STRING;
            new ( & m_union.s ) std::string( s, l );
         }

         void unsafe_assign( const std::string & s )
         {
            m_type = json::type::STRING;
            new ( & m_union.s ) std::string( s );
         }

         void unsafe_assign( std::vector< value > && a ) noexcept
         {
            new ( & m_union.a ) std::vector< value >( std::move( a ) );
            m_type = json::type::ARRAY;
         }

         void unsafe_assign( const std::vector< value > & a )
         {
            new ( & m_union.a ) std::vector< value >( a );
            m_type = json::type::ARRAY;
         }

         void unsafe_assign( std::map< std::string, value > && o ) noexcept
         {
            new ( & m_union.o ) std::map< std::string, value >( std::move( o ) );
            m_type = json::type::OBJECT;
         }

         void unsafe_assign( const std::map< std::string, value > & o )
         {
            new ( & m_union.o ) std::map< std::string, value >( o );
            m_type = json::type::OBJECT;
         }

         // template< typename I, std::enable_if< std::is_constructible< value, decltype( * std::declval< I >() ) >::value, int > = 0 >
         // void unsafe_assign( const I & begin, const I & end )
         // {
         //    new ( & m_union.a ) std::vector< value >( begin, end );
         //    m_type = json::type::ARRAY;
         // }

         // template< typename I, std::enable_if< std::is_constructible< std::pair< const std::string, value >, decltype( * std::declval< I >() ) >::value, int > = 0 >
         // void unsafe_assign( const I & begin, const I & end )
         // {
         //    new ( & m_union.o ) std::map< std::string, value >( begin, end );
         //    m_type = json::type::OBJECT;
         // }

      private:
         value( const char ) = delete;
         value( const unsigned long ) = delete;
         value( const unsigned long long ) = delete;

         void operator= ( const char ) = delete;
         void operator= ( const unsigned long ) = delete;
         void operator= ( const unsigned long long ) = delete;

         // explicit
         // value( const json::type t )
         //       : m_type( t )
         // { }

         // template< std::size_t ... Is, typename ... Ts >
         // value( json_seq::index_sequence< Is ... >, std::tuple< Ts ... > && t )
         //       : m_type( json::type::ARRAY )
         // {
         //    new ( & m_union.a ) std::vector< value >( { std::move( std::get< Is >( t ) ) ... } );
         // }

         // template< std::size_t ... Is, typename ... Ts >
         // value( json_seq::index_sequence< Is ... >, const std::tuple< Ts ... > & t )
         //       : m_type( json::type::ARRAY )
         // {
         //    new ( & m_union.a ) std::vector< value >( { std::get< Is >( t ) ... } );
         // }

         // template< std::size_t ... Is, typename ... Ts >
         // void unsafe_assign( json_seq::index_sequence< Is ... >, std::tuple< Ts ... > && t )
         // {
         //    new ( & m_union.a ) std::vector< value >( { std::move( std::get< Is >( t ) ) ... } );
         //    m_type = json::type::ARRAY;
         // }

         // template< std::size_t ... Is, typename ... Ts >
         // void unsafe_assign( json_seq::index_sequence< Is ... >, const std::tuple< Ts ... > & t )
         // {
         //    new ( & m_union.a ) std::vector< value >( { std::get< Is >( t ) ... } );
         //    m_type = json::type::ARRAY;
         // }

         template< typename T >
         void create_array( std::initializer_list< T > && l )
         {
            new ( & m_union.a ) std::vector< value >();
            m_union.a.reserve( l.size() );
            for ( auto && t : l ) {
               m_union.a.emplace_back( std::move( t ) );
            }
         }

         template< typename T >
         void create_array( const std::initializer_list< T > & l )
         {
            new ( & m_union.a ) std::vector< value >();
            m_union.a.reserve( l.size() );
            for ( const auto & t : l ) {
               m_union.a.emplace_back( t );
            }
         }

         void seize( value && r ) noexcept
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
               case json::type::BOOL_:
               case json::type::INT64:
               case json::type::DOUBLE:
                  m_union.i = r.m_union.i;
                  return;
               case json::type::STRING:
                  new ( & m_union.s ) std::string( std::move( r.m_union.s ) );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< value >( std::move( r.m_union.a ) );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, value >( std::move( r.m_union.o ) );
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void embed( const value & r )
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
               case json::type::BOOL_:
               case json::type::INT64:
               case json::type::DOUBLE:
                  m_union.i = r.m_union.i;
                  return;
               case json::type::STRING:
                  new ( & m_union.s ) std::string( r.m_union.s );
                  return;
               case json::type::ARRAY:
                  new ( & m_union.a ) std::vector< value >( r.m_union.a );
                  return;
               case json::type::OBJECT:
                  new ( & m_union.o ) std::map< std::string, value >( r.m_union.o );
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         void destroy() noexcept
         {
            switch ( m_type ) {
               case json::type::NULL_:
               case json::type::BOOL_:
               case json::type::INT64:
               case json::type::DOUBLE:
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

         static void check_finite( const double d )
         {
            if ( ! std::isfinite( d ) ) {
               throw std::runtime_error( "non-finite double value illegal for json" );
            }
         }

      private:
         internal::value_union< value > m_union;
         json::type m_type;
      };

   } // json

} // tao

#endif
