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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, null_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->is_null();
            }
            else {
               return true;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, null_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->type() < type::NULL_;
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, null_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->type() > type::NULL_;
            }
            else {
               return false;
            }
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const bool rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->is_boolean() && ( p->unsafe_get_boolean() == rhs );
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const bool rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return ( p->type() < type::BOOLEAN ) || ( p->is_boolean() && ( p->unsafe_get_boolean() < rhs ) );
            }
            else {
               return type::NULL_ < type::BOOLEAN;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const bool rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return ( p->type() > type::BOOLEAN ) || ( p->is_boolean() && ( p->unsafe_get_boolean() > rhs ) );
            }
            else {
               return type::NULL_ > type::BOOLEAN;
            }
         }
      };

      namespace internal
      {
         template< typename T >
         struct number_trait
         {
            template< template< typename... > class Traits >
            static void extract( const basic_value< Traits >& v, T& i )
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
         };

         template< typename T >
         struct signed_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits >
            static void assign( basic_value< Traits >& v, const T i ) noexcept
            {
               v.unsafe_assign_signed( i );
            }

            template< template< typename... > class Traits >
            static bool equal( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() == rhs;
                     case type::UNSIGNED:
                        return ( rhs >= 0 ) && ( p->unsafe_get_unsigned() == static_cast< std::uint64_t >( rhs ) );
                     case type::DOUBLE:
                        return p->unsafe_get_double() == rhs;
                     default:
                        return false;
                  }
               }
               else {
                  return false;
               }
            }

            template< template< typename... > class Traits >
            static bool less_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() < rhs;
                     case type::UNSIGNED:
                        return ( rhs >= 0 ) && ( p->unsafe_get_unsigned() < static_cast< std::uint64_t >( rhs ) );
                     case type::DOUBLE:
                        return p->unsafe_get_double() < rhs;
                     default:
                        return p->type() < type::SIGNED;
                  }
               }
               else {
                  return type::NULL_ < type::SIGNED;
               }
            }

            template< template< typename... > class Traits >
            static bool greater_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() > rhs;
                     case type::UNSIGNED:
                        return ( rhs < 0 ) || ( p->unsafe_get_unsigned() > static_cast< std::uint64_t >( rhs ) );
                     case type::DOUBLE:
                        return p->unsafe_get_double() > rhs;
                     default:
                        return p->type() > type::SIGNED;
                  }
               }
               else {
                  return type::NULL_ > type::SIGNED;
               }
            }
         };

         template< typename T >
         struct unsigned_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits >
            static void assign( basic_value< Traits >& v, const T i ) noexcept
            {
               v.unsafe_assign_unsigned( i );
            }

            template< template< typename... > class Traits >
            static bool equal( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED: {
                        const auto v = p->unsafe_get_signed();
                        return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs );
                     }
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() == rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() == rhs;
                     default:
                        return false;
                  }
               }
               else {
                  return false;
               }
            }

            template< template< typename... > class Traits >
            static bool less_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED: {
                        const auto v = p->unsafe_get_signed();
                        return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs );
                     }
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() < rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() < rhs;
                     default:
                        return p->type() < type::UNSIGNED;
                  }
               }
               else {
                  return type::NULL_ < type::UNSIGNED;
               }
            }

            template< template< typename... > class Traits >
            static bool greater_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED: {
                        const auto v = p->unsafe_get_signed();
                        return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) > rhs );
                     }
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() > rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() > rhs;
                     default:
                        return p->type() > type::UNSIGNED;
                  }
               }
               else {
                  return type::NULL_ > type::UNSIGNED;
               }
            }
         };

         template< typename T >
         struct float_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits >
            static void assign( basic_value< Traits >& v, const T f ) noexcept
            {
               v.unsafe_assign_double( f );
            }

            template< template< typename... > class Traits >
            static bool equal( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() == rhs;
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() == rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() == rhs;
                     default:
                        return false;
                  }
               }
               else {
                  return false;
               }
            }

            template< template< typename... > class Traits >
            static bool less_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() < rhs;
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() < rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() < rhs;
                     default:
                        return p->type() < type::DOUBLE;
                  }
               }
               else {
                  return type::NULL_ < type::DOUBLE;
               }
            }

            template< template< typename... > class Traits >
            static bool greater_than( const basic_value< Traits >& lhs, const T rhs ) noexcept
            {
               if( const auto* p = lhs.skip_raw_ptr() ) {
                  switch( p->type() ) {
                     case type::SIGNED:
                        return p->unsafe_get_signed() > rhs;
                     case type::UNSIGNED:
                        return p->unsafe_get_unsigned() > rhs;
                     case type::DOUBLE:
                        return p->unsafe_get_double() > rhs;
                     default:
                        return p->type() > type::DOUBLE;
                  }
               }
               else {
                  return type::NULL_ > type::DOUBLE;
               }
            }
         };
      }

      // clang-format off
      template<> struct traits< signed char > : internal::signed_trait< signed char > {};
      template<> struct traits< signed short > : internal::signed_trait< signed short > {};
      template<> struct traits< signed int > : internal::signed_trait< signed int > {};
      template<> struct traits< signed long > : internal::signed_trait< signed long > {};
      template<> struct traits< signed long long > : internal::signed_trait< signed long long > {};

      template<> struct traits< unsigned char > : internal::unsigned_trait< unsigned char > {};
      template<> struct traits< unsigned short > : internal::unsigned_trait< unsigned short > {};
      template<> struct traits< unsigned int > : internal::unsigned_trait< unsigned int > {};
      template<> struct traits< unsigned long > : internal::unsigned_trait< unsigned long > {};
      template<> struct traits< unsigned long long > : internal::unsigned_trait< unsigned long long > {};

      template<> struct traits< float > : internal::float_trait< float > {};
      template<> struct traits< double > : internal::float_trait< double > {};
      // clang-format on

      template<>
      struct traits< empty_binary_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, empty_binary_t ) noexcept
         {
            v.unsafe_emplace_binary();
         }

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, empty_binary_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary().empty();
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view().empty();
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, empty_binary_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                  case type::BINARY_VIEW:
                     return false;
                  default:
                     return p->type() < type::BINARY;
               }
            }
            else {
               return type::NULL_ < type::BINARY;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, empty_binary_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return !p->unsafe_get_binary().empty();
                  case type::BINARY_VIEW:
                     return !p->unsafe_get_binary_view().empty();
                  default:
                     return p->type() > type::BINARY;
               }
            }
            else {
               return type::NULL_ > type::BINARY;
            }
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, empty_array_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->is_array() && p->unsafe_get_array().empty();
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, empty_array_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->type() < type::ARRAY;
            }
            else {
               return type::NULL_ < type::ARRAY;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, empty_array_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return ( p->type() > type::ARRAY ) || ( p->is_array() && !p->unsafe_get_array().empty() );
            }
            else {
               return type::NULL_ > type::ARRAY;
            }
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, empty_object_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->is_object() && p->unsafe_get_object().empty();
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, empty_object_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return p->type() < type::OBJECT;
            }
            else {
               return type::NULL_ < type::OBJECT;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, empty_object_t ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               return ( p->type() > type::OBJECT ) || ( p->is_object() && !p->unsafe_get_object().empty() );
            }
            else {
               return type::NULL_ > type::OBJECT;
            }
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const std::string& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() == rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() == rhs;
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const std::string& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() < rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() < rhs;
                  default:
                     return p->type() < type::STRING;
               }
            }
            else {
               return type::NULL_ < type::STRING;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const std::string& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() > rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() > rhs;
                  default:
                     return p->type() > type::STRING;
               }
            }
            else {
               return type::NULL_ > type::STRING;
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const tao::string_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() == rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() == rhs;
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const tao::string_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() < rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() < rhs;
                  default:
                     return p->type() < type::STRING;
               }
            }
            else {
               return type::NULL_ < type::STRING;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const tao::string_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() > rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() > rhs;
                  default:
                     return p->type() > type::STRING;
               }
            }
            else {
               return type::NULL_ > type::STRING;
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const char* rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() == rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() == rhs;
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const char* rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() < rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() < rhs;
                  default:
                     return p->type() < type::STRING;
               }
            }
            else {
               return type::NULL_ < type::STRING;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const char* rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::STRING:
                     return p->unsafe_get_string() > rhs;
                  case type::STRING_VIEW:
                     return p->unsafe_get_string_view() > rhs;
                  default:
                     return p->type() > type::STRING;
               }
            }
            else {
               return type::NULL_ > type::STRING;
            }
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() == rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() == rhs;
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() < rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() < rhs;
                  default:
                     return p->type() < type::BINARY;
               }
            }
            else {
               return type::NULL_ < type::BINARY;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() > rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() > rhs;
                  default:
                     return p->type() > type::BINARY;
               }
            }
            else {
               return type::NULL_ > type::BINARY;
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const tao::byte_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() == rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() == rhs;
                  default:
                     return false;
               }
            }
            else {
               return false;
            }
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const tao::byte_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() < rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() < rhs;
                  default:
                     return p->type() < type::BINARY;
               }
            }
            else {
               return type::NULL_ < type::BINARY;
            }
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const tao::byte_view rhs ) noexcept
         {
            if( const auto* p = lhs.skip_raw_ptr() ) {
               switch( p->type() ) {
                  case type::BINARY:
                     return p->unsafe_get_binary() > rhs;
                  case type::BINARY_VIEW:
                     return p->unsafe_get_binary_view() > rhs;
                  default:
                     return p->type() > type::BINARY;
               }
            }
            else {
               return type::NULL_ > type::BINARY;
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

         template< template< typename... > class TraitsL >
         static bool equal( const basic_value< TraitsL >& lhs, const basic_value< Traits >* rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == null );
         }

         template< template< typename... > class TraitsL >
         static bool less_than( const basic_value< TraitsL >& lhs, const basic_value< Traits >* rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < null );
         }

         template< template< typename... > class TraitsL >
         static bool greater_than( const basic_value< TraitsL >& lhs, const basic_value< Traits >* rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > null );
         }
      };

      template< template< typename... > class Traits >
      struct traits< basic_value< Traits >* >
         : traits< const basic_value< Traits >* >
      {
      };

      template<>
      struct traits< std::nullptr_t >
      {
         template< template< typename... > class Traits >
         static void assign( basic_value< Traits >& v, std::nullptr_t ) noexcept
         {
            v.unsafe_assign_raw_ptr( nullptr );
         }

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, std::nullptr_t ) noexcept
         {
            return lhs == null;
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, std::nullptr_t ) noexcept
         {
            return lhs < null;
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, std::nullptr_t ) noexcept
         {
            return lhs > null;
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

         template< template< typename... > class Traits >
         static bool equal( const basic_value< Traits >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == null );
         }

         template< template< typename... > class Traits >
         static bool less_than( const basic_value< Traits >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < null );
         }

         template< template< typename... > class Traits >
         static bool greater_than( const basic_value< Traits >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > null );
         }
      };

   }  // namespace json

}  // namespace tao

#endif
