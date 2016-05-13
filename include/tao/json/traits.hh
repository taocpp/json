// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_HH
#define TAOCPP_JSON_INCLUDE_TRAITS_HH

#include <string>
#include <vector>
#include <map>

#include "type.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      class basic_value;

      // note: traits< ... >::assign() is always called with needs_destroy(v) == false

      template< typename T, typename = void >
      struct traits
      {
         static_assert( sizeof( T ) == 0, "no traits specialization found" );

         template< typename V, typename U >
         static void assign( V &, U && );
      };

      template<>
      struct traits< null_t >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, null_t ) noexcept
         {
            v.unsafe_assign_null();
         }
      };

      template<>
      struct traits< bool >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const bool b ) noexcept
         {
            v.unsafe_assign_bool( b );
         }
      };

      template<>
      struct traits< signed char >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const signed char i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }
      };

      template<>
      struct traits< unsigned char >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const unsigned char i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }
      };

      template<>
      struct traits< signed short >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const signed short i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }
      };

      template<>
      struct traits< unsigned short >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const unsigned short i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }
      };

      template<>
      struct traits< signed int >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const signed int i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }
      };

      template<>
      struct traits< unsigned int >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const unsigned int i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }
      };

      template<>
      struct traits< signed long >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const signed long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }
      };

      template<>
      struct traits< unsigned long >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const unsigned long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }
      };

      template<>
      struct traits< signed long long >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const signed long long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }
      };

      template<>
      struct traits< unsigned long long >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const unsigned long long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }
      };

      template<>
      struct traits< float >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const float f )
         {
            v.unsafe_assign_double( f );
         }
      };

      template<>
      struct traits< double >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const double d )
         {
            v.unsafe_assign_double( d );
         }
      };

      template<>
      struct traits< empty_array_t >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, empty_array_t )
         {
            v.unsafe_emplace_array();
         }
      };

      template<>
      struct traits< empty_object_t >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, empty_object_t )
         {
            v.unsafe_emplace_object();
         }
      };

      template<>
      struct traits< std::string >
      {
         template< template< typename ... > class Traits, typename T >
         static void assign( basic_value< Traits > & v, T && s )
         {
            v.unsafe_emplace_string( std::forward< T >( s ) );
         }
      };

      template<>
      struct traits< const char * >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, const char * s )
         {
            v.unsafe_emplace_string( s );
         }
      };

      template< template< typename ... > class Traits >
      struct traits< std::vector< basic_value< Traits > > >
      {
         template< typename T >
         static void assign( basic_value< Traits > & v, T && a )
         {
            v.unsafe_emplace_array( std::forward< T >( a ) );
         }
      };

      template< template< typename ... > class Traits >
      struct traits< std::map< std::string, basic_value< Traits > > >
      {
         template< typename T >
         static void assign( basic_value< Traits > & v, T && o )
         {
            v.unsafe_emplace_object( std::forward< T >( o ) );
         }
      };

      template< template< typename ... > class Traits >
      struct traits< const basic_value< Traits > * >
      {
         static void assign( basic_value< Traits > & v, const basic_value< Traits > * p ) noexcept
         {
            v.unsafe_assign_pointer( p );
         }
      };

      template< template< typename ... > class Traits >
      struct traits< basic_value< Traits > * >
      {
         static void assign( basic_value< Traits > & v, const basic_value< Traits > * p ) noexcept
         {
            v.unsafe_assign_pointer( p );
         }
      };

      template<>
      struct traits< std::nullptr_t >
      {
         template< template< typename ... > class Traits >
         static void assign( basic_value< Traits > & v, std::nullptr_t ) noexcept
         {
            v.unsafe_assign_pointer( nullptr );
         }
      };

   } // json

} // tao

#endif
