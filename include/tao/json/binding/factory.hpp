// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_FACTORY_HPP
#define TAO_JSON_BINDING_FACTORY_HPP

#include <stdexcept>

#include "../forward.hpp"

#include "../basic_value.hpp"
#include "../external/pegtl/internal/pegtl_string.hpp"
#include "../internal/escape.hpp"

#include "member.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename K, typename T, typename U >
         struct factory_type;

         template< char... Cs, typename T, typename U >  // NOLINT
         struct factory_type< key< Cs... >, T, U >
         {
            static const std::type_info* type()
            {
               return &typeid( T );
            }

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

            template< template< typename... > class Traits, typename Producer >
            static std::shared_ptr< U > consume( Producer& parser )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               return R::template consume< Traits >( parser );
            }
            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const std::shared_ptr< U >& p )
            {
               using R = typename Traits< std::shared_ptr< T > >::template with_base< U >;
               R::template produce< Traits >( c, p );
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
                  throw std::runtime_error( "unexpected json object size for polymorphic object factory" );  // NOLINT
               }
               const auto b = a.begin();
               const auto i = m.find( b->first );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown factory type " + json::internal::escape( b->first ) );  // NOLINT
               }
               return i->second.function( b->second );
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

            template< typename V, template< typename... > class Traits, typename Producer, typename F >
            static bool emplace_consume( std::map< std::string, entry< F > >& m )
            {
               using W = typename V::template bind< U >;
               m.emplace( W::name(), entry< F >( &W::template consume< Traits, Producer > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Producer >
            static std::shared_ptr< U > consume( Producer& parser )
            {
               using F = std::shared_ptr< U > ( * )( Producer& );
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

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_FACTORY_BIND( KeY, ... ) tao::json::binding::factory_temp< TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), __VA_ARGS__ >

#endif
