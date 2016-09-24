// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_COMPARE_HH
#define TAOCPP_JSON_INCLUDE_SAX_COMPARE_HH

#include <tao/json/value.hh>

#include <vector>
#include <cstddef>
#include <set>
#include <string>
#include <cstdint>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< template< typename ... > class Traits >
         struct sax_compare
         {
            using value_t = basic_value< Traits >;
            std::vector< const value_t * > m_current;
            std::vector< std::size_t > m_array_index;
            std::vector< std::set< std::string > > m_object_keys;
            bool m_match = true;

            const value_t & current() const noexcept
            {
               return * m_current.back();
            }

            void null()
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && current().is_null();
               }
            }

            void boolean( const bool v )
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && ( current() == v );
               }
            }

            void number( const std::int64_t v )
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && ( current() == v );
               }
            }

            void number( const std::uint64_t v )
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && ( current() == v );
               }
            }

            void number( const double v )
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && ( current() == v );
               }
            }

            void string( const std::string & v )
            {
               if ( m_match ) {
                  m_match = ( m_current.back() != nullptr ) && ( current() == v );
               }
            }

            // array
            void begin_array()
            {
               if ( m_current.back() == nullptr )
               {
                  m_match = false;
                  m_current.push_back( nullptr );
               }
               else {
                  const auto & a = current();
                  if ( ! a.is_array() ) {
                     m_match = false;
                     m_current.push_back( nullptr );
                  }
                  else if ( ! a.unsafe_get_array().empty() ) {
                     m_current.push_back( & a.unsafe_get_array().front() );
                  }
                  else {
                     m_current.push_back( nullptr );
                  }
               }
               m_array_index.push_back( 0 );
            }

            void element()
            {
               const auto i = ++m_array_index.back();
               if ( m_match ) {
                  if ( m_current.back() != nullptr ) {
                     const auto & a = ( * ( m_current.end() - 2 ) )->unsafe_get_array();
                     if ( i < a.size() ) {
                        m_current.back() = & a[ i ];
                     }
                     else {
                        m_current.back() = nullptr;
                     }
                  }
               }
            }

            void end_array()
            {
               m_current.pop_back();
               if ( m_match ) {
                  if ( m_array_index.back() != current().unsafe_get_array().size() ) {
                     m_match = false;
                  }
               }
               m_array_index.pop_back();
            }

            // object
            void begin_object()
            {
               if ( m_current.back() == nullptr )
               {
                  m_match = false;
               }
               else {
                  const auto & o = current();
                  if ( ! o.is_object() ) {
                     m_match = false;
                  }
               }
               m_object_keys.emplace_back();
            }

            void key( const std::string & v )
            {
               if ( ! m_object_keys.back().insert( v ).second ) {
                  m_match = false; // duplicate key found! throw?
                  m_current.push_back( nullptr );
               }
               else if ( const auto * p = current().find( v ) ) {
                  m_current.push_back( p );
               }
               else {
                  m_match = false;
                  m_current.push_back( nullptr );
               }
            }

            void member()
            {
               m_current.pop_back();
            }

            void end_object()
            {
               if ( m_match ) {
                  if ( m_object_keys.back().size() != current().unsafe_get_object().size() ) {
                     m_match = false;
                  }
               }
               m_object_keys.pop_back();
            }
         };

      } // internal

      namespace sax
      {
         // SAX consumer that compares against a DOM value
         template< template< typename ... > class Traits >
         class basic_compare : public internal::sax_compare< Traits >
         {
         private:
            using typename internal::sax_compare< Traits >::value_t;
            using internal::sax_compare< Traits >::m_current;
            using internal::sax_compare< Traits >::m_array_index;
            using internal::sax_compare< Traits >::m_object_keys;
            using internal::sax_compare< Traits >::m_match;

            const value_t m_value;

         public:
            explicit basic_compare( const value_t & v )
                 : m_value( v )
            {
               reset();
            }

            explicit basic_compare( value_t && v )
                 : m_value( std::move( v ) )
            {
               reset();
            }

            bool match() const noexcept
            {
               return m_match;
            }

            void reset() noexcept
            {
               m_current.clear();
               m_current.push_back( & m_value );
               m_array_index.clear();
               m_object_keys.clear();
               m_match = true;
            }
         };

         using compare = basic_compare< traits >;

      } // sax

   } // json

} // tao

#endif
