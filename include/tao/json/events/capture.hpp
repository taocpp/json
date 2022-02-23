// Copyright (c) 2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_CAPTURE_HPP
#define TAO_JSON_EVENTS_CAPTURE_HPP

#include <initializer_list>
#include <string_view>
#include <type_traits>

#include "produce.hpp"
#include "virtual_base.hpp"

#include "../forward.hpp"
#include "../internal/dependent_false.hpp"

namespace tao::json::events
{
   namespace capture
   {
      template< template< typename... > class Traits = json::traits, typename Consumer = virtual_base >
      class value;

      template< template< typename... > class Traits = json::traits, typename Consumer = virtual_base >
      using array = std::initializer_list< value< Traits, Consumer > >;

      template< template< typename... > class Traits, typename Consumer >
      struct member;

      template< template< typename... > class Traits = json::traits, typename Consumer = virtual_base >
      using object = std::initializer_list< member< Traits, Consumer > >;

   }  // namespace capture

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const capture::value< Traits, Consumer >& v )
   {
      v.produce( c );
   }

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const capture::array< Traits, Consumer >& a )
   {
      c.begin_array( a.size() );
      for( const auto& e : a ) {
         e.produce( c );
         c.element();
      }
      c.end_array( a.size() );
   }

   template< template< typename... > class Traits, typename ActualConsumer, typename Consumer >
   void produce( ActualConsumer& c, const capture::object< Traits, Consumer >& o )
   {
      c.begin_object( o.size() );
      for( const auto& m : o ) {
         c.key( m.key );
         m.produce( c );
         c.member();
      }
      c.end_object( o.size() );
   }

   namespace capture
   {
      union union_t
      {
         explicit union_t( const void* v ) noexcept
            : p( v )
         {}

         explicit union_t( const bool v ) noexcept
            : b( v )
         {}

         explicit union_t( const char v ) noexcept
            : c( v )
         {}

         explicit union_t( const signed char v ) noexcept
            : sc( v )
         {}

         explicit union_t( const unsigned char v ) noexcept
            : uc( v )
         {}

         explicit union_t( const signed short v ) noexcept
            : ss( v )
         {}

         explicit union_t( const unsigned short v ) noexcept
            : us( v )
         {}

         explicit union_t( const signed int v ) noexcept
            : si( v )
         {}

         explicit union_t( const unsigned int v ) noexcept
            : ui( v )
         {}

         explicit union_t( const signed long v ) noexcept
            : sl( v )
         {}

         explicit union_t( const unsigned long v ) noexcept
            : ul( v )
         {}

         explicit union_t( const signed long long v ) noexcept
            : sll( v )
         {}

         explicit union_t( const unsigned long long v ) noexcept
            : ull( v )
         {}

         explicit union_t( const float v ) noexcept
            : f( v )
         {}

         explicit union_t( const double v ) noexcept
            : d( v )
         {}

         const void* p;
         bool b;
         char c;
         signed char sc;
         unsigned char uc;
         signed short ss;
         unsigned short us;
         signed int si;
         unsigned int ui;
         signed long sl;
         unsigned long ul;
         signed long long sll;
         unsigned long long ull;
         float f;
         double d;

         template< typename T >
         [[nodiscard]] T get() const noexcept
         {
            if constexpr( std::is_same_v< T, bool > ) {
               return b;
            }
            else if constexpr( std::is_same_v< T, char > ) {
               return c;
            }
            else if constexpr( std::is_same_v< T, signed char > ) {
               return sc;
            }
            else if constexpr( std::is_same_v< T, unsigned char > ) {
               return uc;
            }
            else if constexpr( std::is_same_v< T, signed short > ) {
               return ss;
            }
            else if constexpr( std::is_same_v< T, unsigned short > ) {
               return us;
            }
            else if constexpr( std::is_same_v< T, signed int > ) {
               return si;
            }
            else if constexpr( std::is_same_v< T, unsigned int > ) {
               return ui;
            }
            else if constexpr( std::is_same_v< T, signed long > ) {
               return sl;
            }
            else if constexpr( std::is_same_v< T, unsigned long > ) {
               return ul;
            }
            else if constexpr( std::is_same_v< T, signed long long > ) {
               return sll;
            }
            else if constexpr( std::is_same_v< T, unsigned long long > ) {
               return ull;
            }
            else if constexpr( std::is_same_v< T, float > ) {
               return f;
            }
            else if constexpr( std::is_same_v< T, double > ) {
               return d;
            }
            else {
               static_assert( internal::dependent_false< T >, "invalid type T" );
            }
         }
      };

      template< typename, typename = void >
      struct traits
         : std::false_type
      {};

      template< typename T >
      struct traits< T, std::enable_if_t< std::is_enum_v< T > > >
         : std::true_type
      {
         using underlying_t = std::underlying_type_t< T >;

         [[nodiscard]] static auto to_underlying( const T v ) noexcept
         {
            return static_cast< underlying_t >( v );
         }

         [[nodiscard]] static T from_underlying( const union_t& v ) noexcept
         {
            return static_cast< T >( v.get< underlying_t >() );
         }
      };

      template< template< typename... > class Traits, typename Consumer >
      class value
      {
      private:
         const union_t m_value;

         using producer_t = void( Consumer&, const union_t& );
         producer_t* const m_producer;

      public:
         value( const bool v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.b );
              } )
         {}

         value( const char v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.c );
              } )
         {}

         value( const signed char v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.sc );
              } )
         {}

         value( const unsigned char v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.uc );
              } )
         {}

         value( const signed short v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.ss );
              } )
         {}

         value( const unsigned short v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.us );
              } )
         {}

         value( const signed int v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.si );
              } )
         {}

         value( const unsigned int v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.ui );
              } )
         {}

         value( const signed long v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.sl );
              } )
         {}

         value( const unsigned long v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.ul );
              } )
         {}

         value( const signed long long v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.sll );
              } )
         {}

         value( const unsigned long long v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.ull );
              } )
         {}

         value( const float v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.f );
              } )
         {}

         value( const double v ) noexcept
            : m_value( v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, u.d );
              } )
         {}

         template< typename T, typename = std::enable_if_t< traits< T >::value > >
         value( const T v ) noexcept
            : m_value( traits< T >::to_underlying( v ) ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, traits< T >::from_underlying( u ) );
              } )
         {
            static_assert( noexcept( traits< T >::to_underlying( v ) ) );
         }

         template< typename T, typename = std::enable_if_t< !traits< T >::value > >
         value( const T& v ) noexcept
            : m_value( &v ),
              m_producer( []( Consumer& c, const union_t& u ) {
                 events::produce< Traits >( c, *static_cast< const T* >( u.p ) );
              } )
         {}

         value( const object< Traits, Consumer >& il ) noexcept
            : m_value( &il ),
              m_producer( []( Consumer& c, const union_t& v ) {
                 events::produce( c, *static_cast< const object< Traits, Consumer >* >( v.p ) );
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
         const std::string_view key;

         template< typename T >
         member( const std::string_view k, const T& v ) noexcept
            : value< Traits, Consumer >( v ),
              key( k )
         {}

         template< typename T >
         member( const T& v ) noexcept
            : member( Traits< std::decay_t< T > >::template default_key< Traits >::as_string_view(), v )
         {}

         member( const std::string_view k, const object< Traits, Consumer >& il ) noexcept
            : value< Traits, Consumer >( il ),
              key( k )
         {}
      };

   }  // namespace capture

}  // namespace tao::json::events

#endif
