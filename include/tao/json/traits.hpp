// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_HPP
#define TAOCPP_JSON_INCLUDE_TRAITS_HPP

#include <cstdint>

#include <map>
#include <string>
#include <vector>

#include "byte_view.hpp"
#include "type.hpp"

#include "internal/throw.hpp"

#include "external/byte.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class Traits >
      class basic_value;

      // note: traits< ... >::assign() is always called with needs_discard(v) == false

      template< typename T, typename = void >
      struct traits;

      template<>
      struct traits< null_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, null_t ) noexcept
         {
            v.unsafe_assign_null();
         }
      };

      template<>
      struct traits< bool >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const bool b ) noexcept
         {
            v.unsafe_assign_bool( b );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, bool& b )
         {
            b = v.get_boolean();
         }
      };

      namespace internal
      {
         template< template< typename... > class Traits, typename T >
         void unchecked_extract_number( const basic_value< Traits >& v, T& i )
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
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const signed char i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, signed char& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned char >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const unsigned char i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, unsigned char& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed short >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const signed short i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, signed short& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned short >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const unsigned short i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, unsigned short& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed int >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const signed int i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, signed int& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned int >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const unsigned int i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, unsigned int& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed long >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const signed long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, signed long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned long >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const unsigned long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, unsigned long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< signed long long >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const signed long long i ) noexcept
         {
            v.unsafe_assign_signed( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, signed long long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< unsigned long long >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const unsigned long long i ) noexcept
         {
            v.unsafe_assign_unsigned( i );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, unsigned long long& i )
         {
            internal::unchecked_extract_number( v, i );
         }
      };

      template<>
      struct traits< float >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const float f ) noexcept
         {
            v.unsafe_assign_double( f );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, float& f )
         {
            internal::unchecked_extract_number( v, f );
         }
      };

      template<>
      struct traits< double >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const double d ) noexcept
         {
            v.unsafe_assign_double( d );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, double& f )
         {
            internal::unchecked_extract_number( v, f );
         }
      };

      template<>
      struct traits< empty_binary_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, empty_binary_t ) noexcept
         {
            v.unsafe_emplace_binary();
         }
      };

      template<>
      struct traits< empty_array_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, empty_array_t ) noexcept
         {
            v.unsafe_emplace_array();
         }
      };

      template<>
      struct traits< empty_object_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, empty_object_t ) noexcept
         {
            v.unsafe_emplace_object();
         }
      };

      template<>
      struct traits< std::string >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const std::string& s )
         {
            v.unsafe_assign_string( s );
         }

         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, std::string&& s ) noexcept
         {
            v.unsafe_assign_string( std::move( s ) );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, std::string& s )
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
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const tao::string_view sv )
         {
            v.unsafe_emplace_string( sv.data(), sv.size() );
         }

         template< template< typename... > class Traits >
         static tao::string_view as( const basic_value< Traits >& v )
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
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const char* s )
         {
            v.unsafe_emplace_string( s );
         }

         template< template< typename... > class Traits >
         static const char* as( const basic_value< Traits >& v )
         {
            return v.get_string().c_str();
         }
      };

      template<>
      struct traits< const std::string& >
      {
         template< template< typename... > class Traits >
         static const std::string& as( const basic_value< Traits >& v )
         {
            return v.get_string();
         }
      };

      template<>
      struct traits< std::vector< tao::byte > >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const std::vector< tao::byte >& x )
         {
            v.unsafe_assign_binary( x );
         }

         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, std::vector< tao::byte >&& x ) noexcept
         {
            v.unsafe_assign_binary( std::move( x ) );
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, std::vector< tao::byte >& x )
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
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const tao::byte_view xv ) noexcept
         {
            v.unsafe_emplace_binary( xv.begin(), xv.end() );
         }

         template< template< typename... > class Traits >
         static tao::byte_view as( const basic_value< Traits >& v )
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
         template< template< typename... > class Traits >
         static const std::vector< tao::byte >& as( const basic_value< Traits >& v )
         {
            return v.get_binary();
         }
      };

      template< template< typename... > class Traits >
      struct traits< std::vector< basic_value< Traits > > >
      {
         static void assign( basic_value< Traits >& v, const std::vector< basic_value< Traits > >& a )
         {
            v.unsafe_assign_array( a );
         }

         static void assign( basic_value< Traits >& v, std::vector< basic_value< Traits > >&& a ) noexcept
         {
            v.unsafe_assign_array( std::move( a ) );
         }
      };

      template< template< typename... > class Traits >
      struct traits< std::map< std::string, basic_value< Traits > > >
      {
         static void assign( basic_value< Traits >& v, const std::map< std::string, basic_value< Traits > >& o )
         {
            v.unsafe_assign_object( std::move( o ) );
         }

         static void assign( basic_value< Traits >& v, std::map< std::string, basic_value< Traits > >&& o ) noexcept
         {
            v.unsafe_assign_object( std::move( o ) );
         }
      };

      template< template< typename... > class Traits >
      struct traits< const basic_value< Traits >* >
      {
         static void assign( basic_value< Traits >& v, const basic_value< Traits >* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( p );
         }
      };

      template< template< typename... > class Traits >
      struct traits< basic_value< Traits >* >
      {
         static void assign( basic_value< Traits >& v, const basic_value< Traits >* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( p );
         }
      };

      template<>
      struct traits< std::nullptr_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, std::nullptr_t ) noexcept
         {
            v.unsafe_assign_raw_ptr( nullptr );
         }
      };

      template< typename T >
      struct traits< tao::optional< T > >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, const tao::optional< T >& o )
         {
            if( o ) {
               v = *o;
            }
            else {
               v.unsafe_assign_null();
            }
         }

         template< template< typename... > class Traits >
         static void extract( const basic_value< Traits >& v, tao::optional< T >& o )
         {
            if( v.is_null() ) {
               o = tao::nullopt;
            }
            else {
               o = v.template as< T >();
            }
         }
      };

   }  // namespace json

}  // namespace tao

#endif
