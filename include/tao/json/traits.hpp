// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_HPP
#define TAOCPP_JSON_INCLUDE_TRAITS_HPP

#include <map>
#include <string>
#include <vector>

#include "byte_view.hpp"
#include "data.hpp"
#include "type.hpp"

#include "internal/throw.hpp"

#include "external/byte.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename B, template< typename... > class Traits >
         class proxy;

      }  // namespace internal

      // note: traits< ... >::assign() is always called with needs_discard(v) == false

      template< typename T, typename = void >
      struct traits
      {
         static_assert( sizeof( T ) == 0, "no traits specialization found" );

         template< typename U >
         static void assign( data&, U&& );
      };

      template<>
      struct traits< null_t >
      {
         static void assign( data& v, null_t ) noexcept
         {
            v.unsafe_assign_null();
         }
      };

      template<>
      struct traits< bool >
      {
         static void assign( data& v, const bool b ) noexcept
         {
            v.unsafe_assign_bool( b );
         }

         static void extract( const data& v, bool& b )
         {
            b = v.get_boolean();
         }
      };

      namespace internal
      {
         template< typename T >
         void unchecked_extract_number( const data& v, T& i )
         {
            switch( v.type() ) {
               case type::SIGNED:
                  i = v.unsafe_get_signed();
                  break;
               case type::UNSIGNED:
                  i = v.unsafe_get_unsigned();
                  break;
               case type::DOUBLE:
                  i = v.unsafe_get_double();
                  break;
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( v.type() );
            }
         }
      }

      template<>
      struct traits< signed char >
      {
         static void assign( data& v, const signed char i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         static void extract( const data& v, signed char& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned char >
      {
         static void assign( data& v, const unsigned char i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         static void extract( const data& v, unsigned char& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed short >
      {
         static void assign( data& v, const signed short i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         static void extract( const data& v, signed short& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned short >
      {
         static void assign( data& v, const unsigned short i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         static void extract( const data& v, unsigned short& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed int >
      {
         static void assign( data& v, const signed int i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         static void extract( const data& v, signed int& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned int >
      {
         static void assign( data& v, const unsigned int i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         static void extract( const data& v, unsigned int& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed long >
      {
         static void assign( data& v, const signed long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         static void extract( const data& v, signed long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned long >
      {
         static void assign( data& v, const unsigned long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         static void extract( const data& v, unsigned long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed long long >
      {
         static void assign( data& v, const signed long long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         static void extract( const data& v, signed long long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned long long >
      {
         static void assign( data& v, const unsigned long long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         static void extract( const data& v, unsigned long long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< float >
      {
         static void assign( data& v, const float f ) noexcept
         {
            v.unsafe_assign_double( f );
         }

         static void extract( const data& v, float& f )
         {
            internal::unchecked_extract_number( v, f );
         }
      };

      template<>
      struct traits< double >
      {
         static void assign( data& v, const double d ) noexcept
         {
            v.unsafe_assign_double( d );
         }

         static void extract( const data& v, double& f )
         {
            internal::unchecked_extract_number( v, f );
         }
      };

      template<>
      struct traits< empty_binary_t >
      {
         static void assign( data& v, empty_binary_t ) noexcept
         {
            v.unsafe_emplace_binary();
         }
      };

      template<>
      struct traits< empty_array_t >
      {
         static void assign( data& v, empty_array_t ) noexcept
         {
            v.unsafe_emplace_array();
         }
      };

      template<>
      struct traits< empty_object_t >
      {
         static void assign( data& v, empty_object_t ) noexcept
         {
            v.unsafe_emplace_object();
         }
      };

      template<>
      struct traits< std::string >
      {
         static void assign( data& v, const std::string& s )
         {
            v.unsafe_assign_string( s );
         }

         static void assign( data& v, std::string&& s ) noexcept
         {
            v.unsafe_assign_string( std::move( s ) );
         }

         static void extract( const data& v, std::string& s )
         {
            switch( v.type() ) {
               case type::STRING:
                  s = v.unsafe_get_string();
                  break;
               case type::STRING_VIEW: {
                  const auto sv = v.unsafe_get_string_view();
                  s.assign( sv.data(), sv.size() );
                  break;
               }
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( v.type() );
            }
         }
      };

      template<>
      struct traits< tao::string_view >
      {
         static void assign( data& v, const tao::string_view sv )
         {
            v.unsafe_emplace_string( sv.data(), sv.size() );
         }

         static tao::string_view as( const data& v )
         {
            switch( v.type() ) {
               case type::STRING:
                  return v.unsafe_get_string();
               case type::STRING_VIEW:
                  return v.unsafe_get_string_view();
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( v.type() );
            }
         }
      };

      template<>
      struct traits< const char* >
      {
         static void assign( data& v, const char* s )
         {
            v.unsafe_emplace_string( s );
         }

         static const char* as( const data& v )
         {
            return v.get_string().c_str();
         }
      };

      template<>
      struct traits< const std::string& >
      {
         static const std::string& as( const data& v )
         {
            return v.get_string();
         }
      };

      template<>
      struct traits< std::vector< tao::byte > >
      {
         static void assign( data& v, const std::vector< tao::byte >& x )
         {
            v.unsafe_assign_binary( x );
         }

         static void assign( data& v, std::vector< tao::byte >&& x ) noexcept
         {
            v.unsafe_assign_binary( std::move( x ) );
         }

         static void extract( const data& v, std::vector< tao::byte >& x )
         {
            switch( v.type() ) {
               case type::BINARY:
                  x = v.unsafe_get_binary();
                  break;
               case type::BINARY_VIEW: {
                  const auto xv = v.unsafe_get_binary_view();
                  x.assign( xv.begin(), xv.end() );
                  break;
               }
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( v.type() );
            }
         }
      };

      template<>
      struct traits< tao::byte_view >
      {
         static void assign( data& v, const tao::byte_view xv ) noexcept
         {
            v.unsafe_emplace_binary( xv.begin(), xv.end() );
         }

         static tao::byte_view as( const data& v )
         {
            switch( v.type() ) {
               case type::BINARY:
                  return v.unsafe_get_binary();
               case type::BINARY_VIEW:
                  return v.unsafe_get_binary_view();
               default:
                  TAOCPP_JSON_THROW_TYPE_ERROR( v.type() );
            }
         }
      };

      template<>
      struct traits< const std::vector< tao::byte >& >
      {
         static const std::vector< tao::byte >& as( const data& v )
         {
            return v.get_binary();
         }
      };

      template<>
      struct traits< std::vector< data > >
      {
         static void assign( data& v, const std::vector< data >& a )
         {
            v.unsafe_assign_array( a );
         }

         static void assign( data& v, std::vector< data >&& a ) noexcept
         {
            v.unsafe_assign_array( std::move( a ) );
         }
      };

      template<>
      struct traits< std::map< std::string, data > >
      {
         static void assign( data& v, const std::map< std::string, data >& o )
         {
            v.unsafe_assign_object( std::move( o ) );
         }

         static void assign( data& v, std::map< std::string, data >&& o ) noexcept
         {
            v.unsafe_assign_object( std::move( o ) );
         }
      };

      template<>
      struct traits< const data* >
      {
         static void assign( data& v, const data* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( p );
         }
      };

      template<>
      struct traits< data* >
      {
         static void assign( data& v, const data* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( p );
         }
      };

      template< typename B, template< typename... > class Traits >
      struct traits< const internal::proxy< B, Traits >* >
      {
         static void assign( data& v, const internal::proxy< B, Traits >* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( &static_cast< const data& >( *p ) );
         }
      };

      template< typename B, template< typename... > class Traits >
      struct traits< internal::proxy< B, Traits >* >
      {
         static void assign( data& v, const internal::proxy< B, Traits >* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( &static_cast< const data& >( *p ) );
         }
      };

      template<>
      struct traits< std::nullptr_t >
      {
         static void assign( data& v, std::nullptr_t ) noexcept
         {
            v.unsafe_assign_raw_ptr( nullptr );
         }
      };

      template< typename T >
      struct traits< tao::optional< T > >
      {
         template< typename B, template< typename... > class Traits >
         static void assign( internal::proxy< B, Traits >& v, const tao::optional< T >& o )
         {
            if( o ) {
               v = *o;
            }
            else {
               v.unsafe_assign_null();
            }
         }

         template< typename B, template< typename... > class Traits >
         static void extract( const internal::proxy< B, Traits >& v, tao::optional< T >& o )
         {
            if( v.is_null() ) {
               o = nullopt;
            }
            else {
               o = v.template as< T >();
            }
         }
      };

   }  // namespace json

}  // namespace tao

#endif
