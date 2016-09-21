// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_POINTER_HH
#define TAOCPP_JSON_INCLUDE_POINTER_HH

#include <cstdint>
#include <string>
#include <utility>
#include <stdexcept>
#include <vector>

#include "external/operators.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         std::size_t token_to_index( const std::string & key );

      } // internal

      // RFC 6901
      class token
      {
      private:
         std::size_t m_index;
         std::string m_key;

      public:
         static const std::size_t npos = -1;

         explicit token( const std::string & key )
              : m_index( internal::token_to_index( key ) ),
                m_key( key )
        { }

        explicit token( std::string && key )
             : m_index( internal::token_to_index( key ) ),
               m_key( std::move( key ) )
        { }

        token( const token & ) = default;
        token( token && v ) noexcept
              : m_index( v.m_index ),
                m_key( std::move( v.m_key ) )
        { }

        token & operator= ( const token & ) = default;
        token & operator= ( token && v ) noexcept
        {
           m_index = v.m_index;
           m_key = std::move( v.m_key );
           return * this;
        }

        const std::string & key() const
        {
           return m_key;
        }

        std::size_t index() const
        {
           if ( m_index == npos ) {
              throw std::invalid_argument( "unable to resolve JSON Pointer, invalid token for array access '" + m_key + '\'' );
           }
           return m_index;
        }

        friend bool operator==( const token & lhs, const token & rhs )
        {
           return lhs.m_key == rhs.m_key;
        }

        friend bool operator<( const token & lhs, const token & rhs )
        {
           return lhs.m_key < rhs.m_key;
        }
      };

      class pointer
           : operators::totally_ordered< pointer >
      {
      private:
         std::vector< token > m_tokens;

         static std::vector< token > parse( const std::string & v )
         {
            std::vector< token > result;
            if ( ! v.empty() ) {
               const char * p = v.data();
               const char * const e = p + v.size();
               if ( *p++ != '/' ) {
                  throw std::invalid_argument( "invalid JSON Pointer value, must be empty or begin with '/'" );
               }
               std::string token;
               while ( p != e ) {
                  const auto c = *p++;
                  switch( c ) {
                  case '/':
                     result.emplace_back( std::move( token ) );
                     token.clear();
                     break;
                  case '~':
                     if ( p == e ) {
                        throw std::invalid_argument( "invalid JSON Pointer escape sequence, '~' must be followed by '0' or '1'" );
                     }
                     switch( *p++ ) {
                     case '0':
                        token += '~';
                        break;
                     case '1':
                        token += '/';
                        break;
                     default:
                        throw std::invalid_argument( "invalid JSON Pointer escape sequence, '~' must be followed by '0' or '1'" );
                     }
                     break;
                  default:
                     token += c;
                  }
               }
               result.emplace_back( std::move( token ) );
            }
            return result;
         }

      public:
         pointer() = default;

         pointer( const pointer & ) = default;

         pointer( pointer && p ) noexcept
              : m_tokens( std::move( p.m_tokens ) )
         { }

         explicit pointer( const std::string & v )
              : m_tokens( parse( v ) )
         { }

         pointer & operator= ( const pointer & ) = default;

         pointer & operator= ( pointer && p ) noexcept
         {
            m_tokens = std::move( p.m_tokens );
            return *this;
         }

         pointer & operator= ( const std::string & v )
         {
            m_tokens = parse( v );
            return *this;
         }

         explicit operator bool() const noexcept
         {
            return ! m_tokens.empty();
         }

         std::vector< token >::const_iterator begin() const noexcept
         {
            return m_tokens.begin();
         }

         std::vector< token >::const_iterator end() const noexcept
         {
            return m_tokens.end();
         }

         friend bool operator== ( const pointer & lhs, const pointer & rhs ) noexcept
         {
            return lhs.m_tokens == rhs.m_tokens;
         }

         friend bool operator< ( const pointer & lhs, const pointer & rhs ) noexcept
         {
            return lhs.m_tokens < rhs.m_tokens;
         }
      };

      namespace internal
      {
         inline std::size_t token_to_index( const std::string & key )
         {
            if ( ! key.empty() && key.size() <= 20 ) {
               if ( key == "0" ) {
                  return 0;
               }
               else if ( ( key[ 0 ] != '0' ) && ( key.find_first_not_of( "0123456789" ) == std::string::npos ) ) {
                  try {
                     return std::stoull( key );
                  }
                  catch( ... ) {
                  }
               }
            }
            return token::npos;
         }
      }

      inline namespace literals
      {
         inline pointer operator"" _json_pointer( const char * data, const std::size_t size )
         {
            return pointer( { data, size } );
         }

      } // literals

   } // json

} // tao

#endif
