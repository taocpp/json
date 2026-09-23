// Copyright (c) 2016-2023 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_SET_VALUE_HPP
#define TAO_JSON_EVENTS_SET_VALUE_HPP

#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../binary_view.hpp"
#include "../forward.hpp"
#include "../type.hpp"

namespace tao::json::events
{
   // Events consumer to build a JSON Value.

   template< template< typename... > class Traits >
   struct set_basic_value
   {
      static constexpr size_t count_ = 42;
      static constexpr size_t count_skip_bit_ = size_t{ 1 } << ( std::numeric_limits< size_t >::digits - 1 );
      struct InPlaceVector
      {
         union
         {
            basic_value< Traits > array[ count_ ];
         };
         size_t count = 0;
         InPlaceVector() {}
         ~InPlaceVector()
         {
            std::destroy_n( array, size() );
         }
         void emplace_back( basic_value< Traits >&& v )
         {
            ::new( array + count ) basic_value< Traits >{ std::move( v ) };
            ++count;
         }
         size_t size() const
         {
            return count & ~count_skip_bit_;
         }
      };
      std::vector< basic_value< Traits > > stack_;
      std::deque< InPlaceVector > elements_;
      std::vector< std::string > keys_;
      basic_value< Traits >& value_;

      set_basic_value( basic_value< Traits >& v ) noexcept
         : value_( v )
      {}

      void reset() noexcept
      {
         value_.set_uninitialized();
         stack_.clear();
         keys_.clear();
      }

      void reset( basic_value< Traits > v ) noexcept
      {
         value_ = std::move( v );
         stack_.clear();
         keys_.clear();
      }

      void null() noexcept
      {
         value_.set_null();
      }

      void boolean( const bool v ) noexcept
      {
         value_.set_boolean( v );
      }

      void number( const std::int64_t v ) noexcept
      {
         value_.set_signed( v );
      }

      void number( const std::uint64_t v ) noexcept
      {
         value_.set_unsigned( v );
      }

      void number( const double v ) noexcept
      {
         value_.set_double( v );
      }

      void string( const std::string_view v )
      {
         value_.emplace_string( v );
      }

      void string( const char* v )
      {
         value_.emplace_string( v );
      }

      void string( std::string&& v )
      {
         value_.emplace_string( std::move( v ) );
      }

      void binary( const tao::binary_view v )
      {
         value_.emplace_binary( v.begin(), v.end() );
      }

      void binary( std::vector< std::byte >&& v )
      {
         value_.emplace_binary( std::move( v ) );
      }

      void begin_array()
      {
         stack_.push_back( empty_array );
         elements_.emplace_back();
      }

      void begin_array( const std::size_t size )
      {
         begin_array();
         elements_.back().count |= count_skip_bit_;
         stack_.back().get_array().reserve( size );
      }

      void element()
      {
         auto& elements = elements_.back();
         if( elements.count < count_ ) {
            elements.emplace_back( std::move( value_ ) );
            return;
         }
         auto& a = stack_.back().get_array();
         if( elements.count == count_ ) {
            a.reserve( count_ + 1 );
            a.resize( count_ );
            elements.count |= count_skip_bit_;
         }
         a.push_back( std::move( value_ ) );
      }

      void end_array( const std::size_t /*unused*/ = 0 )
      {
         auto& elements = elements_.back();
         auto& v = stack_.back();
         if( const size_t size = elements.size() ) {
            auto& a = v.get_array();
            if( a.empty() ) {
               a.assign( std::make_move_iterator( elements.array ),
                         std::make_move_iterator( elements.array + size ) );
            }
            else {
               std::copy_n( std::make_move_iterator( elements.array ),
                            size,
                            a.begin() );
            }
         }
         value_ = std::move( v );
         stack_.pop_back();
         elements_.pop_back();
      }

      void begin_object( const std::size_t /*unused*/ = 0 )
      {
         stack_.push_back( empty_object );
      }

      void key( const std::string_view v )
      {
         keys_.emplace_back( v );
      }

      void key( const char* v )
      {
         keys_.emplace_back( v );
      }

      void key( std::string&& v )
      {
         keys_.emplace_back( std::move( v ) );
      }

      void member()
      {
         stack_.back().try_emplace( std::move( keys_.back() ), std::move( value_ ) );
         keys_.pop_back();
      }

      void end_object( const std::size_t /*unused*/ = 0 )
      {
         value_ = std::move( stack_.back() );
         stack_.pop_back();
      }
   };

   using set_value = set_basic_value< traits >;

}  // namespace tao::json::events

#endif
