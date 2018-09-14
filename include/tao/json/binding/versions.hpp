// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_VERSIONS_HPP
#define TAO_JSON_BINDING_VERSIONS_HPP

#include <exception>
#include <stdexcept>

#include "../forward.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename... Vs >
         struct versions;

         template< typename V >
         struct versions< V >
            : public V
         {
         };

         template< typename V, typename... Vs >
         struct versions< V, Vs... >
            : public V
         {
            // TODO: Clear x between attempts? How?

            static void throw_on_error( const bool ok, const std::exception_ptr& e )
            {
               if( !ok ) {
                  try {
                     std::rethrow_exception( e );  // TODO: Did I miss a way to avoid the throw?
                  }
                  catch( ... ) {
                     std::throw_with_nested( std::runtime_error( "all variants failed -- see nested for first error" ) );
                  }
               }
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static std::exception_ptr first_to( const basic_value< Traits, Base >& v, C& x )
            {
               try {
                  V::to( v, x );
                  return std::exception_ptr();
               }
               catch( ... ) {
                  return std::current_exception();
               }
            }

            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool attempt_to( const basic_value< Traits, Base >& v, C& x )
            {
               try {
                  A::to( v, x );
                  return true;
               }
               catch( ... ) {
                  return false;
               }
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void to( const basic_value< Traits, Base >& v, C& x )
            {
               const std::exception_ptr e = first_to( v, x );
               bool ok = ( e == std::exception_ptr() );
               (void)json::internal::swallow{ ( ok = ok || attempt_to< Vs >( v, x ) )... };
               throw_on_error( ok, e );
            }

            template< template< typename... > class Traits, typename Producer, typename C >
            static std::exception_ptr first_consume( Producer& parser, C& x )
            {
               try {
                  auto m = parser.mark();
                  V::template consume< Traits >( parser, x );
                  m( true );
                  return std::exception_ptr();
               }
               catch( ... ) {
                  return std::current_exception();
               }
            }

            template< typename A, template< typename... > class Traits, typename Producer, typename C >
            static bool attempt_consume( Producer& parser, C& x )
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

            template< template< typename... > class Traits, typename Producer, typename C >
            static void consume( Producer& parser, C& x )
            {
               const std::exception_ptr e = first_consume< Traits >( parser, x );
               bool ok = ( e == std::exception_ptr() );
               (void)json::internal::swallow{ ( ok = ok || attempt_consume< Vs, Traits >( parser, x ) )... };
               throw_on_error( ok, e );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
