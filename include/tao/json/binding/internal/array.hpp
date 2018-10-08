// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
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

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

            template< typename... As, std::size_t... Is >
            struct array< json::internal::type_list< As... >, std::index_sequence< Is... > >
            {
               using elements = json::internal::type_list< As... >;

               template< typename A, std::size_t I, template< typename... > class Traits, typename Base, typename C >
               static void to_element( const std::vector< basic_value< Traits, Base > >& a, C& x )
               {
                  A::to( a.at( I ), x );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void to( const basic_value< Traits, Base >& v, C& x )
               {
                  const auto& a = v.get_array();
                  if( a.size() != sizeof...( As ) ) {
                     throw std::runtime_error( json::internal::format( "array size mismatch for type ", typeid( C ), " -- expected ", sizeof...( As ), " received ", a.size(), json::base_message_extension( v.base() ) ) );  // NOLINT
                  }
                  ( to_element< As, Is >( a, x ), ... );
               }

               template< typename A, template< typename... > class Traits, typename Base, typename C >
               static void assign_element( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_back( A::read( x ) );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static void assign( basic_value< Traits, Base >& v, const C& x )
               {
                  v.unsafe_emplace_array();
                  ( assign_element< As >( v, x ), ... );
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

               template< typename A, std::size_t I, template< typename... > class Traits, typename Base, typename C >
               static bool equal_element( const std::vector< basic_value< Traits, Base > >& a, C& x )
               {
                  return a[ I ] == A::read( x );
               }

               template< template< typename... > class Traits, typename Base, typename C >
               static bool equal( const basic_value< Traits, Base >& lhs, const C& rhs ) noexcept
               {
                  const auto& p = lhs.skip_value_ptr();
                  if( bool result = p.is_array() && ( p.unsafe_get_array().size() == sizeof...( As ) ) ) {
                     const auto& a = p.get_array();
                     return ( result && ... && equal_element< As, Is >( a, rhs ) );
                  }
                  return false;
               }
            };

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 )
#pragma GCC diagnostic pop
#endif

         }  // namespace internal

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
