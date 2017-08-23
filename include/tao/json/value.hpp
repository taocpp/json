// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP
#define TAOCPP_JSON_INCLUDE_CUSTOM_VALUE_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "external/byte.hpp"
#include "external/operators.hpp"
#include "external/optional.hpp"
#include "external/string_view.hpp"

#include "byte_view.hpp"
#include "data.hpp"
#include "pair.hpp"
#include "pointer.hpp"
#include "single.hpp"
#include "traits.hpp"
#include "type.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename, typename, typename = void >
         struct has_as : std::false_type
         {
         };

         template< typename T, typename V >
         struct has_as< T, V, decltype( T::as( std::declval< const V& >() ), void() ) > : std::true_type
         {
         };

      }  // namespace internal

      template< template< typename... > class Traits >
      class basic_custom_value
         : public data
      {
      public:
         using data::data;

         basic_custom_value() noexcept = default;

         basic_custom_value( const basic_custom_value& ) = default;
         basic_custom_value( basic_custom_value&& ) noexcept = default;

         basic_custom_value( basic_custom_value& v )
            : basic_custom_value( static_cast< const basic_custom_value& >( v ) )
         {
         }

         basic_custom_value( const basic_custom_value&& v )
            : basic_custom_value( static_cast< const basic_custom_value& >( v ) )
         {
         }

         basic_custom_value( const data& v )
            : data( v )
         {
         }

         basic_custom_value( data&& v ) noexcept
            : data( std::move( v ) )
         {
         }

         template< typename T >
         basic_custom_value( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_custom_value& >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         basic_custom_value( std::initializer_list< pair< Traits > >&& l )
         {
            unsafe_assign( std::move( l ) );
         }

         basic_custom_value( const std::initializer_list< pair< Traits > >& l )
         {
            unsafe_assign( l );
         }

         basic_custom_value( std::initializer_list< pair< Traits > >& l )
            : basic_custom_value( static_cast< const std::initializer_list< pair< Traits > >& >( l ) )
         {
         }

         static basic_custom_value array( std::initializer_list< single< Traits > >&& l )
         {
            basic_custom_value v;
            v.append( std::move( l ) );
            return v;
         }

         static basic_custom_value array( const std::initializer_list< single< Traits > >& l )
         {
            basic_custom_value v;
            v.append( l );
            return v;
         }

         using data::operator=;

         basic_custom_value& operator=( basic_custom_value v ) noexcept
         {
            data::operator=( std::move( v ) );
            return *this;
         }

         template< typename T >
         void extract( T& v ) const
         {
            Traits< typename std::decay< T >::type >::extract( *this, v );
         }

         template< typename T >
         typename std::enable_if< internal::has_as< Traits< T >, basic_custom_value >::value, T >::type as() const
         {
            return Traits< T >::as( *this );
         }

         template< typename T >
         typename std::enable_if< !internal::has_as< Traits< T >, basic_custom_value >::value, T >::type as() const
         {
            T nrv;
            this->extract( nrv );
            return nrv;
         }

         template< typename T >
         tao::optional< T > optional() const
         {
            if( this->is_null() ) {
               return tao::nullopt;
            }
            else {
               return this->as< T >();
            }
         }

         data& insert( const pointer& k, basic_custom_value value )
         {
            return data::insert( k, std::move( value ) );
         }

         // TODO: If C++14 is enabled, allow other key types...
         template< typename T >
         tao::optional< T > optional( const std::string& key ) const
         {
            TAOCPP_JSON_CHECK_TYPE_ERROR( m_type, json::type::OBJECT );
            const auto it = m_union.o.find( key );
            if( it == m_union.o.end() ) {
               return tao::nullopt;
            }
            else {
               // TODO: This static_cast is probably illegal - use a proxy?
               return static_cast< const basic_custom_value& >( it->second ).template as< T >();
            }
         }

         // The unsafe_assign()-functions MUST NOT be called on a
         // value v when json::needs_discard( v.type() ) is true!

         template< typename T >
         void unsafe_assign( T&& v ) noexcept( noexcept( Traits< typename std::decay< T >::type >::assign( std::declval< basic_custom_value& >(), std::forward< T >( v ) ) ) )
         {
            using D = typename std::decay< T >::type;
            Traits< D >::assign( *this, std::forward< T >( v ) );
         }

         void unsafe_assign( std::initializer_list< pair< Traits > >&& l )
         {
            unsafe_emplace_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void unsafe_assign( const std::initializer_list< pair< Traits > >& l )
         {
            unsafe_emplace_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void unsafe_assign( std::initializer_list< pair< Traits > >& l )
         {
            unsafe_assign( static_cast< const std::initializer_list< pair< Traits > >& >( l ) );
         }

         template< typename V >
         void unsafe_emplace_back( V&& v )
         {
            data::unsafe_emplace_back( basic_custom_value( std::forward< V >( v ) ) );
         }

         template< typename K, typename V >
         std::pair< typename object_t::iterator, bool > unsafe_emplace( K&& k, V&& v )
         {
            return data::unsafe_emplace( std::forward< K >( k ), basic_custom_value( std::forward< V >( v ) ) );
         }

         template< typename V >
         void emplace_back( V&& v )
         {
            prepare_array();
            unsafe_emplace_back( std::forward< V >( v ) );
         }

         template< typename K, typename V >
         std::pair< typename object_t::iterator, bool > emplace( K&& k, V&& v )
         {
            prepare_object();
            return unsafe_emplace( std::forward< K >( k ), std::forward< V >( v ) );
         }

         void append( std::initializer_list< single< Traits > >&& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( auto& e : l ) {
               unsafe_emplace_back( std::move( e.value ) );
            }
         }

         void append( const std::initializer_list< single< Traits > >& l )
         {
            prepare_array();
            auto& v = unsafe_get_array();
            v.reserve( v.size() + l.size() );
            for( const auto& e : l ) {
               unsafe_emplace_back( e.value );
            }
         }

         void insert( std::initializer_list< pair< Traits > >&& l )
         {
            prepare_object();
            for( auto& e : l ) {
               const auto r = unsafe_emplace( std::move( e.key ), std::move( e.value ) );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         void insert( const std::initializer_list< pair< Traits > >& l )
         {
            prepare_object();
            for( const auto& e : l ) {
               const auto r = unsafe_emplace( e.key, e.value );
               if( !r.second ) {
                  throw std::runtime_error( "duplicate JSON object key: " + r.first->first );
               }
            }
         }

         // TODO: more forwarders that fix the return type? or drop this?

         basic_custom_value& unsafe_at( const std::size_t index ) noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::unsafe_at( index ) );
         }

         const basic_custom_value& unsafe_at( const std::size_t index ) const noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::unsafe_at( index ) );
         }

         basic_custom_value& unsafe_at( const std::string& key ) noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::unsafe_at( key ) );
         }

         const basic_custom_value& unsafe_at( const std::string& key ) const noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::unsafe_at( key ) );
         }

         basic_custom_value& at( const std::size_t index )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::at( index ) );
         }

         const basic_custom_value& at( const std::size_t index ) const
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::at( index ) );
         }

         basic_custom_value& at( const std::string& key )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::at( key ) );
         }

         const basic_custom_value& at( const std::string& key ) const
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::at( key ) );
         }

         basic_custom_value& at( const pointer& k )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::at( k ) );
         }

         const basic_custom_value& at( const pointer& k ) const
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::at( k ) );
         }

         basic_custom_value& operator[]( const std::size_t index ) noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::operator[]( index ) );
         }

         const basic_custom_value& operator[]( const std::size_t index ) const noexcept
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< const basic_custom_value& >( data::operator[]( index ) );
         }

         basic_custom_value& operator[]( const std::string& key )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::operator[]( key ) );
         }

         basic_custom_value& operator[]( std::string&& key )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::operator[]( key ) );
         }

         basic_custom_value& operator[]( const pointer& k )
         {
            // TODO: This static_cast is probably illegal - use a proxy?
            return static_cast< basic_custom_value& >( data::operator[]( k ) );
         }
      };

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator+=( basic_custom_value< Traits >& v, std::initializer_list< pair< Traits > >&& l )
      {
         v.insert( std::move( l ) );
         return v;
      }

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator+=( basic_custom_value< Traits >& v, const std::initializer_list< pair< Traits > >& l )
      {
         v.insert( l );
         return v;
      }

      template< template< typename... > class Traits >
      basic_custom_value< Traits >& operator-=( basic_custom_value< Traits >& v, std::initializer_list< std::string > l )
      {
         auto& o = v.get_object();
         for( const auto& k : l ) {
            if( o.erase( k ) == 0 ) {
               throw std::runtime_error( "JSON object key not found: " + k );
            }
         }
         return v;
      }

      using custom_value = basic_custom_value< traits >;

   }  // namespace json

}  // namespace tao

#endif
