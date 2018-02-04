// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_HPP
#define TAOCPP_JSON_INCLUDE_TRAITS_HPP

#include <cstdint>

#include <map>
#include <string>
#include <vector>

#include "byte_view.hpp"
#include "forward.hpp"
#include "type.hpp"

#include "events/from_value.hpp"

#include "external/byte.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

namespace tao
{
   namespace json
   {
      // note: traits< ... >::assign() is always called with needs_discard(v) == false

      template<>
      struct traits< null_t >
      {
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, null_t /*unused*/ ) noexcept
         {
            v.unsafe_assign_null();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_raw_ptr()->is_null();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_raw_ptr()->type() < type::NULL_;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_raw_ptr()->type() > type::NULL_;
         }
      };

      template<>
      struct traits< bool >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const bool b )
         {
            c.boolean( b );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const bool b ) noexcept
         {
            v.unsafe_assign_bool( b );
         }

         template< template< typename... > class Traits, typename Base >
         static void extract( const basic_value< Traits, Base >& v, bool& b )
         {
            b = v.get_boolean();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return p->is_boolean() && ( p->unsafe_get_boolean() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return ( p->type() < type::BOOLEAN ) || ( p->is_boolean() && ( p->unsafe_get_boolean() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return ( p->type() > type::BOOLEAN ) || ( p->is_boolean() && ( p->unsafe_get_boolean() > rhs ) );
         }
      };

      namespace internal
      {
         template< typename T >
         struct number_trait
         {
            template< template< typename... > class Traits, typename Base >
            static void extract( const basic_value< Traits, Base >& v, T& n )
            {
               switch( v.type() ) {
                  case type::SIGNED:
                     n = static_cast< T >( v.unsafe_get_signed() );
                     break;
                  case type::UNSIGNED:
                     n = static_cast< T >( v.unsafe_get_unsigned() );
                     break;
                  case type::DOUBLE:
                     n = static_cast< T >( v.unsafe_get_double() );
                     break;
                  default:
                     v.throw_invalid_json_type();
               }
            }
         };

         template< typename T >
         struct signed_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T i )
            {
               c.number( std::int64_t( i ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T i ) noexcept
            {
               v.unsafe_assign_signed( i );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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
         };

         template< typename T >
         struct unsigned_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T i )
            {
               c.number( std::uint64_t( i ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T i ) noexcept
            {
               v.unsafe_assign_unsigned( i );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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
         };

         template< typename T >
         struct float_trait
            : number_trait< T >
         {
            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T f )
            {
               c.number( double( f ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T f ) noexcept
            {
               v.unsafe_assign_double( f );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_raw_ptr();
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
         };

      }  // namespace internal

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
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, empty_binary_t /*unused*/ ) noexcept
         {
            v.unsafe_emplace_binary();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, empty_binary_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary().empty();
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view().empty();
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_binary_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
               case type::BINARY_VIEW:
                  return false;
               default:
                  return p->type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_binary_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return !p->unsafe_get_binary().empty();
               case type::BINARY_VIEW:
                  return !p->unsafe_get_binary_view().empty();
               default:
                  return p->type() > type::BINARY;
            }
         }
      };

      template<>
      struct traits< empty_array_t >
      {
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, empty_array_t /*unused*/ ) noexcept
         {
            v.unsafe_emplace_array();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, empty_array_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return p->is_array() && p->unsafe_get_array().empty();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_array_t /*unused*/ ) noexcept
         {
            return lhs.skip_raw_ptr()->type() < type::ARRAY;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_array_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return ( p->type() > type::ARRAY ) || ( p->is_array() && !p->unsafe_get_array().empty() );
         }
      };

      template<>
      struct traits< empty_object_t >
      {
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, empty_object_t /*unused*/ ) noexcept
         {
            v.unsafe_emplace_object();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, empty_object_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return p->is_object() && p->unsafe_get_object().empty();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_object_t /*unused*/ ) noexcept
         {
            return lhs.skip_raw_ptr()->type() < type::OBJECT;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_object_t /*unused*/ ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            return ( p->type() > type::OBJECT ) || ( p->is_object() && !p->unsafe_get_object().empty() );
         }
      };

      template<>
      struct traits< std::string >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::string& s )
         {
            c.string( s );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, std::string&& s )
         {
            c.string( std::move( s ) );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::string& s )
         {
            v.unsafe_assign_string( s );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, std::string&& s ) noexcept
         {
            v.unsafe_assign_string( std::move( s ) );
         }

         template< template< typename... > class Traits, typename Base >
         static void extract( const basic_value< Traits, Base >& v, std::string& s )
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
                  v.throw_invalid_json_type();
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() < rhs;
               default:
                  return p->type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() > rhs;
               default:
                  return p->type() > type::STRING;
            }
         }
      };

      template<>
      struct traits< tao::string_view >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const tao::string_view sv )
         {
            c.string( sv );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const tao::string_view sv )
         {
            v.unsafe_emplace_string( sv.data(), sv.size() );
         }

         template< template< typename... > class Traits, typename Base >
         static tao::string_view as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::STRING:
                  return v.unsafe_get_string();
               case type::STRING_VIEW:
                  return v.unsafe_get_string_view();
               default:
                  v.throw_invalid_json_type();
            }
            throw std::runtime_error( "code should be unreachable" );  // NOLINT, LCOV_EXCL_LINE
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() < rhs;
               default:
                  return p->type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() > rhs;
               default:
                  return p->type() > type::STRING;
            }
         }
      };

      template<>
      struct traits< const char* >
      {
         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const char* s )
         {
            v.unsafe_emplace_string( s );
         }

         template< template< typename... > class Traits, typename Base >
         static const char* as( const basic_value< Traits, Base >& v )
         {
            return v.get_string().c_str();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() < rhs;
               default:
                  return p->type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::STRING:
                  return p->unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p->unsafe_get_string_view() > rhs;
               default:
                  return p->type() > type::STRING;
            }
         }
      };

      template<>
      struct traits< const std::string& >
      {
         template< template< typename... > class Traits, typename Base >
         static const std::string& as( const basic_value< Traits, Base >& v )
         {
            return v.get_string();
         }
      };

      template<>
      struct traits< std::vector< tao::byte > >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::vector< tao::byte >& x )
         {
            c.binary( x );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, std::vector< tao::byte >&& x )
         {
            c.binary( std::move( x ) );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::vector< tao::byte >& x )
         {
            v.unsafe_assign_binary( x );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, std::vector< tao::byte >&& x ) noexcept
         {
            v.unsafe_assign_binary( std::move( x ) );
         }

         template< template< typename... > class Traits, typename Base >
         static void extract( const basic_value< Traits, Base >& v, std::vector< tao::byte >& x )
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
                  v.throw_invalid_json_type();
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() == rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() < rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() < rhs;
               default:
                  return p->type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() > rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() > rhs;
               default:
                  return p->type() > type::BINARY;
            }
         }
      };

      template<>
      struct traits< tao::byte_view >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const tao::byte_view xv )
         {
            c.binary( xv );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const tao::byte_view xv ) noexcept
         {
            v.unsafe_emplace_binary( xv.begin(), xv.end() );
         }

         template< template< typename... > class Traits, typename Base >
         static tao::byte_view as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::BINARY:
                  return v.unsafe_get_binary();
               case type::BINARY_VIEW:
                  return v.unsafe_get_binary_view();
               default:
                  v.throw_invalid_json_type();
            }
            throw std::runtime_error( "code should be unreachable" );  // NOLINT, LCOV_EXCL_LINE
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const tao::byte_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() == rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const tao::byte_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() < rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() < rhs;
               default:
                  return p->type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const tao::byte_view rhs ) noexcept
         {
            const auto* p = lhs.skip_raw_ptr();
            switch( p->type() ) {
               case type::BINARY:
                  return p->unsafe_get_binary() > rhs;
               case type::BINARY_VIEW:
                  return p->unsafe_get_binary_view() > rhs;
               default:
                  return p->type() > type::BINARY;
            }
         }
      };

      template<>
      struct traits< const std::vector< tao::byte >& >
      {
         template< template< typename... > class Traits, typename Base >
         static const std::vector< tao::byte >& as( const basic_value< Traits, Base >& v )
         {
            return v.get_binary();
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< std::vector< basic_value< Traits, Base > > >
      {
         template< template< typename... > class, typename Consumer >
         static void produce( Consumer& c, const std::vector< basic_value< Traits, Base > >& a )
         {
            c.begin_array( a.size() );
            for( const auto& i : a ) {
               events::from_value( c, i );
               c.element();
            }
            c.end_array( a.size() );
         }

         static void assign( basic_value< Traits, Base >& v, const std::vector< basic_value< Traits, Base > >& a )
         {
            v.unsafe_assign_array( a );
         }

         static void assign( basic_value< Traits, Base >& v, std::vector< basic_value< Traits, Base > >&& a ) noexcept
         {
            v.unsafe_assign_array( std::move( a ) );
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< std::map< std::string, basic_value< Traits, Base > > >
      {
         template< template< typename... > class, typename Consumer >
         static void produce( Consumer& c, const std::map< std::string, basic_value< Traits, Base > >& o )
         {
            c.begin_array( o.size() );
            for( const auto& i : o ) {
               c.key( i.first );
               events::from_value( c, i.second );
               c.member();
            }
            c.end_array( o.size() );
         }

         static void assign( basic_value< Traits, Base >& v, const std::map< std::string, basic_value< Traits, Base > >& o )
         {
            v.unsafe_assign_object( std::move( o ) );
         }

         static void assign( basic_value< Traits, Base >& v, std::map< std::string, basic_value< Traits, Base > >&& o ) noexcept
         {
            v.unsafe_assign_object( std::move( o ) );
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< const basic_value< Traits, Base >* >
      {
         static void assign( basic_value< Traits, Base >& v, const basic_value< Traits, Base >* p ) noexcept
         {
            v.unsafe_assign_raw_ptr( p );
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool equal( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == null );
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool less_than( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < null );
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool greater_than( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > null );
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< basic_value< Traits, Base >* >
         : traits< const basic_value< Traits, Base >* >
      {
      };

      template< typename T >
      struct traits< tao::optional< T > >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const tao::optional< T >& o )
         {
            if( o ) {
               Traits< T >::template produce< Traits >( c, *o );
            }
            else {
               c.null();
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const tao::optional< T >& o )
         {
            if( o ) {
               v = *o;
            }
            else {
               v.unsafe_assign_null();
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void extract( const basic_value< Traits, Base >& v, tao::optional< T >& o )
         {
            if( v.is_null() ) {
               o = tao::nullopt;
            }
            else {
               o = v.template as< T >();
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == null );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < null );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > null );
         }
      };

   }  // namespace json

}  // namespace tao

#endif
