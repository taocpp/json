// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_POINTER_HPP
#define TAOCPP_JSON_INCLUDE_POINTER_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "type.hpp"

#include "external/operators.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         std::size_t token_to_index( const std::string& key );

      }  // namespace internal

      // RFC 6901
      class token
      {
      private:
         std::size_t m_index;
         std::string m_key;

      public:
         explicit token( const std::string& key )
            : m_index( internal::token_to_index( key ) ),
              m_key( key )
         {
         }

         explicit token( std::string&& key )
            : m_index( internal::token_to_index( key ) ),
              m_key( std::move( key ) )
         {
         }

         token( const token& ) = default;
         token( token&& v ) noexcept
            : m_index( v.m_index ),
              m_key( std::move( v.m_key ) )
         {
         }

         token& operator=( const token& ) = default;
         token& operator=( token&& v ) noexcept
         {
            m_index = v.m_index;
            m_key = std::move( v.m_key );
            return *this;
         }

         const std::string& key() const
         {
            return m_key;
         }

         std::size_t index() const
         {
            if( m_index == std::string::npos ) {
               throw std::invalid_argument( "unable to resolve JSON Pointer, invalid token for array access '" + m_key + '\'' );
            }
            return m_index;
         }

         friend bool operator==( const token& lhs, const token& rhs )
         {
            return lhs.m_key == rhs.m_key;
         }

         friend bool operator<( const token& lhs, const token& rhs )
         {
            return lhs.m_key < rhs.m_key;
         }
      };

      class pointer
         : operators::totally_ordered< pointer >
      {
      private:
         std::vector< token > m_tokens;

         static std::vector< token > parse( const std::string& v )
         {
            std::vector< token > result;
            if( !v.empty() ) {
               const char* p = v.data();
               const char* const e = p + v.size();
               if( *p++ != '/' ) {
                  throw std::invalid_argument( "invalid JSON Pointer value, must be empty or begin with '/'" );
               }
               std::string token;
               while( p != e ) {
                  const char c = *p++;
                  switch( c ) {
                     case '~':
                        if( p != e ) {
                           switch( *p++ ) {
                              case '0':
                                 token += '~';
                                 continue;
                              case '1':
                                 token += '/';
                                 continue;
                           }
                        }
                        throw std::invalid_argument( "invalid JSON Pointer escape sequence, '~' must be followed by '0' or '1'" );

                     case '/':
                        result.emplace_back( std::move( token ) );
                        token.clear();
                        continue;

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

         pointer( const pointer& ) = default;

         pointer( pointer&& p ) noexcept
            : m_tokens( std::move( p.m_tokens ) )
         {
         }

         explicit pointer( const std::string& v )
            : m_tokens( parse( v ) )
         {
         }

         pointer& operator=( const pointer& ) = default;

         pointer& operator=( pointer&& p ) noexcept
         {
            m_tokens = std::move( p.m_tokens );
            return *this;
         }

         pointer& operator=( const std::string& v )
         {
            m_tokens = parse( v );
            return *this;
         }

         explicit operator bool() const noexcept
         {
            return !m_tokens.empty();
         }

         std::vector< token >::const_iterator begin() const noexcept
         {
            return m_tokens.begin();
         }

         std::vector< token >::const_iterator end() const noexcept
         {
            return m_tokens.end();
         }

         void push_back( const std::string& v )
         {
            m_tokens.push_back( token( v ) );
         }

         void push_back( std::string&& v )
         {
            m_tokens.push_back( token( std::move( v ) ) );
         }

         pointer& operator+=( const std::string& v )
         {
            push_back( v );
            return *this;
         }

         pointer& operator+=( std::string&& v )
         {
            push_back( std::move( v ) );
            return *this;
         }

         friend bool operator==( const pointer& lhs, const pointer& rhs ) noexcept
         {
            return lhs.m_tokens == rhs.m_tokens;
         }

         friend bool operator<( const pointer& lhs, const pointer& rhs ) noexcept
         {
            return lhs.m_tokens < rhs.m_tokens;
         }
      };

      inline pointer operator+( const pointer& p, const std::string& v )
      {
         pointer nrv( p );
         nrv += v;
         return nrv;
      }

      inline pointer operator+( const pointer& p, std::string&& v )
      {
         pointer nrv( p );
         nrv += std::move( v );
         return nrv;
      }

      namespace internal
      {
         inline std::size_t token_to_index( const std::string& key )
         {
            if( !key.empty() && key.size() <= 20 ) {
               if( key == "0" ) {
                  return 0;
               }
               else if( ( key[ 0 ] != '0' ) && ( key.find_first_not_of( "0123456789" ) == std::string::npos ) ) {
                  if( key.size() < 20 || key < "18446744073709551616" ) {
                     return std::stoull( key );
                  }
               }
            }
            return std::string::npos;
         }

         inline std::string tokens_to_string( std::vector< token >::const_iterator it, const std::vector< token >::const_iterator& end )
         {
            std::string result;
            while( it != end ) {
               result += '/';
               for( const char c : it->key() ) {
                  switch( c ) {
                     case '~':
                        result += "~0";
                        break;
                     case '/':
                        result += "~1";
                        break;
                     default:
                        result += c;
                  }
               }
               ++it;
            }
            return result;
         }

         inline std::string to_string( const pointer& p )
         {
            return tokens_to_string( p.begin(), p.end() );
         }

         inline std::runtime_error invalid_type( const std::vector< token >::const_iterator& begin, const std::vector< token >::const_iterator& end )
         {
            return std::runtime_error( "unable to resolve JSON Pointer '" + tokens_to_string( begin, end ) + "' -- value type is neither 'object' nor 'array'" );
         }

         template< typename T >
         T& pointer_at( T* v, const std::vector< token >::const_iterator& begin, const std::vector< token >::const_iterator& end )
         {
            auto it = begin;
            while( it != end ) {
               switch( v->type() ) {
                  case type::ARRAY:
                     v = &v->at( it->index() );
                     break;
                  case type::OBJECT:
                     v = &v->at( it->key() );
                     break;
                  default:
                     throw invalid_type( begin, std::next( it ) );
               }
               ++it;
            }
            return *v;
         }

      }  // namespace internal

      inline namespace literals
      {
         inline pointer operator"" _json_pointer( const char* data, const std::size_t size )
         {
            return pointer( { data, size } );
         }

      }  // namespace literals

   }  // namespace json

}  // namespace tao

#endif
