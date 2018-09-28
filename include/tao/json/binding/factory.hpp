// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_FACTORY_HPP
#define TAO_JSON_BINDING_FACTORY_HPP

#include <stdexcept>

#include "../forward.hpp"

#include "../basic_value.hpp"
#include "../external/pegtl/internal/pegtl_string.hpp"
#include "../internal/escape.hpp"
#include "../internal/string_t.hpp"

#include "internal/type_key.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         namespace internal
         {
            template< typename K, typename T, typename U, template< typename... > class P >
            struct factory_type
               : public type_key< K, T >
            {
               static const std::type_info* type()
               {
                  return &typeid( T );
               }

               template< template< typename... > class Traits, typename Base >
               static P< U > as( const basic_value< Traits, Base >& v )
               {
                  using R = typename Traits< P< T > >::template with_base< U >;
                  return R::as( v );
               }

               template< template< typename... > class Traits, typename Base >
               static void assign( basic_value< Traits, Base >& v, const P< U >& p )
               {
                  using R = Traits< T >;  // OK to bypass Traits< P< T > >?
                  R::assign( v, static_cast< const T& >( *p ) );
               }

               template< template< typename... > class Traits, typename Producer >
               static P< U > consume( Producer& parser )
               {
                  using R = typename Traits< P< T > >::template with_base< U >;
                  return R::template consume< Traits >( parser );
               }

               template< template< typename... > class Traits, typename Consumer >
               static void produce( Consumer& c, const P< U >& p )
               {
                  using R = Traits< T >;  // OK to bypass Traits< P< T > >?
                  R::template produce< Traits >( c, static_cast< const T& >( *p ) );
               }
            };

            template< typename K, typename T >
            struct factory_temp
            {
               template< typename U, template< typename... > class P >
               using bind = factory_type< K, T, U, P >;
            };

         }  // namespace internal

         template< template< typename... > class P, typename U, typename... Ts >
         struct basic_factory
         {
            template< template< typename... > class Q > using with_pointer = basic_factory< Q, U, Ts... >;

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
               entry2( F c, std::string&& n )
                  : function( c ),
                    name( std::move( n ) )
               {
               }

               F function;
               std::string name;
            };

            template< typename V, template< typename... > class Traits, typename Base, typename F >
            static bool emplace_as( std::map< std::string, entry< F > >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::template key< Traits >(), entry< F >( &W::template as< Traits, Base > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base >
            static P< U > as( const basic_value< Traits, Base >& v )
            {
               using F = P< U > ( * )( const basic_value< Traits, Base >& );
               static const std::map< std::string, entry< F > > m = []() {
                  std::map< std::string, entry< F > > t;
                  (void)json::internal::swallow{ emplace_as< Ts, Traits, Base >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto& a = v.get_object();
               if( a.size() != 1 ) {
                  throw std::runtime_error( "unexpected JSON object size for polymorphic object factory" + json::base_message_extension( v.base() ) );  // NOLINT
               }
               const auto b = a.begin();
               const auto i = m.find( b->first );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + json::internal::escape( b->first ) + json::base_message_extension( v.base() ) );  // NOLINT
               }
               return i->second.function( b->second );
            }

            template< typename V, template< typename... > class Traits, typename Base, typename F >
            static bool emplace_assign( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::type(), entry2< F >( &W::template assign< Traits, Base >, W::template key< Traits >() ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const P< U >& p )
            {
               using F = void ( * )( basic_value< Traits, Base >&, const P< U >& );
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

            template< typename V, template< typename... > class Traits, typename Producer, typename F >
            static bool emplace_consume( std::map< std::string, entry< F > >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::template key< Traits >(), entry< F >( &W::template consume< Traits, Producer > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Producer >
            static P< U > consume( Producer& parser )
            {
               using F = P< U > ( * )( Producer& );
               static const std::map< std::string, entry< F > > m = []() {
                  std::map< std::string, entry< F > > t;
                  (void)json::internal::swallow{ emplace_consume< Ts, Traits, Producer >( t )... };
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

            template< typename V, template< typename... > class Traits, typename Consumer, typename F >
            static bool emplace_produce( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::type(), entry2< F >( &W::template produce< Traits, Consumer >, W::template key< Traits >() ) );
               return true;
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& consumer, const P< U >& p )
            {
               using F = void ( * )( Consumer&, const P< U >& );
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

         template< typename U, typename... Ts > using factory = basic_factory< std::shared_ptr, U, Ts... >;

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_FACTORY_BIND( KeY, ... ) tao::json::binding::internal::factory_temp< TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, KeY ), __VA_ARGS__ >

#define TAO_JSON_FACTORY_BIND1( ... ) tao::json::binding::internal::factory_temp< tao::json::binding::internal::use_default_key, __VA_ARGS__ >

#endif
