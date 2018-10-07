// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_FACTORY_HPP
#define TAO_JSON_BINDING_FACTORY_HPP

#include <bitset>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../forward.hpp"

#include "../basic_value.hpp"
#include "../external/pegtl/internal/pegtl_string.hpp"
#include "../internal/escape.hpp"
#include "../internal/format.hpp"
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
            template< typename... Ts >
            void list_all_types( std::ostream& oss, const std::map< std::string, Ts... >& m )
            {
               for( const auto& i : m ) {
                  json::internal::format_to( oss, " \"", i.first, '\"' );
               }
            }

            template< typename... Ts >
            void list_all_types( std::ostream& oss, const std::map< const std::type_info*, Ts... >& m )
            {
               for( const auto& i : m ) {
                  json::internal::format_to( oss, ' ', *i.first );
               }
            }

            template< typename K, typename T, typename U, template< typename... > class P >
            struct factory_type
               : public type_key< K, T >
            {
               static const std::type_info* type()
               {
                  return &typeid( T );
               }

               template< template< typename... > class Traits, typename Base, typename... With >
               static P< U > as( const basic_value< Traits, Base >& v, With&... with )
               {
                  using R = typename Traits< P< T > >::template with_base< U >;
                  return R::as( v, with... );
               }

               template< template< typename... > class Traits, typename Base >
               static void assign( basic_value< Traits, Base >& v, const P< U >& p )
               {
                  using R = typename Traits< P< T > >::template with_base< U >;
                  R::assign( v, p );
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
                  using R = typename Traits< P< T > >::template with_base< U >;
                  R::template produce< Traits >( c, p );
               }
            };

            template< typename K, typename T >
            struct factory_temp
            {
               template< typename U, template< typename... > class P >
               using bind = factory_type< K, T, U, P >;
            };

         }  // namespace internal

         template< template< typename... > class P, typename... Ts >
         struct basic_factory
         {
            template< template< typename... > class Q > using with_pointer = basic_factory< Q, Ts... >;

            template< typename F >
            struct entry1
            {
               explicit entry1( F c )
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

            template< template< typename... > class Traits, typename Base, typename U, typename... With > using as_func_t = P< U >( * )( const basic_value< Traits, Base >&, With&... );

            template< typename V, template< typename... > class Traits, typename Base, typename U, typename... With >
            static bool emplace_as( std::map< std::string, entry1< as_func_t< Traits, Base, U, With... > > >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::template key< Traits >(), entry1< as_func_t< Traits, Base, U, With... > >( &W::template as< Traits, Base, With... > ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename U, typename... With >
            static void to( const basic_value< Traits, Base >& v, P< U >& r, With&... with )
            {
               static const std::map< std::string, entry1< as_func_t< Traits, Base, U, With... > > > m = []() {
                  std::map< std::string, entry1< as_func_t< Traits, Base, U, With... > > > t;
                  (void)json::internal::swallow{ basic_factory::emplace_as< Ts >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto& a = v.get_object();
               if( a.size() != 1 ) {
                  throw std::runtime_error( json::internal::format( "polymorphic factory requires object of size one for base class ", typeid( U ), json::base_message_extension( v.base() ) ) );  // NOLINT
               }
               const auto b = a.begin();
               const auto i = m.find( b->first );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type \"", json::internal::escape( b->first ), "\" -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( U ), json::base_message_extension( v.base() ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               r = i->second.function( b->second, with... );
            }

            template< typename V, template< typename... > class Traits, typename Base, typename U, typename F >
            static bool emplace_assign( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::type(), entry2< F >( &W::template assign< Traits, Base >, W::template key< Traits >() ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename U >
            static void assign( basic_value< Traits, Base >& v, const P< U >& p )
            {
               using F = void ( * )( basic_value< Traits, Base >&, const P< U >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  (void)json::internal::swallow{ emplace_assign< Ts, Traits, Base, U >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type ", typeid( *p ), " -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( U ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               i->second.function( v, p );
               v = {
                  { i->second.name, std::move( v ) }
               };
            }

            template< typename V, template< typename... > class Traits, typename U, typename Producer, typename F >
            static bool emplace_consume( std::map< std::string, entry1< F > >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::template key< Traits >(), entry1< F >( &W::template consume< Traits, Producer > ) );
               return true;
            }

            template< template< typename... > class Traits, typename U, typename Producer >
            static void consume( Producer& parser, P< U >& r )
            {
               using F = P< U > ( * )( Producer& );
               static const std::map< std::string, entry1< F > > m = []() {
                  std::map< std::string, entry1< F > > t;
                  (void)json::internal::swallow{ emplace_consume< Ts, Traits, U, Producer >( t )... };
                  return t;
               }();

               auto s = parser.begin_object();
               const auto k = parser.key();
               const auto i = m.find( k );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type \"", json::internal::escape( k ), "\" -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( U ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               r = i->second.function( parser );
               parser.end_object( s );
            }

            template< typename V, template< typename... > class Traits, typename U, typename Consumer, typename F >
            static bool emplace_produce( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< U, P >;
               m.emplace( W::type(), entry2< F >( &W::template produce< Traits, Consumer >, W::template key< Traits >() ) );
               return true;
            }

            template< template< typename... > class Traits, typename U, typename Consumer >
            static void produce( Consumer& consumer, const P< U >& p )
            {
               using F = void ( * )( Consumer&, const P< U >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  (void)json::internal::swallow{ emplace_produce< Ts, Traits, U, Consumer >( t )... };
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type ", typeid( *p ), " -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( U ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               consumer.begin_object( 1 );
               consumer.key( i->second.name );
               i->second.function( consumer, p );
               consumer.member();
               consumer.end_object( 1 );
            }
         };

         template< typename... Ts > using factory = basic_factory< std::shared_ptr, Ts... >;

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#define TAO_JSON_FACTORY_BIND( KeY, ... ) tao::json::binding::internal::factory_temp< TAO_JSON_PEGTL_INTERNAL_STRING( tao::json::internal::string_t, KeY ), __VA_ARGS__ >

#define TAO_JSON_FACTORY_BIND1( ... ) tao::json::binding::internal::factory_temp< tao::json::binding::internal::use_default_key, __VA_ARGS__ >

#endif
