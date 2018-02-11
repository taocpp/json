// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_HASH_HPP
#define TAO_JSON_EVENTS_HASH_HPP

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../byte_view.hpp"
#include "../external/string_view.hpp"
#include "../internal/sha256.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer that calculates an SHA-256 hash.

         class hash
         {
         private:
            std::vector< std::unique_ptr< internal::sha256 > > m_digests;
            std::vector< std::string > m_keys;
            std::vector< std::map< std::string, std::string > > m_properties;

            void push()
            {
               m_digests.emplace_back( new internal::sha256 );
            }

         public:
            hash()
            {
               push();
            }

            hash( const hash& ) = delete;
            hash( hash&& ) = delete;

            ~hash() = default;

            void operator=( const hash& ) = delete;
            void operator=( hash&& ) = delete;

            std::string value() const
            {
               return m_digests.back()->get();
            }

            void reset()
            {
               m_digests.back()->reset();
            }

            void null()
            {
               m_digests.back()->feed( 'n' );
            }

            void boolean( const bool v )
            {
               m_digests.back()->feed( v ? 't' : 'f' );
            }

            void number( const std::int64_t v )
            {
               if( v >= 0 ) {
                  number( static_cast< std::uint64_t >( v ) );
               }
               else {
                  m_digests.back()->feed( 'i' );
                  m_digests.back()->feed( &v, sizeof( v ) );
               }
            }

            void number( const std::uint64_t v )
            {
               m_digests.back()->feed( 'u' );
               m_digests.back()->feed( &v, sizeof( v ) );
            }

            void number( const double v )
            {
               if( v >= 0 ) {
                  const auto u = static_cast< std::uint64_t >( v );
                  if( u == v ) {
                     number( u );
                     return;
                  }
               }
               else {
                  const auto i = static_cast< std::int64_t >( v );
                  if( i == v ) {
                     number( i );
                     return;
                  }
               }
               m_digests.back()->feed( 'd' );
               m_digests.back()->feed( &v, sizeof( v ) );
            }

            void string( const tao::string_view v )
            {
               m_digests.back()->feed( 's' );
               const auto s = v.size();
               m_digests.back()->feed( &s, sizeof( s ) );
               m_digests.back()->feed( v.data(), v.size() );
            }

            void binary( const tao::byte_view v )
            {
               m_digests.back()->feed( 'x' );
               const auto s = v.size();
               m_digests.back()->feed( &s, sizeof( s ) );
               m_digests.back()->feed( v.data(), v.size() );
            }

            void begin_array( const std::size_t /*unused*/ = 0 )
            {
               m_digests.back()->feed( '[' );
            }

            void element()
            {
            }

            void end_array( const std::size_t /*unused*/ = 0 )
            {
               m_digests.back()->feed( ']' );
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               m_digests.back()->feed( '{' );
               m_properties.emplace_back();
               push();
            }

            void key( const tao::string_view v )
            {
               m_digests.back()->feed( v.data(), v.size() );
               m_keys.emplace_back( m_digests.back()->get() );
               if( m_properties.back().count( m_keys.back() ) != 0 ) {
                  throw std::runtime_error( "duplicate JSON object key: " + std::string( v.data(), v.size() ) );  // NOLINT
               }
               m_digests.back()->reset();
            }

            void member()
            {
               m_properties.back().emplace( std::move( m_keys.back() ), m_digests.back()->get() );
               m_keys.pop_back();
               m_digests.back()->reset();
            }

            void end_object( const std::size_t /*unused*/ = 0 )
            {
               m_digests.pop_back();
               for( const auto& e : m_properties.back() ) {
                  m_digests.back()->feed( e.first );
                  m_digests.back()->feed( e.second );
               }
               m_properties.pop_back();
               m_digests.back()->feed( '}' );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
