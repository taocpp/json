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
      class value;

      template< typename T, typename = void >
      struct traits
      {
         static_assert( sizeof( T ) == 0, "no traits specialization found" );

         template< typename U >
         static void assign( value &, U && );
      };

      namespace internal
      {
         template< typename T >
         const char* no_automatic_key_defined()
         {
            static_assert( sizeof( T ) == 0, "no default key defined for T" );
            return nullptr;
         }
      }

      template< typename >
      struct default_key
      {
         static const char* value;
      };

      template< typename T >
      const char* default_key< T >::value = internal::no_automatic_key_defined< T >();

      namespace internal
      {
         template< typename T >
         struct pair
         {
            mutable std::pair< std::string, T > e;

            template< typename U >
            pair( U && v ) : e( default_key< typename std::decay< U >::type >::value, std::forward< U >( v ) ) {}

            pair( std::string k, const T & v ) : e( std::move( k ), v ) {}
            pair( std::string k, T && v ) : e( std::move( k ), std::move( v ) ) {}
         };

         template< typename T, typename U, type E >
         struct totally_ordered
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T& lhs, const U& rhs )
            {
               if( lhs.type() == type::POINTER ) {
                  return *lhs.get_pointer() == rhs;
               }
               return ( lhs.type() == E ) && ( lhs.T::template get_by_enum< E >() == rhs );
            }

            friend bool operator<( const T& lhs, const U& rhs )
            {
               if( lhs.type() == type::POINTER ) {
                  return *lhs.get_pointer() < rhs;
               }
               return ( lhs.type() < E ) || ( ( lhs.type() == E ) && ( lhs.T::template get_by_enum< E >() < rhs ) );
            }

            friend bool operator>( const T& lhs, const U& rhs )
            {
               if( lhs.type() == type::POINTER ) {
                  return *lhs.get_pointer() > rhs;
               }
               return ( lhs.type() > E ) || ( ( lhs.type() == E ) && ( lhs.T::template get_by_enum< E >() > rhs ) );
            }
         };
      }

      class value
         : operators::totally_ordered< value >,
           internal::totally_ordered< value, std::nullptr_t, type::NULL_ >, // null
           internal::totally_ordered< value, bool, type::BOOL_ >, // bool
           internal::totally_ordered< value, signed char, type::INT64 >, // int64
           internal::totally_ordered< value, unsigned char, type::INT64 >, // int64
           internal::totally_ordered< value, signed short, type::INT64 >, // int64
           internal::totally_ordered< value, unsigned short, type::INT64 >, // int64
           internal::totally_ordered< value, signed int, type::INT64 >, // int64
           internal::totally_ordered< value, unsigned int, type::INT64 >, // int64
           internal::totally_ordered< value, signed long, type::INT64 >, // int64
           internal::totally_ordered< value, signed long long, type::INT64 >, // int64
           internal::totally_ordered< value, double, type::DOUBLE >, // double
           internal::totally_ordered< value, std::string, type::STRING >, // string
           internal::totally_ordered< value, const char*, type::STRING >, // string
           internal::totally_ordered< value, std::vector< value >, type::ARRAY >, // array
           internal::totally_ordered< value, std::map< std::string, value >, type::OBJECT > // object
      {
      public:
         friend struct traits< std::nullptr_t >;
         friend struct traits< bool >;
         friend struct traits< signed char >;
         friend struct traits< unsigned char >;
         friend struct traits< signed short >;
         friend struct traits< unsigned short >;
         friend struct traits< signed int >;
         friend struct traits< unsigned int >;
         friend struct traits< signed long >;
         friend struct traits< signed long long >;
         friend struct traits< double >;
         friend struct traits< empty_array_t >;
         friend struct traits< empty_object_t >;
         friend struct traits< std::string >;
         friend struct traits< const char * >;
         friend struct traits< std::vector< value > >;
         friend struct traits< std::map< std::string, value > >;
         friend struct traits< const value * >;

         value() noexcept
         { }

         value( value && r ) noexcept
         {
            seize( std::move( r ) );
            m_type = r.m_type;
         }

         value( const value & r )
         {
            embed( r );
            m_type = r.m_type;
         }

         value( value & r )
         {
            embed( r );
            m_type = r.m_type;
         }

         template< typename T >
         value( T && v ) // TODO: noexcept( noexcept( unsafe_assign( std::forward< T >( v ) ) ) )
         {
            unsafe_assign( std::forward< T >( v ) );
         }

         value( const std::initializer_list< internal::pair< value > > & l )
         {
            unsafe_assign( l );
         }

         value( std::initializer_list< internal::pair< value > > && l )
         {
            unsafe_assign( std::move( l ) );
         }

         template< typename... Ts >
         static value array( Ts && ... ts )
         {
            value v;
            v.unsafe_emplace_array( std::vector< value >( { std::forward< Ts >( ts )... } ) );
            return v;
         }

         ~value() noexcept
         {
            unsafe_destroy();
         }

         template< typename T >
         value & operator= ( T && v ) // TODO: noexcept( noexcept( assign( std::forward< T >( v ) ) ) )
         {
            assign( std::forward< T >( v ) );
            return *this;
         }

         value & operator= ( const std::initializer_list< internal::pair< value > > & l )
         {
            assign( l );
            return *this;
         }

         value & operator= ( std::initializer_list< internal::pair< value > > && l )
         {
            assign( std::move( l ) );
            return *this;
         }

         value & operator= ( value && r ) noexcept
         {
            if ( this != & r ) {
               destroy();
               seize( std::move( r ) );
               m_type = r.m_type;
            }
            return * this;
         }

         value & operator= ( const value & r )
         {
            if ( this != & r ) {
               destroy();
               embed( r );
               m_type = r.m_type;
            }
            return * this;
         }

         value & operator= ( value & r )
         {
            if ( this != & r ) {
               destroy();
               embed( r );
               m_type = r.m_type;
            }
            return * this;
         }

         template< typename T >
         void assign( T && v ) // TODO: noexcept( noexcept( unsafe_assign( std::forward< T >( v ) ) ) )
         {
            destroy();
            unsafe_assign( std::forward< T >( v ) );
         }

         void assign( const std::initializer_list< internal::pair< value > > & l )
         {
            destroy();
            unsafe_assign( l );
         }

         void assign( std::initializer_list< internal::pair< value > > && l )
         {
            destroy();
            unsafe_assign( std::move( l ) );
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

         bool is_pointer() const noexcept
         {
            return m_type == json::type::POINTER;
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

         const value * get_pointer() const noexcept
         {
            CHECK_TYPE_ERROR( m_type, json::type::POINTER );
            return unsafe_pointer();
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

         const value * unsafe_pointer() const noexcept
         {
            return m_union.p;
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
            if( m_type == json::type::POINTER ) {
              return (*unsafe_pointer())[ index ];
            }
            CHECK_TYPE_ERROR( m_type, json::type::ARRAY );
            return m_union.a.at( index );
         }

         const value & operator[] ( const std::string & index ) const
         {
            if( m_type == json::type::POINTER ) {
              return (*unsafe_pointer())[ index ];
            }
            CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            return m_union.o.at( index );
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
                  unsafe_emplace_object();
               case json::type::OBJECT:
                  m_union.o.emplace( std::forward< K >( k ), std::forward< V >( v ) );
                  break;
               default:
                  THROW_TYPE_ERROR( m_type );
            }
         }

         template< typename ... Ts >
         void emplace_array( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_array( std::forward< Ts >( ts ) ... );
         }

         template< typename ... Ts >
         void emplace_object( Ts && ... ts )
         {
            destroy();
            unsafe_emplace_object( std::forward< Ts >( ts ) ... );
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_destroy( v.type() ) is true!

         template< typename T >
         void unsafe_assign( T && v ) // TODO: noexcept( noexcept( traits< typename std::decay_t< T >::type >::assign( *this, std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( const std::initializer_list< internal::pair< value > > & l )
         {
            unsafe_emplace_object();
            for( auto & e : l ) {
              const auto r = m_union.o.emplace( e.e.first, e.e.second );
              if( !r.second ) {
                throw std::runtime_error( "duplicate key detected: " + r.first->first );
              }
            }
         }

         void unsafe_assign( std::initializer_list< internal::pair< value > > && l )
         {
            unsafe_emplace_object();
            for( auto & e : l ) {
              const auto r = m_union.o.emplace( std::move( e.e.first ), std::move( e.e.second ) );
              if( !r.second ) {
                throw std::runtime_error( "duplicate key detected: " + r.first->first );
              }
            }
         }

         template< typename ... Ts >
         void unsafe_emplace_array( Ts && ... ts )
         {
            new ( & m_union.a ) std::vector< value >( std::forward< Ts >( ts ) ... );
            m_type = json::type::ARRAY;
         }

         template< typename ... Ts >
         void unsafe_emplace_object( Ts && ... ts )
         {
            new ( & m_union.o ) std::map< std::string, value >( std::forward< Ts >( ts ) ... );
            m_type = json::type::OBJECT;
         }

      private:
         void seize( value && r ) noexcept
         {
            switch ( r.m_type ) {
               case json::type::NULL_:
                  return;
               case json::type::BOOL_:
               case json::type::INT64:
               case json::type::DOUBLE:
               case json::type::POINTER:
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
                  return;
               case json::type::BOOL_:
               case json::type::INT64:
               case json::type::DOUBLE:
               case json::type::POINTER:
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
            unsafe_destroy();
            m_type = json::type::NULL_;
         }

         void unsafe_destroy() noexcept
         {
            switch ( m_type ) {
               case json::type::NULL_:
               case json::type::BOOL_:
               case json::type::INT64:
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

      private:
         internal::value_union< value > m_union;
         json::type m_type = json::type::NULL_;
      };

      inline bool operator< ( const value & lhs, const value & rhs )
      {
         if ( lhs.type() == type::POINTER ) {
           return *lhs.unsafe_pointer() < rhs;
         }
         if ( rhs.type() == type::POINTER ) {
           return lhs < *rhs.unsafe_pointer();
         }
         if ( lhs.type() != rhs.type() ) {
            return lhs.type() < rhs.type();
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return false;
            case type::BOOL_:
               return lhs.unsafe_bool() < rhs.unsafe_bool();
            case type::INT64:
               return lhs.unsafe_int64() < rhs.unsafe_int64();
            case type::DOUBLE:
               return lhs.unsafe_double() < rhs.unsafe_double();
            case type::STRING:
               return lhs.unsafe_string() < rhs.unsafe_string();
            case type::ARRAY:
               return lhs.unsafe_array() < rhs.unsafe_array();
            case type::OBJECT:
               return lhs.unsafe_object() < rhs.unsafe_object();
            case type::POINTER:
               break;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      inline bool operator== ( const value & lhs, const value & rhs )
      {
         if ( lhs.type() == type::POINTER ) {
           return *lhs.unsafe_pointer() == rhs;
         }
         if ( rhs.type() == type::POINTER ) {
           return lhs == *rhs.unsafe_pointer();
         }
         if ( lhs.type() != rhs.type() ) {
            return false;
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return true;
            case type::BOOL_:
               return lhs.unsafe_bool() == rhs.unsafe_bool();
            case type::INT64:
               return lhs.unsafe_int64() == rhs.unsafe_int64();
            case type::DOUBLE:
               return lhs.unsafe_double() == rhs.unsafe_double();
            case type::STRING:
               return lhs.unsafe_string() == rhs.unsafe_string();
            case type::ARRAY:
               return lhs.unsafe_array() == rhs.unsafe_array();
            case type::OBJECT:
               return lhs.unsafe_object() == rhs.unsafe_object();
            case type::POINTER:
               break;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      template<>
      struct traits< std::nullptr_t >
      {
         static void assign( value & v, std::nullptr_t )
         {
            v.m_type = json::type::NULL_;
         }
      };

      template<>
      struct traits< bool >
      {
         static void assign( value & v, const bool b )
         {
            v.m_union.b = b;
            v.m_type = json::type::BOOL_;
         }
      };

      template<>
      struct traits< signed char >
      {
         static void assign( value & v, const signed char i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< unsigned char >
      {
         static void assign( value & v, const unsigned char i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< signed short >
      {
         static void assign( value & v, const signed short i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< unsigned short >
      {
         static void assign( value & v, const unsigned short i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< signed int >
      {
         static void assign( value & v, const signed int i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< unsigned int >
      {
         static void assign( value & v, const unsigned int i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< signed long >
      {
         static void assign( value & v, const signed long i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< signed long long >
      {
         static void assign( value & v, const signed long long i )
         {
            v.m_union.i = i;
            v.m_type = json::type::INT64;
         }
      };

      template<>
      struct traits< double >
      {
         static void assign( value & v, const double d )
         {
            if ( ! std::isfinite( d ) ) {
               throw std::runtime_error( "non-finite double value illegal for json" );
            }
            v.m_union.d = d;
            v.m_type = json::type::DOUBLE;
         }
      };

      template<>
      struct traits< empty_array_t >
      {
         static void assign( value & v, empty_array_t )
         {
            new ( & v.m_union.a ) std::vector< value >();
            v.m_type = json::type::ARRAY;
         }
      };

      template<>
      struct traits< empty_object_t >
      {
         static void assign( value & v, empty_object_t )
         {
            new ( & v.m_union.o ) std::map< std::string, value >();
            v.m_type = json::type::OBJECT;
         }
      };

      template<>
      struct traits< std::string >
      {
         template< typename T >
         static void assign( value & v, T && s )
         {
            new ( & v.m_union.s ) std::string( std::forward< T >( s ) );
            v.m_type = json::type::STRING;
         }
      };

      template<>
      struct traits< const char * >
      {
         static void assign( value & v, const char * s )
         {
            new ( & v.m_union.s ) std::string( s );
            v.m_type = json::type::STRING;
         }
      };

      template<>
      struct traits< std::vector< value > >
      {
         template< typename T >
         static void assign( value & v, T && a )
         {
            v.unsafe_emplace_array( std::forward< T >( a ) );
         }
      };

      template<>
      struct traits< std::map< std::string, value > >
      {
         template< typename T >
         static void assign( value & v, T && o )
         {
            v.unsafe_emplace_object( std::forward< T >( o ) );
         }
      };

      template<>
      struct traits< const value * >
      {
         static void assign( value & v, const value * p )
         {
            v.m_union.p = p;
            v.m_type = json::type::POINTER;
         }
      };

   } // json

} // tao

#endif
