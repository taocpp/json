// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_INTERNAL_OBJECT_HPP
#define TAO_JSON_BINDING_INTERNAL_OBJECT_HPP

#include <bitset>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "../for_nothing_value.hpp"
#include "../for_unknown_key.hpp"
#include "../member_kind.hpp"

#include "../../forward.hpp"

#include "../../basic_value.hpp"
#include "../../internal/escape.hpp"
#include "../../internal/format.hpp"
#include "../../internal/type_traits.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         namespace internal
         {
            template< for_unknown_key E, for_nothing_value N, typename T, typename L = std::make_index_sequence< T::size > >
            struct basic_object;

            template< typename... Ts >
            void list_all_keys( std::ostream& oss, const std::map< std::string, Ts... >& m )
            {
               for( const auto& p : m ) {
                  json::internal::format_to( oss, ' ', p.first );
               }
            }

            template< std::size_t N, typename... Ts >
            void list_missing_keys( std::ostream& oss, const std::bitset< N >& t, const std::map< std::string, Ts... >& m )
            {
               for( const auto& p : m ) {
                  if( !t.test( p.second.index ) ) {
                     json::internal::format_to( oss, ' ', p.first );
                  }
               }
            }

            template< for_unknown_key E, for_nothing_value N, typename... As, std::size_t... Is >
            struct basic_object< E, N, json::internal::type_list< As... >, std::index_sequence< Is... > >
            {
               using members = json::internal::type_list< As... >;

               template< typename F >
               struct entry
               {
                  entry( F c, std::size_t i )
                     : function( c ),
                       index( i )
                  {
                  }

                  F function;
                  std::size_t index;
               };

               template< typename A, std::size_t I >
               static void set_optional_bit( std::bitset< sizeof...( As ) >& t )
               {
                  t.set( I, A::kind == member_kind::optional );
               }

               template< typename A, typename C, template< typename... > class Traits >
               static void to_wrapper( const basic_value< Traits >& v, C& x )
               {
                  A::template to< Traits >( v, x );
               }

               template< typename A, std::size_t I, typename C, template< typename... > class Traits, typename F >
               static void emplace_to( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::template key< Traits >(), entry< F >( &basic_object::to_wrapper< A, C, Traits >, I ) );
               }

               template< template< typename... > class Traits, typename C >
               static void to( const basic_value< Traits >& v, C& x )
               {
                  using F = void ( * )( const basic_value< Traits >&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     ( emplace_to< As, Is, C, Traits >( t ), ... );
                     assert( t.size() == sizeof...( As ) );  // TODO: Check for duplicate keys at compile time?
                     return t;
                  }();
                  static const std::bitset< sizeof...( As ) > o = []() {
                     std::bitset< sizeof...( As ) > t;
                     ( set_optional_bit< As, Is >( t ), ... );
                     return t;
                  }();

                  const auto& a = v.get_object();
                  std::bitset< sizeof...( As ) > b;
                  for( const auto& p : a ) {
                     const auto& k = p.first;
                     const auto i = m.find( k );
                     if( i == m.end() ) {
                        if constexpr( E == for_unknown_key::skip ) {
                           continue;
                        }
                        std::ostringstream oss;
                        json::internal::format_to( oss, "unknown object key \"", json::internal::escape( k ), "\" -- known are" );
                        list_all_keys( oss, m );
                        json::internal::format_to( oss, " for type ", typeid( C ), json::message_extension( v ) );
                        throw std::runtime_error( oss.str() );  // NOLINT
                     }
                     i->second.function( p.second, x );
                     b.set( i->second.index );
                  }
                  b |= o;
                  if( !b.all() ) {
                     std::ostringstream oss;
                     json::internal::format_to( oss, "missing requiredrequired key(s)" );
                     list_missing_keys( oss, b, m );
                     json::internal::format_to( oss, " for type ", typeid( C ), json::message_extension( v ) );
                     throw std::runtime_error( oss.str() );  // NOLINT
                  }
               }

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4127 )
#endif
               template< typename A, template< typename... > class Traits, typename C >
               static void assign_member( basic_value< Traits >& v, const C& x )
               {
                  if( ( N == for_nothing_value::encode ) || ( !A::template is_nothing< Traits >( x ) ) ) {
                     v.unsafe_emplace( A::template key< Traits >(), A::read( x ) );
                  }
               }
