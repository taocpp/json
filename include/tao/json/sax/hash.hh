// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_HASH_HH
#define TAOCPP_JSON_INCLUDE_SAX_HASH_HH

#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <utility>

#include "../internal/digest.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX consumer that calculates an SHA-256 hash
         class hash
         {
         private:
            std::vector< std::unique_ptr< internal::digest > > m_digest;
            std::vector< std::string > m_keys;
            std::vector< std::map< std::string, std::string > > m_properties;

         public:
            void push()
            {
               m_digest.emplace_back( new internal::digest( ::EVP_sha256() ) );
            }

            hash()
            {
               push();
            }

            std::string value() const
            {
               return m_digest.back()->get();
            }

            void null()
            {
               m_digest.back()->feed( "n", 1 );
            }

            void boolean( const bool v )
            {
               m_digest.back()->feed( v ? "t" : "f", 1 );
            }

            void number( const std::int64_t v )
            {
               if ( v >= 0 ) {
                  number( static_cast< std::uint64_t >( v ) );
               }
               else {
                  m_digest.back()->feed( "i", 1 );
                  m_digest.back()->feed( & v, sizeof( v ) );
               }
            }

            void number( const std::uint64_t v )
            {
               m_digest.back()->feed( "u", 1 );
               m_digest.back()->feed( & v, sizeof( v ) );
            }

            void number( const double v )
            {
               const std::uint64_t u = v;
               if ( u == v ) {
                  number( u );
                  return;
               }
               const std::int64_t i = v;
               if ( i == v ) {
                  number( i );
                  return;
               }
               m_digest.back()->feed( "d", 1 );
               m_digest.back()->feed( & v, sizeof( v ) );
            }

            void string( const std::string & v )
            {
               m_digest.back()->feed( "s", 1 );
               const auto s = v.size();
               m_digest.back()->feed( & s, sizeof( s ) );
               m_digest.back()->feed( v );
            }

            // array
            void begin_array()
            {
               m_digest.back()->feed( "[", 1 );
            }

            void element() {}

            void end_array()
            {
               m_digest.back()->feed( "]", 1 );
            }

            // object
            void begin_object()
            {
               m_digest.back()->feed( "{", 1 );
               m_properties.emplace_back();
               push();
            }

            void key( const std::string & v )
            {
               m_digest.back()->feed( v );
               m_keys.emplace_back( m_digest.back()->get() );
               if ( m_properties.back().find( m_keys.back() ) != m_properties.back().end() ) {
                  throw std::runtime_error( "duplicate JSON object key: " + v );
               }
               m_digest.back()->reset();
            }

            void member()
            {
               m_properties.back().emplace( std::move( m_keys.back() ), m_digest.back()->get() );
               m_keys.pop_back();
               m_digest.back()->reset();
            }

            void end_object()
            {
               m_digest.pop_back();
               for ( const auto & e : m_properties.back() ) {
                  m_digest.back()->feed( e.first );
                  m_digest.back()->feed( e.second );
               }
               m_properties.pop_back();
               m_digest.back()->feed( "}", 1 );
            }
         };

      } // sax

   } // json

} // tao

#endif
