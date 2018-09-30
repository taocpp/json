// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_TRAITS_HPP
#define TAO_JSON_TRAITS_HPP

#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "binary_view.hpp"
#include "consume.hpp"
#include "forward.hpp"
#include "local_date.hpp"
#include "local_date_time.hpp"
#include "local_time.hpp"
#include "offset_date_time.hpp"
#include "type.hpp"

#include "events/from_value.hpp"
#include "events/produce.hpp"

#include "external/byte.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

#include "internal/identity.hpp"
#include "internal/indirect_traits.hpp"
#include "internal/number_traits.hpp"
#include "internal/string_t.hpp"
#include "internal/type_traits.hpp"

#include "external/pegtl/internal/pegtl_string.hpp"

#define TAO_JSON_DEFAULT_KEY( x ) \
   using default_key = TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, x )

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
      struct traits< local_date >
      {
         template< template< typename... > class Traits, typename Base >
         static local_date as( const basic_value< Traits, Base >& v )
         {
            return v.get_local_date();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const local_date ld ) noexcept
         {
            v.unsafe_assign_local_date( ld );
         }

         template< template< typename... > class, typename Producer >
         static local_date consume( Producer& parser )
         {
            return parser.local_date();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const local_date ld )
         {
            c.local_date( ld );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const local_date rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return p.is_local_date() && ( p.unsafe_get_local_date() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const local_date rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() < type::LOCAL_DATE ) || ( p.is_local_date() && ( p.unsafe_get_local_date() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const local_date rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::LOCAL_DATE ) || ( p.is_local_date() && ( p.unsafe_get_local_date() > rhs ) );
         }
      };

      template<>
      struct traits< local_time >
      {
         template< template< typename... > class Traits, typename Base >
         static local_time as( const basic_value< Traits, Base >& v )
         {
            return v.get_local_time();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const local_time lt ) noexcept
         {
            v.unsafe_assign_local_time( lt );
         }

         template< template< typename... > class, typename Producer >
         static local_time consume( Producer& parser )
         {
            return parser.local_time();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const local_time lt )
         {
            c.local_time( lt );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const local_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return p.is_local_time() && ( p.unsafe_get_local_time() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const local_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() < type::LOCAL_TIME ) || ( p.is_local_time() && ( p.unsafe_get_local_time() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const local_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::LOCAL_TIME ) || ( p.is_local_time() && ( p.unsafe_get_local_time() > rhs ) );
         }
      };

      template<>
      struct traits< local_date_time >
      {
         template< template< typename... > class Traits, typename Base >
         static local_date_time as( const basic_value< Traits, Base >& v )
         {
            return v.get_local_date_time();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const local_date_time ldt ) noexcept
         {
            v.unsafe_assign_local_date_time( ldt );
         }

         template< template< typename... > class, typename Producer >
         static local_date_time consume( Producer& parser )
         {
            return parser.local_date_time();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const local_date_time ldt )
         {
            c.local_date_time( ldt );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const local_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return p.is_local_date_time() && ( p.unsafe_get_local_date_time() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const local_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() < type::LOCAL_DATE_TIME ) || ( p.is_local_date_time() && ( p.unsafe_get_local_date_time() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const local_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::LOCAL_DATE_TIME ) || ( p.is_local_date_time() && ( p.unsafe_get_local_date_time() > rhs ) );
         }
      };

      template<>
      struct traits< offset_date_time >
      {
         template< template< typename... > class Traits, typename Base >
         static offset_date_time as( const basic_value< Traits, Base >& v )
         {
            return v.get_offset_date_time();
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const offset_date_time odt ) noexcept
         {
            v.unsafe_assign_offset_date_time( odt );
         }

         template< template< typename... > class, typename Producer >
         static offset_date_time consume( Producer& parser )
         {
            return parser.offset_date_time();
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const offset_date_time odt )
         {
            c.offset_date_time( odt );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const offset_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return p.is_offset_date_time() && ( p.unsafe_get_offset_date_time() == rhs );
         }

         template< template< typename... > class Traits, typename Base >
         static bool less_than( const basic_value< Traits, Base >& lhs, const offset_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() < type::OFFSET_DATE_TIME ) || ( p.is_offset_date_time() && ( p.unsafe_get_offset_date_time() < rhs ) );
         }

         template< template< typename... > class Traits, typename Base >
         static bool greater_than( const basic_value< Traits, Base >& lhs, const offset_date_time rhs ) noexcept
         {
            const auto& p = lhs.skip_value_ptr();
            return ( p.type() > type::OFFSET_DATE_TIME ) || ( p.is_offset_date_time() && ( p.unsafe_get_offset_date_time() > rhs ) );
         }
      };

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
                  throw std::logic_error( std::string( "invalid json type '" ) + to_string( v.type() ) + "' for conversion to std::string" );  // NOLINT
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
      struct traits< tao::string_view >
      {
         template< template< typename... > class Traits, typename Base >
         static tao::string_view as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::STRING:
                  return v.unsafe_get_string();
               case type::STRING_VIEW:
                  return v.unsafe_get_string_view();
               default:
                  throw std::logic_error( std::string( "invalid json type '" ) + to_string( v.type() ) + "' for conversion to tao::string_view" );  // NOLINT
            }
         }

         template< template< typename... > class Traits, typename Base >
         static void assign( basic_value< Traits, Base >& v, const tao::string_view sv )
         {
            v.unsafe_emplace_string( sv.data(), sv.size() );
         }

         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& c, const tao::string_view sv )
         {
            c.string( sv );
         }

         template< template< typename... > class Traits, typename Base >
         static bool equal( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
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
         static bool less_than( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
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
         static bool greater_than( const basic_value< Traits, Base >& lhs, const tao::string_view rhs ) noexcept
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
      struct traits< std::vector< tao::byte > >
      {
         template< template< typename... > class Traits, typename Base >
         static std::vector< tao::byte > as( const basic_value< Traits, Base >& v )
         {
            switch( v.type() ) {
               case type::BINARY:
                  return v.unsafe_get_binary();
               case type::BINARY_VIEW: {
                  const auto xv = v.unsafe_get_binary_view();
                  return std::vector< tao::byte >( xv.begin(), xv.end() );
               }
               default:
                  throw std::logic_error( std::string( "invalid json type '" ) + to_string( v.type() ) + "' for conversion to std::vector< tao::byte >" + json::base_message_extension( v.base() ) );  // NOLINT
            }
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

         template< template< typename... > class, typename Producer >
         static std::vector< tao::byte > consume( Producer& parser )
         {
            return parser.binary();
         }

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
         static bool equal( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
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
         static bool less_than( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
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
         static bool greater_than( const basic_value< Traits, Base >& lhs, const std::vector< tao::byte >& rhs ) noexcept
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
                  throw std::logic_error( std::string( "invalid json type '" ) + to_string( v.type() ) + "' for conversion to tao::binary_view" + json::base_message_extension( v.base() ) );  // NOLINT
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
      struct traits< tao::optional< T > >
         : public internal::indirect_traits< tao::optional< T > >
      {
         template< template< typename... > class Traits, typename Base >
         static tao::optional< T > as( const basic_value< Traits, Base >& v )
         {
            if( v == null ) {
               return tao::nullopt;
            }
            return v.template as< T >();
         }

         template< template< typename... > class Traits, typename Producer >
         static tao::optional< T > consume( Producer& parser )
         {
            if( parser.null() ) {
               return tao::nullopt;
            }
            return json::consume< T, Traits >( parser );
         }
      };

   }  // namespace json

}  // namespace tao

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