#ifdef _MSC_VER
#pragma warning( pop )
#endif

               template< template< typename... > class Traits, typename C >
               static void assign( basic_value< Traits >& v, const C& x )
               {
                  v.unsafe_emplace_object();
                  ( assign_member< As >( v, x ), ... );
               }

               template< typename A, typename C, template< typename... > class Traits, typename Producer >
               static void consume_wrapper( Producer& parser, C& x )
               {
                  A::template consume< Traits, Producer >( parser, x );
               }

               template< typename A, std::size_t I, typename C, template< typename... > class Traits, typename Producer, typename F >
               static void emplace_consume( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::template key< Traits >(), entry< F >( &basic_object::consume_wrapper< A, C, Traits, Producer >, I ) );
               }

               template< template< typename... > class Traits = traits, typename Producer, typename C >
               static void consume( Producer& parser, C& x )
               {
                  using F = void ( * )( Producer&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     ( emplace_consume< As, Is, C, Traits, Producer >( t ), ... );
                     assert( t.size() == sizeof...( As ) );  // TODO: Check for duplicate keys at compile time?
                     return t;
                  }();
                  static const std::bitset< sizeof...( As ) > o = []() {
                     std::bitset< sizeof...( As ) > t;
                     ( set_optional_bit< As, Is >( t ), ... );
                     return t;
                  }();

                  auto s = parser.begin_object();
                  std::bitset< sizeof...( As ) > b;
                  while( parser.member_or_end_object( s ) ) {
                     const auto k = parser.key();
                     const auto i = m.find( k );
                     if( i == m.end() ) {
                        if constexpr( E == for_unknown_key::skip ) {
                           parser.skip_value();
                           continue;
                        }
                        std::ostringstream oss;
                        json::internal::format_to( oss, "unknown object key \"", json::internal::escape( k ), "\" -- known are" );
                        list_all_keys( oss, m );
                        json::internal::format_to( oss, " for type ", typeid( C ) );
                        parser.throw_parse_error( oss.str() );  // NOLINT
                     }
                     if( b.test( i->second.index ) ) {
                        parser.throw_parse_error( json::internal::format( "duplicate object key \"", json::internal::escape( k ), "\" for type ", typeid( C ) ) );
                     }
                     i->second.function( parser, x );
                     b.set( i->second.index );
                  }
                  b |= o;
                  if( !b.all() ) {
                     std::ostringstream oss;
                     json::internal::format_to( oss, "missing required key(s)" );
                     list_missing_keys( oss, b, m );
                     json::internal::format_to( oss, " for type ", typeid( C ) );
                     parser.throw_parse_error( oss.str() );  // NOLINT
                  }
               }

               template< template< typename... > class Traits, typename C >
               static std::size_t produce_size( const C& x )
               {
                  if constexpr( N == for_nothing_value::encode ) {
                     return sizeof...( As );
                  }
                  return ( std::size_t( !As::template is_nothing< Traits >( x ) ) + ... );
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static void produce_member( Consumer& consumer, const C& x )
               {
                  if( ( N == for_nothing_value::encode ) || ( !A::template is_nothing< Traits >( x ) ) ) {
                     A::template produce_key< Traits >( consumer );
                     A::template produce< Traits >( consumer, x );
                     consumer.member();
                  }
               }

               template< template< typename... > class Traits = traits, typename Consumer, typename C >
               static void produce( Consumer& consumer, const C& x )
               {
                  const auto size = produce_size< Traits >( x );
                  consumer.begin_object( size );
                  ( produce_member< As, Traits >( consumer, x ), ... );
                  consumer.end_object( size );
               }

               template< typename A, template< typename... > class Traits, typename C >
               static bool equal_member( const std::map< std::string, basic_value< Traits > >& a, C& x )
               {
                  if( !A::template is_nothing< Traits >( x ) ) {
                     return a.at( A::template key< Traits >() ) == A::read( x );
                  }
                  if constexpr( N == for_nothing_value::encode ) {
                     return a.at( A::template key< Traits >() ).is_null();
                  }
                  const auto i = a.find( A::template key< Traits >() );
                  return ( i == a.end() ) || i->second.is_null();
               }

               template< template< typename... > class Traits, typename C >
               static bool equal( const basic_value< Traits >& lhs, const C& rhs ) noexcept
               {
                  const auto& p = lhs.skip_value_ptr();
                  if( p.is_object() && ( p.unsafe_get_object().size() == sizeof...( As ) ) ) {
                     const auto& a = p.unsafe_get_object();
                     return ( equal_member< As >( a, rhs ) && ... );
                  }
                  return false;
               }
            };

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
