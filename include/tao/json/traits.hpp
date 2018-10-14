// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_TRAITS_HPP
#define TAO_JSON_TRAITS_HPP

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "binary_view.hpp"
#include "consume.hpp"
#include "forward.hpp"
#include "type.hpp"

#include "events/from_value.hpp"
#include "events/produce.hpp"

#include "internal/format.hpp"
#include "internal/identity.hpp"
#include "internal/number_traits.hpp"
#include "internal/string_t.hpp"
#include "internal/type_traits.hpp"

#include "external/pegtl/internal/pegtl_string.hpp"

#define TAO_JSON_DEFAULT_KEY( x )                   \
   template< template< typename... > class Traits > \
   using default_key = TAO_JSON_STRING_T( x )

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

namespace tao
{
   namespace json
   {
      // NOTE: traits< ... >::assign() is always called with needs_discard(v) == false

      template<>
      struct traits< null_t >
      {
         template< template< typename... > class Traits, typename Base >
         static null_t as( const basic_value< Traits, Base >& v ) noexcept
         {
            v.validate_json_type( type::NULL_ );
            return tao::json::null;
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, null_t /*unused*/ ) noexcept
         {
            v.unsafe_assign_null();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const null_t /*unused*/ )
         {
            c.null();
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_value_ptr().is_null();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_value_ptr().type() < type::NULL_;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, null_t /*unused*/ ) noexcept
         {
            return lhs.skip_value_ptr().type() > type::NULL_;
         }
      };

      template<>
      struct traits< bool >
      {
         template< template< typename... > class Traits, typename Base >
         static bool as( const basic_value< Traits, Base >& v )
         {
            return v.get_boolean();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const bool b ) noexcept
         {
            v.unsafe_assign_boolean( b );
         }

