// Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_PRODUCE_HPP
#define TAO_JSON_EVENTS_PRODUCE_HPP

#include <initializer_list>
#include <type_traits>
#include <utility>

#include "virtual_base.hpp"

#include "../forward.hpp"

namespace tao::json::events
{
   template< template< typename... > class Traits = traits, typename Consumer, typename T >
   void produce( Consumer& c, T&& t )
   {
      Traits< std::decay_t< T > >::template produce< Traits >( c, std::forward< T >( t ) );
   }

   template< template< typename... > class Traits = traits, typename Consumer = virtual_base >
   struct value;

   template< template< typename... > class Traits, typename Consumer >
   using element = value< Traits, Consumer >;

   template< template< typename... > class Traits = traits, typename Consumer = virtual_base >
   using array = std::initializer_list< element< Traits, Consumer > >;

   template< template< typename... > class Traits, typename Consumer >
   struct member;

   template< template< typename... > class Traits = traits, typename Consumer = virtual_base >
   using object = std::initializer_list< member< Traits, Consumer > >;

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const value< Traits, Consumer >& v )
   {
      v.produce( c );
   }

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const array< Traits, Consumer >& a )
   {
      c.begin_array( a.size() );
      for( const auto& e : a ) {
         e.produce( c );
         c.element();
      }
      c.end_array( a.size() );
   }

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const object< Traits, Consumer >& o )
   {
      c.begin_object( o.size() );
      for( const auto& m : o ) {
         c.key( m.m_key );
         m.produce( c );
         c.member();
      }
      c.end_object( o.size() );
   }

   template< template< typename... > class Traits, typename Consumer >
   struct value
   {
   private:
      const void* const m_value;

      using producer_t = void( Consumer&, const void* );
      producer_t* const m_producer;

   public:
      template< typename T >
      value( const T& v ) noexcept
         : m_value( &v ),
           m_producer( []( Consumer& c, const void* d ) {
              events::produce< Traits >( c, *static_cast< const T* >( d ) );
           } )
      {}

      value( const object< Traits, Consumer >& il ) noexcept
         : m_value( &il ),
           m_producer( []( Consumer& c, const void* d ) {
              events::produce( c, *static_cast< const object< Traits, Consumer >* >( d ) );
           } )
      {}

      void produce( Consumer& c ) const
      {
         m_producer( c, m_value );
      }
   };

   template< template< typename... > class Traits, typename Consumer >
   struct member
      : value< Traits, Consumer >
   {
      const std::string_view m_key;

      template< typename T >
      member( const std::string_view k, const T& v ) noexcept
         : value< Traits, Consumer >( v ),
           m_key( k )
      {}

      template< typename T >
      member( const T& v ) noexcept
         : member( Traits< std::decay_t< T > >::template default_key< Traits >::as_string_view(), v )
      {}

      member( const std::string_view k, const object< Traits, Consumer >& il ) noexcept
         : value< Traits, Consumer >( il ),
           m_key( k )
      {}
   };

}  // namespace tao::json::events

#endif
