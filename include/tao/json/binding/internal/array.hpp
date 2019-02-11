// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_INTERNAL_ARRAY_HPP
#define TAO_JSON_BINDING_INTERNAL_ARRAY_HPP

#include <utility>
#include <vector>

#include "../../basic_value.hpp"
#include "../../forward.hpp"

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
            template< typename T, typename L = std::make_index_sequence< T::size > >
            struct array;

            template< typename... As, std::size_t... Is >
            struct array< json::internal::type_list< As... >, std::index_sequence< Is... > >
            {
               using elements = json::internal::type_list< As... >;

               template< template< typename... > class Traits, typename C >
               static void to( const basic_value< Traits >& v, C& x )
               {
                  const auto& a = v.get_array();
                  if( a.size() != sizeof...( As ) ) {
                     throw std::runtime_error( json::internal::format( "array size mismatch for type ", typeid( C ), " -- expected ", sizeof...( As ), " received ", a.size(), json::message_extension( v ) ) );  // NOLINT
                  }
                  ( As::to( a.at( Is ), x ), ... );
               }

               template< template< typename... > class Traits, typename C >
               static void assign( basic_value< Traits >& v, const C& x )
               {
                  v.unsafe_emplace_array();
                  ( v.unsafe_emplace_back( As::read( x ) ), ... );
               }

               template< typename A, template< typename... > class Traits = traits, typename Producer, typename C, typename State >
               static void consume_element( Producer& parser, C& x, State& s )
               {
                  parser.element( s );
                  A::template consume< Traits >( parser, x );
               }

               template< template< typename... > class Traits = traits, typename Producer, typename C >
               static void consume( Producer& parser, C& x )
               {
                  auto s = parser.begin_array();
                  ( consume_element< As, Traits >( parser, x, s ), ... );
                  parser.end_array( s );
               }

               template< typename A, template< typename... > class Traits, typename Consumer, typename C >
               static void produce_element( Consumer& consumer, const C& x )
               {
                  A::template produce< Traits >( consumer, x );
                  consumer.element();
               }

               template< template< typename... > class Traits = traits, typename Consumer, typename C >
               static void produce( Consumer& consumer, const C& x )
               {
                  consumer.begin_array( sizeof...( As ) );
                  ( produce_element< As, Traits >( consumer, x ), ... );
                  consumer.end_array( sizeof...( As ) );
               }

               template< template< typename... > class Traits, typename C >
               static bool equal( const basic_value< Traits >& lhs, const C& rhs ) noexcept
               {
                  const auto& p = lhs.skip_value_ptr();
                  if( p.is_array() && ( p.unsafe_get_array().size() == sizeof...( As ) ) ) {
                     const auto& a = p.unsafe_get_array();
                     return ( ( a[ Is ] == As::read( rhs ) ) && ... );
                  }
                  return false;
               }
            };

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
