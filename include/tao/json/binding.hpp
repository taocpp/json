// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_HPP
#define TAO_JSON_BINDING_HPP

#include <bitset>
#include <exception>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "external/pegtl/internal/pegtl_string.hpp"

#include "consume.hpp"
#include "events/produce.hpp"
#include "forward.hpp"
#include "internal/escape.hpp"
#include "internal/type_traits.hpp"

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename T, T, typename = void >
         struct element;

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_object_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( v.*P )
            {
               return v.*P;
            }

            template< typename U >
            static void write( C& v, U&& u )
            {
               v.*P = std::forward< U >( u );
            }

            template< template< typename... > class Traits, typename Base >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               v.as( x.*P );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& parser, C& v )
            {
               json::consume< Traits >( parser, v.*P );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, v.*P );
            }
         };

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_function_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( ( v.*P )() )
            {
               return ( v.*P )();
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, ( v.*P )() );
            }
         };

         template< typename C, typename T, T( *P )( const C& ) >
         struct element< T( * )( const C& ), P >
         {
            static auto read( const C& v ) -> decltype( P( v ) )
            {
               return P( v );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, P( v ) );
            }
         };

         template< char... Cs >
         using key = json_pegtl::string< Cs... >;

         enum class member_kind : bool
         {
            OPTIONAL,
            REQUIRED
         };

         template< member_kind R, typename K >
         struct member_key;

         template< member_kind R, char... Cs >
         struct member_key< R, key< Cs... > >
         {
            static constexpr member_kind kind = R;

            static std::string key()
            {
               static const char s[] = { Cs..., 0 };
               return std::string( s, sizeof...( Cs ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce_key( Consumer& consumer )
            {
               static const char s[] = { Cs..., 0 };
               consumer.key( tao::string_view( s, sizeof...( Cs ) ) );
            }
         };

         template< member_kind R, typename K, typename T, T P >
         struct member
            : public element< T, P >,
              public member_key< R, K >
         {
            template< template< typename... > class Traits, typename C >
            static bool is_nothing( const C& x )
            {
               return json::internal::is_nothing< Traits >( element< T, P >::read( x ) );
            }
         };

         template< typename T >
         struct inherit;

         enum class for_unknown_key : bool
         {
            THROW,
            CONTINUE
         };

         enum class for_nothing_value : bool
         {
            ENCODE,
            SUPPRESS
         };

         namespace internal
         {
            template< for_unknown_key >
            struct throw_o
            {
               static void r_continue( const std::string& k )
               {
                  throw std::runtime_error( "unknown object key " + json::internal::escape( k ) );  // NOLINT
               }
            };

            template<>
            struct throw_o< for_unknown_key::CONTINUE >
            {
               static void r_continue( const std::string& /*unused*/ )
               {
               }
            };

            template< typename T, typename L = TAO_JSON_PEGTL_NAMESPACE::internal::make_index_sequence< T::size > >
            struct array;

            template< typename... As, std::size_t... Is >
            struct array< json::internal::type_list< As... >, TAO_JSON_PEGTL_NAMESPACE::internal::index_sequence< Is... > >
            {
               using elements = json::internal::type_list< As... >;

               template< typename A, std::size_t I, template< typename... > class Traits, typename Base, typename C >
               static bool as_element( const std::vector< basic_value< Traits, Base > >& a, C& x )
               {
                  A::as( a.at( I ), x );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void as( const basic_value< Traits, Base >& v, C& x )
               {
                  const auto& a = v.get_array();
                  (void)json::internal::swallow{ as_element< As, Is >( a, x )... };
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool assign_element( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_back( A::read( x ) );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void assign( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_array();
                  (void)json::internal::swallow{ assign_element< As >( v, x )... };
               }

               template< typename A, template< typename... > class Traits = traits, typename Producer, typename C, typename State >
               static bool consume_element( Producer& parser, C& x, State& s )
               {
                  parser.element( s );
                  A::template consume< Traits >( parser, x );
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Producer, typename C >
               static void consume( Producer& parser, C& x )
               {
                  auto s = parser.begin_array();
                  (void)json::internal::swallow{ consume_element< As, Traits >( parser, x, s )... };
                  parser.end_array( s );
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static bool produce_element( Consumer& consumer, const C& x )
               {
                  A::template produce< Traits >( consumer, x );
                  consumer.element();
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Consumer, typename C >
               static void produce( Consumer& consumer, const C& x )
               {
                  consumer.begin_array( sizeof...( As ) );
                  (void)json::internal::swallow{ produce_element< As, Traits >( consumer, x )... };
                  consumer.end_array( sizeof...( As ) );
               }

               template< typename A, std::size_t I, template< typename... > class Traits, typename Base, typename C >
               static bool equal_element( const std::vector< basic_value< Traits, Base > >& a, C& x )
               {
                  return a[ I ] == A::read( x );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static bool equal( const basic_value< Traits, Base >& lhs, const C& rhs ) noexcept
               {
                  if( bool result = lhs.is_array() && ( lhs.unsafe_get_array().size() == sizeof...( As ) ) ) {
                     const auto& a = lhs.get_array();
                     (void)json::internal::swallow{ result = result && equal_element< As, Is >( a, rhs )... };
                     return result;
                  }
                  return false;
               }
            };

            // TODO: Control how to create the instances?

            template< for_unknown_key E, for_nothing_value N, typename T, typename L = TAO_JSON_PEGTL_NAMESPACE::internal::make_index_sequence< T::size > >
            struct basic_object;

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
                  t.set( I, A::kind == member_kind::OPTIONAL );
                  return true;
               }

               template< typename A, std::size_t I, template< typename... > class Traits, typename Base, typename F >
               static bool emplace_as( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::key(), entry< F >( &A::template as< Traits, Base >, I ) );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void as( const basic_value< Traits, Base >& v, C& x )
               {
                  using F = void ( * )( const basic_value< Traits, Base >&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_as< As, Is, Traits, Base >( t )... };
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
                        internal::throw_o< E >::r_continue( k );
                        continue;
                     }
                     i->second.function( p.second, x );
                     b.set( i->second.index );
                  }
                  b |= o;
                  if( !b.all() ) {
                     // TODO: List the missing required key(s) in the exception?
                     throw std::runtime_error( "missing required key(s)" );  // NOLINT
                  }
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool assign_member( basic_value< Traits, Base >& v, const C& x )
               {
                  if( ( N == for_nothing_value::ENCODE ) && ( !A::template is_nothing< Traits >( x ) ) ) {
                     v.unsafe_emplace( A::key(), A::read( x ) );
                  }
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void assign( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_object();
                  (void)json::internal::swallow{ assign_member< As >( v, x )... };
               }

               template< typename A, std::size_t I, template< typename... > class Traits, typename Producer, typename F >
               static bool emplace_consume( std::map< std::string, entry< F > >& m )
               {
                  m.emplace( A::key(), entry< F >( &A::template consume< Traits, Producer >, I ) );
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Producer, typename C >
               static void consume( Producer& parser, C& x )
               {
                  using F = void ( * )( Producer&, C& );
                  static const std::map< std::string, entry< F > > m = []() {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_consume< As, Is, Traits, Producer >( t )... };
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
                        binding::internal::throw_o< E >::r_continue( k );
                        parser.skip_value();
                        continue;
                     }
                     if( b.test( i->second.index ) ) {
                        parser.throw_parse_error( "duplicate object key", json::internal::escape( k ), "for type", typeid( C ) );
                     }
                     i->second.function( parser, x );
                     b.set( i->second.index );
                  }
                  b |= o;
                  if( !b.all() ) {
                     // TODO: List the missing required key(s) in the exception?
                     throw std::runtime_error( "missing required key(s)" );  // NOLINT
                  }
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static bool produce_member( Consumer& consumer, const C& x )
               {
                  if( ( N == for_nothing_value::ENCODE ) && ( !A::template is_nothing< Traits >( x ) ) ) {
                     A::template produce_key< Traits >( consumer );
                     A::template produce< Traits >( consumer, x );
                     consumer.member();
                  }
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Consumer, typename C >
               static void produce( Consumer& consumer, const C& x )
               {
                  consumer.begin_object( sizeof...( As ) );
                  (void)json::internal::swallow{ produce_member< As, Traits >( consumer, x )... };
                  consumer.end_object( sizeof...( As ) );
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool equal_member( const std::map< std::string, basic_value< Traits, Base > >& a, C& x )
               {
                  // TODO: If we could assume the As... to be sorted by their keys we could easily optimise this, otherwise it's slightly more involved...
                  return a[ A::key() ] == A::read( x );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static bool equal( const basic_value< Traits, Base >& lhs, const C& rhs ) noexcept
               {
                  if( bool result = lhs.is_object() && ( lhs.unsafe_get_object().size() == sizeof...( As ) ) ) {
                     const auto& a = lhs.get_object();
                     (void)json::internal::swallow{ result = result && equal_member< As >( a, rhs )... };
                     return result;
                  }
                  return false;
               }
            };

            template< typename T >
            struct inherit_elements_t
            {
               using list = json::internal::type_list< T >;
            };

            template< typename T >
            struct inherit_elements_t< inherit< T > >
            {
               using list = typename T::elements;
            };

            template< typename T >
            using inherit_elements = typename inherit_elements_t< T >::list;

            template< typename T >
            struct inherit_members_t
            {
               using list = json::internal::type_list< T >;
            };

            template< typename T >
            struct inherit_members_t< inherit< T > >
            {
               using list = typename T::members;
            };

            template< typename T >
            using inherit_members = typename inherit_members_t< T >::list;

         }  // namespace internal

         template< typename... As >
         using array = internal::array< json::internal::merge_type_lists< internal::inherit_elements< As >... > >;

         template< for_unknown_key E, for_nothing_value N, typename... As >
         using basic_object = internal::basic_object< E, N, json::internal::merge_type_lists< internal::inherit_members< As >... > >;

         template< typename... As >
         using object = basic_object< for_unknown_key::THROW, for_nothing_value::ENCODE, As... >;

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic pop
#endif

#define TAO_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#include "binding/constants.hpp"
#include "binding/factory.hpp"
#include "binding/versions.hpp"

#endif