         template< template< typename... > class, typename Producer >
         static bool consume( Producer& parser )
         {
            return parser.boolean();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const bool b )
         {
            c.boolean( b );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return p.is_boolean() && ( p.unsafe_get_boolean() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() < type::BOOLEAN ) || ( p.is_boolean() && ( p.unsafe_get_boolean() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const bool rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::BOOLEAN ) || ( p.is_boolean() && ( p.unsafe_get_boolean() > rhs ) );
         }
      };

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
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary().empty();
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view().empty();
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_binary_t /*unused*/ ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
               case type::BINARY_VIEW:
                  return false;
               default:
                  return p.type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_binary_t /*unused*/ ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return !p.unsafe_get_binary().empty();
               case type::BINARY_VIEW:
                  return !p.unsafe_get_binary_view().empty();
               default:
                  return p.type() > type::BINARY;
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
            const auto& p = lhs.skip_value_ptr();
            return p.is_array() && p.unsafe_get_array().empty();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_array_t /*unused*/ ) noexcept
         {
            return lhs.skip_value_ptr().type() < type::ARRAY;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_array_t /*unused*/ ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::ARRAY ) || ( p.is_array() && !p.unsafe_get_array().empty() );
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
            const auto& p = lhs.skip_value_ptr();
            return p.is_object() && p.unsafe_get_object().empty();
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, empty_object_t /*unused*/ ) noexcept
         {
            return lhs.skip_value_ptr().type() < type::OBJECT;
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, empty_object_t /*unused*/ ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::OBJECT ) || ( p.is_object() && !p.unsafe_get_object().empty() );
         }
      };

      template<>
      struct traits< std::string >
      {
         template< template< typename... > class Traits, typename Base >
         static std::string as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::STRING:
                  return v.unsafe_get_string();
               case type::STRING_VIEW: {
                  const auto sv = v.unsafe_get_string_view();
                  return std::string( sv.data(), sv.size() );
               }
               default:
                  throw std::logic_error( internal::format( "invalid json type '", v.type(), "' for conversion to std::string", json::base_message_extension( v.base() ) ) );  // NOLINT
            }
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

         template< template< typename... > class, typename Producer >
         static std::string consume( Producer& parser )
         {
            return parser.string();
         }

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
         static bool equal( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() < rhs;
               default:
                  return p.type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::string& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() > rhs;
               default:
                  return p.type() > type::STRING;
            }
         }
      };

      template<>
      struct traits< std::string_view >
      {
         template< template< typename... > class Traits, typename Base >
         static std::string_view as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::STRING:
                  return v.unsafe_get_string();
               case type::STRING_VIEW:
                  return v.unsafe_get_string_view();
               default:
                  throw std::logic_error( internal::format( "invalid json type '", v.type(), "' for conversion to std::string_view", json::base_message_extension( v.base() ) ) );  // NOLINT
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::string_view sv )
         {
            v.unsafe_emplace_string( sv.data(), sv.size() );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::string_view sv )
         {
            c.string( sv );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const std::string_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::string_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() < rhs;
               default:
                  return p.type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::string_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() > rhs;
               default:
                  return p.type() > type::STRING;
            }
         }
      };

      template<>
      struct traits< const char* >
      {
         template< template< typename... > class Traits, typename Base >
         static const char* as( const basic_value< Traits, Base >& v )
         {
            return v.get_string().c_str();  // String views might not be '\0'-terminated.
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const char* s )
         {
            v.unsafe_emplace_string( s );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() == rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() < rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() < rhs;
               default:
                  return p.type() < type::STRING;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const char* rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::STRING:
                  return p.unsafe_get_string() > rhs;
               case type::STRING_VIEW:
                  return p.unsafe_get_string_view() > rhs;
               default:
                  return p.type() > type::STRING;
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
      struct traits< std::vector< std::byte > >
      {
         template< template< typename... > class Traits, typename Base >
         static std::vector< std::byte > as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::BINARY:
                  return v.unsafe_get_binary();
               case type::BINARY_VIEW: {
                  const auto xv = v.unsafe_get_binary_view();
                  return std::vector< std::byte >( xv.begin(), xv.end() );
               }
               default:
                  throw std::logic_error( internal::format( "invalid json type '", v.type(), "' for conversion to std::vector< std::byte >", json::base_message_extension( v.base() ) ) );  // NOLINT
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::vector< std::byte >& x )
         {
            v.unsafe_assign_binary( x );
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, std::vector< std::byte >&& x ) noexcept
         {
            v.unsafe_assign_binary( std::move( x ) );
         }

         template< template< typename... > class, typename Producer >
         static std::vector< std::byte > consume( Producer& parser )
         {
            return parser.binary();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::vector< std::byte >& x )
         {
            c.binary( x );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, std::vector< std::byte >&& x )
         {
            c.binary( std::move( x ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const std::vector< std::byte >& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() == rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::vector< std::byte >& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() < rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() < rhs;
               default:
                  return p.type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::vector< std::byte >& rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() > rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() > rhs;
               default:
                  return p.type() > type::BINARY;
            }
         }
      };

      template<>
      struct traits< tao::binary_view >
      {
         template< template< typename... > class Traits, typename Base >
         static tao::binary_view as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::BINARY:
                  return v.unsafe_get_binary();
               case type::BINARY_VIEW:
                  return v.unsafe_get_binary_view();
               default:
                  throw std::logic_error( internal::format( "invalid json type '", v.type(), "' for conversion to tao::binary_view", json::base_message_extension( v.base() ) ) );  // NOLINT
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const tao::binary_view xv ) noexcept
         {
            v.unsafe_emplace_binary( xv.begin(), xv.end() );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const tao::binary_view xv )
         {
            c.binary( xv );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const tao::binary_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() == rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() == rhs;
               default:
                  return false;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const tao::binary_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() < rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() < rhs;
               default:
                  return p.type() < type::BINARY;
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const tao::binary_view rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            switch( p.type() ) {
               case type::BINARY:
                  return p.unsafe_get_binary() > rhs;
               case type::BINARY_VIEW:
                  return p.unsafe_get_binary_view() > rhs;
               default:
                  return p.type() > type::BINARY;
            }
         }
      };

      template<>
      struct traits< const std::vector< std::byte >& >
      {
         template< template< typename... > class Traits, typename Base >
         static const std::vector< std::byte >& as( const basic_value< Traits, Base >& v )
         {
            return v.get_binary();
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< std::vector< basic_value< Traits, Base > > >
      {
         static void assign( basic_value< Traits, Base >& v, const std::vector< basic_value< Traits, Base > >& a )
         {
            v.unsafe_assign_array( a );
         }

         static void assign( basic_value< Traits, Base >& v, std::vector< basic_value< Traits, Base > >&& a ) noexcept
         {
            v.unsafe_assign_array( std::move( a ) );
         }

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

         template< template< typename... > class, typename Consumer >
         static void produce( Consumer& c, std::vector< basic_value< Traits, Base > >&& a )
         {
            c.begin_array( a.size() );
            for( auto&& i : a ) {
               events::from_value( c, std::move( i ) );
               c.element();
            }
            c.end_array( a.size() );
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< std::map< std::string, basic_value< Traits, Base > > >
      {
         static void assign( basic_value< Traits, Base >& v, const std::map< std::string, basic_value< Traits, Base > >& o )
         {
            v.unsafe_assign_object( std::move( o ) );
         }

         static void assign( basic_value< Traits, Base >& v, std::map< std::string, basic_value< Traits, Base > >&& o ) noexcept
         {
            v.unsafe_assign_object( std::move( o ) );
         }

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

         template< template< typename... > class, typename Consumer >
         static void produce( Consumer& c, std::map< std::string, basic_value< Traits, Base > >&& o )
         {
            c.begin_array( o.size() );
            for( auto&& i : o ) {
               c.key( std::move( i.first ) );
               events::from_value( c, std::move( i.second ) );
               c.member();
            }
            c.end_array( o.size() );
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< const basic_value< Traits, Base >* >
      {
         static void assign( basic_value< Traits, Base >& v, const basic_value< Traits, Base >* p ) noexcept
         {
            v.unsafe_assign_value_ptr( p );
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool equal( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            assert( rhs );
            return lhs == *rhs;
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool less_than( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            assert( rhs );
            return lhs < *rhs;
         }

         template< template< typename... > class TraitsL, typename BaseL >
         static bool greater_than( const basic_value< TraitsL, BaseL >& lhs, const basic_value< Traits, Base >* rhs ) noexcept
         {
            assert( rhs );
            return lhs > *rhs;
         }
      };

      template< template< typename... > class Traits, typename Base >
      struct traits< basic_value< Traits, Base >* >
         : public traits< const basic_value< Traits, Base >* >
      {
      };

      template< typename T >
      struct traits< std::optional< T > >
      {
         template< template< typename... > class Traits >
         using default_key = typename Traits< T >::template default_key< Traits >;

         template< template< typename... > class Traits >
         static bool is_nothing( const std::optional< T >& o )
         {
            return ( !bool( o ) ) || internal::is_nothing< Traits >( *o );  // TODO: Only query o?
         }

         template< template< typename... > class Traits, typename Base >
         static std::optional< T > as( const basic_value< Traits, Base >& v )
         {
            if( v == null ) {
               return std::nullopt;
            }
            return v.template as< T >();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const std::optional< T >& o )
         {
            if( o ) {
               v = *o;
            }
            else {
               v = null;
            }
         }

         template< template< typename... > class Traits, typename Producer >
         static std::optional< T > consume( Producer& parser )
         {
            if( parser.null() ) {
               return std::nullopt;
            }
            return json::consume< T, Traits >( parser );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const std::optional< T >& o )
         {
            if( o ) {
               json::events::produce< Traits >( c, *o );
            }
            else {
               json::events::produce< Traits >( c, null );
            }
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const std::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs == *rhs ) : ( lhs == null );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs < *rhs ) : ( lhs < null );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::optional< T >& rhs ) noexcept
         {
            return rhs ? ( lhs > *rhs ) : ( lhs > null );
         }
      };

   }  // namespace json

}  // namespace tao

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
