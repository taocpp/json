// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_HPP
#define TAO_JSON_BINDING_HPP

#include <bitset>
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

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, v.*P );
            }

            template< template< typename... > class Traits = traits, typename Parts >
            static void consume( Parts& parser, C& v )
            {
               json::consume< Traits >( parser, v.*P );
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
         };

         template< typename T >
         struct inherit;

         enum class for_unknown_key : bool
         {
            THROW,
            CONTINUE
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

            template< typename T >
            struct array;

            template< typename... As >
            struct array< json::internal::type_list< As... > >
            {
               using elements = json::internal::type_list< As... >;

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool as_element( const std::vector< basic_value< Traits, Base > >& a, C& x, std::size_t& i )
               {
                  A::as( a.at( i++ ), x );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void as( const basic_value< Traits, Base >& v, C& x )
               {
                  std::size_t i = 0;
                  const auto& a = v.get_array();
                  (void)json::internal::swallow{ as_element< As >( a, x, i )... };
               }

               template< typename A, template< typename... > class Traits = traits, typename Parts, typename C, typename State >
               static bool consume_element( Parts& parser, C& x, State& s )
               {
                  parser.element( s );
                  A::template consume< Traits >( parser, x );
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Parts, typename C >
               static void consume( Parts& parser, C& x )
               {
                  auto s = parser.begin_array();
                  (void)json::internal::swallow{ consume_element< As, Traits >( parser, x, s )... };
                  parser.end_array( s );
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

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool equal_element( const std::vector< basic_value< Traits, Base > >& a, C& x, std::size_t& i )
               {
                  return a[ i++ ] == A::read( x );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static bool equal( const basic_value< Traits, Base >& lhs, const C& rhs ) noexcept
               {
                  if( bool result = lhs.is_array() && ( lhs.unsafe_get_array().size() == sizeof...( As ) ) ) {
                     std::size_t i = 0;
                     const auto& a = lhs.get_array();
                     (void)json::internal::swallow{ result = result && equal_element< As >( a, rhs, i )... };
                     return result;
                  }
                  return false;
               }
            };

            // TODO: Control how to create the instances?

            template< for_unknown_key E, typename T >
            struct basic_object;

            template< for_unknown_key E, typename... As >
            struct basic_object< E, json::internal::type_list< As... > >
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

               template< typename A >
               static bool set_optional_bit( std::bitset< sizeof...( As ) >& t, std::size_t& i )
               {
                  t.set( i++, A::kind == member_kind::OPTIONAL );
                  return true;
               }

               template< typename A, template< typename... > class Traits, typename Base, typename F >
               static bool emplace_as( std::map< std::string, entry< F > >& m, std::size_t& i )
               {
                  m.emplace( A::key(), entry< F >( &A::template as< Traits, Base >, i++ ) );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void as( const basic_value< Traits, Base >& v, C& x )
               {
                  using F = void ( * )( const basic_value< Traits, Base >&, C& );
                  static const std::map< std::string, entry< F > > m = []( std::size_t i ) {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_as< As, Traits, Base >( t, i )... };
                     assert( t.size() == sizeof...( As ) );
                     return t;
                  }( 0 );
                  static const std::bitset< sizeof...( As ) > o = []( std::size_t i ) {
                     std::bitset< sizeof...( As ) > t;
                     (void)json::internal::swallow{ set_optional_bit< As >( t, i )... };
                     return t;
                  }( 0 );

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

               template< typename A, template< typename... > class Traits, typename Parts, typename F >
               static bool emplace_consume( std::map< std::string, entry< F > >& m, std::size_t& i )
               {
                  m.emplace( A::key(), entry< F >( &A::template consume< Traits, Parts >, i++ ) );
                  return true;
               }

               template< template< typename... > class Traits = traits, typename Parts, typename C >
               static void consume( Parts& parser, C& x )
               {
                  using F = void ( * )( Parts&, C& );
                  static const std::map< std::string, entry< F > > m = []( std::size_t i ) {
                     std::map< std::string, entry< F > > t;
                     (void)json::internal::swallow{ emplace_consume< As, Traits, Parts >( t, i )... };
                     assert( t.size() == sizeof...( As ) );
                     return t;
                  }( 0 );
                  static const std::bitset< sizeof...( As ) > o = []( std::size_t i ) {
                     std::bitset< sizeof...( As ) > t;
                     (void)json::internal::swallow{ set_optional_bit< As >( t, i )... };
                     return t;
                  }( 0 );

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
                        parser.throw_parse_error( "duplicate object key ", json::internal::escape( k ) );
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

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static bool assign_member( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace( A::key(), A::read( x ) );
                  return true;
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void assign( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_object();
                  (void)json::internal::swallow{ assign_member< As >( v, x )... };
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static bool produce_member( Consumer& consumer, const C& x )
               {
                  A::template produce_key< Traits >( consumer );
                  A::template produce< Traits >( consumer, x );
                  consumer.member();
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

         template< for_unknown_key W, typename... As >
         using basic_object = internal::basic_object< W, json::internal::merge_type_lists< internal::inherit_members< As >... > >;

         template< typename... As >
         using object = basic_object< for_unknown_key::THROW, As... >;

         template< typename K >
         struct factory_name;

         template< char... Cs >
         struct factory_name< key< Cs... > >
         {
            static std::string name()
            {
               static const char s[] = { Cs..., 0 };
               return std::string( s, sizeof...( Cs ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce_name( Consumer& consumer )
            {
               static const char s[] = { Cs..., 0 };
               consumer.key( tao::string_view( s, sizeof...( Cs ) ) );
            }
         };

         template< typename K, typename T, typename U >
         struct factory_type
            : public factory_name< K >
         {
            static const std::type_info* type()
            {
               return &typeid( T );
            }

            template< template< typename... > class Traits, typename Base >
            static std::shared_ptr< U > as( const basic_value< Traits, Base >& v )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               return R::as( v );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const std::shared_ptr< U >& p )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               R::assign( v, p );
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const std::shared_ptr< U >& p )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               R::template produce< Traits >( c, p );
            }

            template< template< typename... > class Traits, typename Parts >
            static std::shared_ptr< U > consume( Parts& parser )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               return R::template consume< Traits >( parser );
            }
         };

         template< typename K, typename T >
         struct factory_temp
         {
            template< typename U >
            using bind = factory_type< K, T, U >;
         };

         template< typename U, typename... Ts >
         struct factory
         {
            template< typename F >
            struct entry
            {
               explicit entry( F c )
                  : function( c )
               {
               }

               F function;
            };

            template< typename F >
            struct entry2
            {
               entry2( F c, const std::string& n )
                  : function( c ),
                    name( n )
               {
               }

               F function;
               std::string name;
            };

            template< typename V, template< typename... > class Traits, typename Base, typename F >
            static bool emplace_as( std::map< std::string, entry< F > >& m )
            {
               using W = typename V::template bind< U >;
               m.emplace( W::name(), entry< F >( &W::template as< Traits, Base > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base >
            static std::shared_ptr< U > as( const basic_value< Traits, Base >& v )
            {
               using F = std::shared_ptr< U > ( * )( const basic_value< Traits, Base >& );
               static const std::map< std::string, entry< F > > m = []() {
                  std::map< std::string, entry< F > > t;
                  (void)json::internal::swallow{ emplace_as< Ts, Traits, Base >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto& a = v.get_object();
               if( a.size() != 1 ) {
                  throw std::runtime_error( "unexpected JSON object size for polymorphic object factory" );  // NOLINT
               }
               const auto b = a.begin();
               const auto i = m.find( b->first );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + json::internal::escape( b->first ) );  // NOLINT
               }
               return i->second.function( b->second );
            }

            template< typename V, template< typename... > class Traits, typename Parts, typename F >
            static bool emplace_consume( std::map< std::string, entry< F > >& m )
            {
               using W = typename V::template bind< U >;
               m.emplace( W::name(), entry< F >( &W::template consume< Traits, Parts > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Parts >
            static std::shared_ptr< U > consume( Parts& parser )
            {
               using F = std::shared_ptr< U > ( * )( Parts & );
               static const std::map< std::string, entry< F > > m = []() {
                  std::map< std::string, entry< F > > t;
                  (void)json::internal::swallow{ emplace_consume< Ts, Traits, Parts >( t )... };
                  return t;
               }();

               auto s = parser.begin_object();
               const auto k = parser.key();
               const auto i = m.find( k );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + json::internal::escape( k ) );  // NOLINT
               }
               auto r = i->second.function( parser );
               parser.end_object( s );
               return r;
            }

            template< typename V, template< typename... > class Traits, typename Base, typename F >
            static bool emplace_assign( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U >;
               m.emplace( W::type(), entry2< F >( &W::template assign< Traits, Base >, W::name() ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const std::shared_ptr< U >& p )
            {
               using F = void ( * )( basic_value< Traits, Base >&, const std::shared_ptr< U >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  (void)json::internal::swallow{ emplace_assign< Ts, Traits, Base >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + TAO_JSON_PEGTL_NAMESPACE::internal::demangle( typeid( *p ).name() ) );  // NOLINT
               }
               i->second.function( v, p );
               v = {
                  { i->second.name, std::move( v ) }
               };
            }

            template< typename V, template< typename... > class Traits, typename Consumer, typename F >
            static bool emplace_produce( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U >;
               m.emplace( W::type(), entry2< F >( &W::template produce< Traits, Consumer >, W::name() ) );
               return true;
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& consumer, const std::shared_ptr< U >& p )
            {
               using F = void ( * )( Consumer&, const std::shared_ptr< U >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  (void)json::internal::swallow{ emplace_produce< Ts, Traits, Consumer >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + TAO_JSON_PEGTL_NAMESPACE::internal::demangle( typeid( *p ).name() ) );  // NOLINT
               }
               consumer.begin_object( 1 );
               consumer.key( i->second.name );
               i->second.function( consumer, p );
               consumer.member();
               consumer.end_object( 1 );
            }
         };

         template< typename... Vs >
         struct versions;

         template< typename V >
         struct versions< V >
            : public V
         {
            // TODO: Or static_assert more than one version?
         };

         template< typename V, typename... Vs >
         struct versions< V, Vs... >
            : public V
         {
            // NOTE: This is a bit like a high-level pegtl::sor<>.
            // TODO: Clear x between attempts?

            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool as_attempt( const basic_value< Traits, Base >& v, C& x )
            {
               try {
                  A::as( v, x );
                  return true;
               }
               catch( ... ) {
                  return false;
               }
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               bool ok = false;
               (void)json::internal::swallow{ ok = as_attempt< V >( v, x ), ( ok = ok || as_attempt< Vs >( v, x ) )... };
               if( ! ok ) {
                  throw std::runtime_error( "all versions failed" );  // NOLINT
               }
            }

            template< typename A, template< typename... > class Traits, typename Parts, typename C >
            static bool consume_attempt( Parts& parser, C& x )
            {
               try {
                  auto m = parser.mark();
                  A::template consume< Traits >( parser, x );
                  return m( true );
               }
               catch( ... ) {
                  return false;
               }
            }

            template< template< typename... > class Traits, typename Parts, typename C >
            static void consume( Parts& parser, C& x )
            {
               bool ok = false;
               (void)json::internal::swallow{ ok = consume_attempt< V, Traits >( parser, x ), ( ok = ok || consume_attempt< Vs, Traits >( parser, x ) )... };
               if( ! ok ) {
                  throw std::runtime_error( "all versions failed" );  // NOLINT
               }
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic pop
#endif

#define TAO_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >

#define TAO_JSON_BIND_REQUIRED( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::REQUIRED, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAO_JSON_BIND_OPTIONAL( KeY, ... ) tao::json::binding::member< tao::json::binding::member_kind::OPTIONAL, TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#define TAO_JSON_FACTORY_BIND( KeY, ... ) tao::json::binding::factory_temp< TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), __VA_ARGS__ >

#endif
