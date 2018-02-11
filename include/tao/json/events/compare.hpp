// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_COMPARE_HPP
#define TAO_JSON_EVENTS_COMPARE_HPP

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "../value.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename... > class Traits, typename Base >
         class events_compare
         {
         protected:
            std::vector< const basic_value< Traits, Base >* > m_current;
            std::vector< std::size_t > m_array_index;
            // TODO: use std::unordered_set? or even std::vector!?
            std::vector< std::set< const basic_value< Traits, Base >* > > m_object_keys;
            bool m_match = true;

         public:
            events_compare() = default;

            events_compare( const events_compare& ) = delete;
            events_compare( events_compare&& ) = delete;

            ~events_compare() = default;

            void operator=( const events_compare& ) = delete;
            void operator=( events_compare&& ) = delete;

            void reset() noexcept
            {
               m_current.clear();
               m_array_index.clear();
               m_object_keys.clear();
               m_match = true;
            }

            static const basic_value< Traits, Base >* skip_pointer( const basic_value< Traits, Base >* p ) noexcept
            {
               while( p && p->is_raw_ptr() ) {
                  p = p->unsafe_get_raw_ptr();
               }
               return p;
            }

            void push( const basic_value< Traits, Base >* p )
            {
               m_current.push_back( skip_pointer( p ) );
            }

            bool match() const noexcept
            {
               return m_match;
            }

            const basic_value< Traits, Base >& current() const noexcept
            {
               return *m_current.back();
            }

            void null() noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current().is_null() );
            }

            void boolean( const bool v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void number( const std::int64_t v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void number( const std::uint64_t v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void number( const double v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void string( const tao::string_view v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void binary( const tao::byte_view v ) noexcept
            {
               m_match = m_match && ( m_current.back() != nullptr ) && ( current() == v );
            }

            void begin_array( const std::size_t /*unused*/ = 0 )
            {
               if( m_current.back() == nullptr ) {
                  m_match = false;
                  m_current.push_back( nullptr );
               }
               else {
                  const auto& a = current();
                  if( !a.is_array() ) {
                     m_match = false;
                     m_current.push_back( nullptr );
                  }
                  else if( !a.unsafe_get_array().empty() ) {
                     push( &a.unsafe_get_array().front() );
                  }
                  else {
                     m_current.push_back( nullptr );
                  }
               }
               m_array_index.push_back( 0 );
            }

            void element() noexcept
            {
               const auto i = ++m_array_index.back();
               if( m_match ) {
                  if( m_current.back() != nullptr ) {
                     const auto& a = ( *( m_current.end() - 2 ) )->unsafe_get_array();
                     if( i < a.size() ) {
                        m_current.back() = skip_pointer( &a[ i ] );
                     }
                     else {
                        m_current.back() = nullptr;
                     }
                  }
               }
            }

            void end_array( const std::size_t /*unused*/ = 0 ) noexcept
            {
               m_current.pop_back();
               if( m_match ) {
                  if( m_array_index.back() != current().unsafe_get_array().size() ) {
                     m_match = false;
                  }
               }
               m_array_index.pop_back();
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               if( m_current.back() == nullptr ) {
                  m_match = false;
               }
               else {
                  const auto& o = current();
                  if( !o.is_object() ) {
                     m_match = false;
                  }
               }
               m_object_keys.emplace_back();
            }

            void key( const std::string& v )
            {
               if( !m_match ) {
                  m_current.push_back( nullptr );
               }
               else if( const auto* p = current().unsafe_find( v ) ) {
                  if( !m_object_keys.back().insert( p ).second ) {
                     m_match = false;  // duplicate key found! -> fail
                     m_current.push_back( nullptr );
                  }
                  else {
                     push( p );
                  }
               }
               else {
                  m_match = false;
                  m_current.push_back( nullptr );
               }
            }

            void key( const tao::string_view v )
            {
               key( std::string( v.data(), v.size() ) );
            }

            void key( const char* v )
            {
               key( std::string( v ) );
            }

            void member() noexcept
            {
               m_current.pop_back();
            }

            void end_object( const std::size_t /*unused*/ = 0 ) noexcept
            {
               if( m_match ) {
                  if( m_object_keys.back().size() != current().unsafe_get_object().size() ) {
                     m_match = false;
                  }
               }
               m_object_keys.pop_back();
            }
         };

      }  // namespace internal

      namespace events
      {
         // Events consumer that compares against a JSON Value.

         template< template< typename... > class Traits, typename Base = internal::empty_base >
         class basic_compare
            : public internal::events_compare< Traits, Base >
         {
         private:
            const basic_value< Traits, Base > m_value;

         public:
            explicit basic_compare( const basic_value< Traits, Base >& v )
               : m_value( v )
            {
               reset();
            }

            explicit basic_compare( basic_value< Traits, Base >&& v )
               : m_value( std::move( v ) )
            {
               reset();
            }

            basic_compare( const basic_compare& ) = delete;
            basic_compare( basic_compare&& ) = delete;

            ~basic_compare() = default;

            void operator=( const basic_compare& ) = delete;
            void operator=( basic_compare&& ) = delete;

            void reset()
            {
               internal::events_compare< Traits, Base >::reset();
               internal::events_compare< Traits, Base >::push( &m_value );
            }
         };

         using compare = basic_compare< traits >;

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
