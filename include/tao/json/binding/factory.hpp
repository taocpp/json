// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
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

            template< typename K, typename T, typename Base, template< typename... > class Pointer >
            struct factory_type
               : public type_key< K, T >
            {
               static const std::type_info* type()
               {
                  return &typeid( T );
               }

               template< template< typename... > class Traits, typename... With >
               static Pointer< Base > as( const basic_value< Traits >& v, With&... with )
               {
                  using R = typename Traits< Pointer< T > >::template with_base< Base >;
                  return R::as( v, with... );
               }

               template< template< typename... > class Traits >
               static void assign( basic_value< Traits >& v, const Pointer< Base >& p )
               {
                  using R = typename Traits< Pointer< T > >::template with_base< Base >;
                  R::assign( v, p );
               }

               template< template< typename... > class Traits, typename Producer >
               static Pointer< Base > consume( Producer& parser )
               {
                  using R = typename Traits< Pointer< T > >::template with_base< Base >;
                  return R::template consume< Traits >( parser );
               }

               template< template< typename... > class Traits, typename Consumer >
               static void produce( Consumer& c, const Pointer< Base >& p )
               {
                  using R = typename Traits< Pointer< T > >::template with_base< Base >;
                  R::template produce< Traits >( c, p );
               }
            };

            template< typename K, typename T >
            struct factory_temp
            {
               template< typename Base, template< typename... > class Pointer >
               using bind = factory_type< K, T, Base, Pointer >;
            };

         }  // namespace internal

         template< typename... Ts >
         struct factory
         {
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

            template< template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename... With >
            using as_func_t = Pointer< Base > ( * )( const basic_value< Traits >&, With&... );

            template< typename V, template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename... With >
            static void emplace_as( std::map< std::string, entry1< as_func_t< Traits, Pointer, Base, With... > > >& m )
            {
               using W = typename V::template bind< Base, Pointer >;
               m.emplace( W::template key< Traits >(), entry1< as_func_t< Traits, Pointer, Base, With... > >( &W::template as< Traits, With... > ) );
            }

            template< template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename... With >
            static void to( const basic_value< Traits >& v, Pointer< Base >& r, With&... with )
            {
               static const std::map< std::string, entry1< as_func_t< Traits, Pointer, Base, With... > > > m = []() {
                  std::map< std::string, entry1< as_func_t< Traits, Pointer, Base, With... > > > t;
                  ( emplace_as< Ts >( t ), ... );
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto& a = v.get_object();
               if( a.size() != 1 ) {
                  throw std::runtime_error( json::internal::format( "polymorphic factory requires object of size one for base class ", typeid( Base ), json::message_extension( v ) ) );  // NOLINT
               }
               const auto b = a.begin();
               const auto i = m.find( b->first );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type \"", json::internal::escape( b->first ), "\" -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( Base ), json::message_extension( v ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               r = i->second.function( b->second, with... );
            }

            template< typename V, template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename F >
            static void emplace_assign( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< Base, Pointer >;
               m.emplace( W::type(), entry2< F >( &W::template assign< Traits >, W::template key< Traits >() ) );
            }

            template< template< typename... > class Traits, template< typename... > class Pointer, typename Base >
            static void assign( basic_value< Traits >& v, const Pointer< Base >& p )
            {
               using F = void ( * )( basic_value< Traits >&, const Pointer< Base >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  ( emplace_assign< Ts, Traits, Pointer, Base >( t ), ... );
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type ", typeid( *p ), " -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( Base ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               i->second.function( v, p );
               v = {
                  { i->second.name, std::move( v ) }
               };
            }

            template< typename V, template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename Producer, typename F >
            static void emplace_consume( std::map< std::string, entry1< F > >& m )
            {
               using W = typename V::template bind< Base, Pointer >;
               m.emplace( W::template key< Traits >(), entry1< F >( &W::template consume< Traits, Producer > ) );
            }

            template< template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename Producer >
            static void consume( Producer& parser, Pointer< Base >& r )
            {
               using F = Pointer< Base > ( * )( Producer& );
               static const std::map< std::string, entry1< F > > m = []() {
                  std::map< std::string, entry1< F > > t;
                  ( emplace_consume< Ts, Traits, Pointer, Base, Producer >( t ), ... );
                  return t;
               }();

               auto s = parser.begin_object();
               const auto k = parser.key();
               const auto i = m.find( k );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type \"", json::internal::escape( k ), "\" -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( Base ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
               }
               r = i->second.function( parser );
               parser.end_object( s );
            }

            template< typename V, template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename Consumer, typename F >
            static bool emplace_produce( std::map< const std::type_info*, entry2< F >, json::internal::type_info_less >& m )
            {
               using W = typename V::template bind< Base, Pointer >;
               m.emplace( W::type(), entry2< F >( &W::template produce< Traits, Consumer >, W::template key< Traits >() ) );
               return true;
            }

            template< template< typename... > class Traits, template< typename... > class Pointer, typename Base, typename Consumer >
            static void produce( Consumer& consumer, const Pointer< Base >& p )
            {
               using F = void ( * )( Consumer&, const Pointer< Base >& );
               static const std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > m = []() {
                  std::map< const std::type_info*, entry2< F >, json::internal::type_info_less > t;
                  ( emplace_produce< Ts, Traits, Pointer, Base, Consumer >( t ), ... );
                  assert( t.size() == sizeof...( Ts ) );
                  return t;
               }();

               const auto i = m.find( &typeid( *p ) );
               if( i == m.end() ) {
                  std::ostringstream oss;
                  json::internal::format_to( oss, "unknown factory type ", typeid( *p ), " -- known are" );
                  internal::list_all_types( oss, m );
                  json::internal::format_to( oss, " for base class ", typeid( Base ) );
                  throw std::runtime_error( oss.str() );  // NOLINT
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

#endif
