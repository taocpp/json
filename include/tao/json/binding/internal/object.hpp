// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_INTERNAL_OBJECT_HPP
#define TAO_JSON_BINDING_INTERNAL_OBJECT_HPP

#include <bitset>
#include <map>
#include <stdexcept>
#include <string>

#include "../for_nothing_value.hpp"
#include "../for_unknown_key.hpp"
#include "../member_kind.hpp"

#include "../../forward.hpp"

#include "../../basic_value.hpp"
#include "../../external/pegtl/internal/integer_sequence.hpp"
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
            template< for_unknown_key E, for_nothing_value N, typename T, typename L = TAO_JSON_PEGTL_NAMESPACE::internal::make_index_sequence< T::size > >
            struct basic_object;

            template< for_nothing_value N >
            struct consumer_object_size_helper;

            template< std::size_t N, typename... Ts >
            void list_missing_bits( std::ostream& o, const std::bitset< N >& t, const std::map< std::string, Ts... >& m )
            {
               for( const auto& p : m ) {
                  if( !t.test( p.second.index ) ) {
                     o << ' ' << p.first;
                  }
               }
            }

            template<>
            struct consumer_object_size_helper< for_nothing_value::ENCODE >
            {
               template< typename Consumer >
               static void begin( Consumer& consumer, const std::size_t n )
               {
                  consumer.begin_object( n );
               }

               template< typename Consumer >
               static void end( Consumer& consumer, const std::size_t n )
               {
                  consumer.end_object( n );
               }
            };

            template<>
            struct consumer_object_size_helper< for_nothing_value::SUPPRESS >
            {
               template< typename Consumer >
               static void begin( Consumer& consumer, const std::size_t /*unused*/ )
               {
                  consumer.begin_object();
               }

               template< typename Consumer >
               static void end( Consumer& consumer, const std::size_t /*unused*/ )
               {
                  consumer.end_object();
               }
            };

            template< for_unknown_key E, for_nothing_value N, typename... As, std::size_t... Is >
            struct basic_object< E, N, json::internal::type_list< As... >, TAO_JSON_PEGTL_NAMESPACE::internal::index_sequence< Is... > >
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
               static bool set_optional_bit( std::bitset< sizeof...( As ) >& t )
               {
                  t.set( I, A::kind == member_kind::OPTIONAL_MEMBER );
                  return true;
               }

               template< typename A, typename C, template< typename... > class Traits, typename Base >
               static void to_wrapper( const basic_value< Traits, Base >& v, C& x )
               {
                  A::template to< Traits, Base >( v, x );
               }

               template< typename A, std::size_t I, typename C, template< typename... > class Traits, typename Base, typename F >
               static bool emplace_to( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::template key< Traits >(), entry< F >( &basic_object::to_wrapper< A, C, Traits, Base >, I ) );
                  return true;
               }

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4127 )
#endif
               template< template< typename... > class Traits, typename Base, typename C >
               static void to( const basic_value< Traits, Base >& v, C& x )
               {
                  using F = void ( * )( const basic_value< Traits, Base >&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_to< As, Is, C, Traits, Base >( t )... };
                     assert( t.size() == sizeof...( As ) );
                     return t;
                  }();
                  static const std::bitset< sizeof...( As ) > o = []() {
                     std::bitset< sizeof...( As ) > t;
                     (void)json::internal::swallow{ set_optional_bit< As, Is >( t )... };
                     return t;
                  }();

                  const auto& a = v.get_object();
                  std::bitset< sizeof...( As ) > b;
                  for( const auto& p : a ) {
                     const auto& k = p.first;
                     const auto i = m.find( k );
                     if( i == m.end() ) {
                        if( E == for_unknown_key::CONTINUE ) {
                           continue;
                        }
                        throw std::runtime_error( "unknown object key " + json::internal::escape( k ) + json::base_message_extension( v.base() ) );  // NOLINT
                     }
                     i->second.function( p.second, x );
                     b.set( i->second.index );
                  }
                  b |= o;
                  if( !b.all() ) {
                     std::ostringstream oss;
                     json::internal::format_to( oss, "missing required key(s)" );
                     list_missing_bits( oss, b, m );
                     json::internal::format_to( oss, " for type ", typeid( C ), json::base_message_extension( v.base() ) );
                     throw std::runtime_error( oss.str() );  // NOLINT
                  }
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool assign_member( basic_value< Traits, Base >& v, const C& x )
               {
                  if( ( N == for_nothing_value::ENCODE ) || ( !A::template is_nothing< Traits >( x ) ) ) {
                     v.unsafe_emplace( A::template key< Traits >(), A::read( x ) );
                  }
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void assign( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_object();
                  (void)json::internal::swallow{ assign_member< As >( v, x )... };
               }

               template< typename A, typename C, template< typename... > class Traits, typename Producer >
               static void consume_wrapper( Producer& parser, C& x )
               {
                  A::template consume< Traits, Producer >( parser, x );
               }

               template< typename A, std::size_t I, typename C, template< typename... > class Traits, typename Producer, typename F >
               static bool emplace_consume( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::template key< Traits >(), entry< F >( &basic_object::consume_wrapper< A, C, Traits, Producer >, I ) );
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Producer, typename C >
               static void consume( Producer& parser, C& x )
               {
                  using F = void ( * )( Producer&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_consume< As, Is, C, Traits, Producer >( t )... };
                     assert( t.size() == sizeof...( As ) );
                     return t;
                  }();
                  static const std::bitset< sizeof...( As ) > o = []() {
                     std::bitset< sizeof...( As ) > t;
                     (void)json::internal::swallow{ set_optional_bit< As, Is >( t )... };
                     return t;
                  }();

                  auto s = parser.begin_object();
                  std::bitset< sizeof...( As ) > b;
                  while( parser.member_or_end_object( s ) ) {
                     const auto k = parser.key();
                     const auto i = m.find( k );
                     if( i == m.end() ) {
                        if( E == for_unknown_key::CONTINUE ) {
                           parser.skip_value();
                        }
                        else {
                           parser.throw_parse_error( json::internal::format( "unknown object key \"", json::internal::escape( k ), '"' ) );  // NOLINT
                        }
                        continue;
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
                     list_missing_bits( oss, b, m );
                     json::internal::format_to( oss, " for type ", typeid( C ) );
                     throw std::runtime_error( oss.str() );  // NOLINT
                  }
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static bool produce_member( Consumer& consumer, const C& x )
               {
                  if( ( N == for_nothing_value::ENCODE ) || ( !A::template is_nothing< Traits >( x ) ) ) {
                     A::template produce_key< Traits >( consumer );
                     A::template produce< Traits >( consumer, x );
                     consumer.member();
                  }
                  return true;
               }
#ifdef _MSC_VER
#pragma warning( pop )
#endif

               template< template< typename... > class Traits = traits, typename Consumer, typename C >
               static void produce( Consumer& consumer, const C& x )
               {
                  consumer_object_size_helper< N >::begin( consumer, sizeof...( As ) );
                  (void)json::internal::swallow{ produce_member< As, Traits >( consumer, x )... };
                  consumer_object_size_helper< N >::end( consumer, sizeof...( As ) );
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool equal_member( const std::map< std::string, basic_value< Traits, Base > >& a, C& x )
               {
                  if( !A::template is_nothing< Traits >( x ) ) {
                     return a.at( A::template key< Traits >() ) == A::read( x );
                  }
                  if( N == for_nothing_value::ENCODE ) {
                     return a.at( A::template key< Traits >() ).is_null();
                  }
                  const auto i = a.find( A::template key< Traits >() );
                  return ( i == a.end() ) || i->second.is_null();
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static bool equal( const basic_value< Traits, Base >& lhs, const C& rhs ) noexcept
               {
                  const auto& p = lhs.skip_value_ptr();
                  if( bool result = p.is_object() && ( p.unsafe_get_object().size() == sizeof...( As ) ) ) {
                     const auto& a = p.get_object();
                     (void)json::internal::swallow{ result = result && equal_member< As >( a, rhs )... };
                     return result;
                  }
                  return false;
               }
            };

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
