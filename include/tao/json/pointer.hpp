// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_POINTER_HPP
#define TAO_JSON_POINTER_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "type.hpp"

#include "external/pegtl.hpp"
#include "internal/format.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< std::size_t = sizeof( std::size_t ) >
         struct token_to_index;

         template<>
         struct token_to_index< 4 >
         {
            static std::size_t convert( const std::string& key ) noexcept
            {
               if( !key.empty() && key.size() <= 10 ) {
                  if( key == "0" ) {
                     return 0;
                  }
                  if( ( key[ 0 ] != '0' ) && ( key.find_first_not_of( "0123456789" ) == std::string::npos ) ) {
                     if( key.size() < 10 || key < "4294967296" ) {
                        return static_cast< std::size_t >( std::stoul( key ) );
                     }
                  }
               }
               return std::string::npos;
            }
         };

         template<>
         struct token_to_index< 8 >
         {
            static std::size_t convert( const std::string& key ) noexcept
            {
               if( !key.empty() && key.size() <= 20 ) {
                  if( key == "0" ) {
                     return 0;
                  }
                  if( ( key[ 0 ] != '0' ) && ( key.find_first_not_of( "0123456789" ) == std::string::npos ) ) {
                     if( key.size() < 20 || key < "18446744073709551616" ) {
                        return static_cast< std::size_t >( std::stoull( key ) );
                     }
                  }
               }
               return std::string::npos;
            }
         };

      }  // namespace internal

      // RFC 6901
      class token
      {
      private:
         std::size_t m_index;
         std::string m_key;

      public:
         explicit token( const std::string& in_key )
            : m_index( internal::token_to_index<>::convert( in_key ) ),
              m_key( in_key )
         {
         }

         explicit token( std::string&& in_key ) noexcept
            : m_index( internal::token_to_index<>::convert( in_key ) ),
              m_key( std::move( in_key ) )
         {
         }

         explicit token( const std::size_t in_index )
            : m_index( in_index ),
              m_key( std::to_string( m_index ) )
         {
         }

         token( const token& ) = default;
         token( token&& v ) noexcept
            : m_index( v.m_index ),
              m_key( std::move( v.m_key ) )
         {
         }

         ~token() = default;

         token& operator=( const token& ) = default;
         token& operator=( token&& v ) noexcept
         {
            m_index = v.m_index;
            m_key = std::move( v.m_key );
            return *this;
         }

         const std::string& key() const noexcept
         {
            return m_key;
         }

         std::size_t index() const
         {
            if( m_index == std::string::npos ) {
               throw std::invalid_argument( internal::format( "unable to resolve json pointer with array, token '", m_key, "' is not an index" ) );  // NOLINT
            }
            return m_index;
         }

         friend bool operator==( const token& lhs, const token& rhs ) noexcept
         {
            return lhs.m_key == rhs.m_key;
         }

         friend bool operator<( const token& lhs, const token& rhs ) noexcept
         {
            return lhs.m_key < rhs.m_key;
         }
      };

      namespace internal
      {
         // clang-format off
         struct pointer_zero : json_pegtl::one< '0' > {};
         struct pointer_one : json_pegtl::one< '1' > {};
         struct pointer_tilde : json_pegtl::one< '~' > {};
         struct pointer_escaped : json_pegtl::sor< pointer_zero, pointer_one > {};
         struct pointer_slash : json_pegtl::one< '/' > {};
         struct pointer_char : json_pegtl::utf8::not_one< '/' > {};
         struct pointer_escape : json_pegtl::if_must< pointer_tilde, pointer_escaped > {};
         struct pointer_token : json_pegtl::star< json_pegtl::sor< pointer_escape, pointer_char > > {};
         struct pointer_rule : json_pegtl::until< json_pegtl::eof, json_pegtl::must< pointer_slash, pointer_token > > {};
         struct pointer_grammar : json_pegtl::must< pointer_rule > {};
         // clang-format on

         template< typename Rule >
         struct pointer_action
            : public json_pegtl::nothing< Rule >
         {
         };

         template<>
         struct pointer_action< pointer_zero >
         {
            static void apply0( std::vector< token >& /*unused*/, std::string& t )
            {
               t += '~';
            }
         };

         template<>
         struct pointer_action< pointer_one >
         {
            static void apply0( std::vector< token >& /*unused*/, std::string& t )
            {
               t += '/';
            }
         };

         template<>
         struct pointer_action< pointer_char >
         {
            template< typename Input >
            static void apply( const Input& in, std::vector< token >& /*unused*/, std::string& t )
            {
               t.append( in.begin(), in.size() );
            }
         };

         template<>
         struct pointer_action< pointer_token >
         {
            static void apply0( std::vector< token >& v, std::string& t )
            {
               v.push_back( token( std::move( t ) ) );
               t.clear();
            }
         };

      }  // namespace internal

      class pointer
      {
      private:
         std::vector< token > m_tokens;

         void parse( const std::string& v )
         {
            std::string t;
            json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, const char* > in( v, __PRETTY_FUNCTION__ );
            json_pegtl::parse< internal::pointer_grammar, internal::pointer_action >( in, m_tokens, t );
         }

      public:
         pointer() = default;

         pointer( const pointer& ) = default;

         pointer( pointer&& p ) noexcept
            : m_tokens( std::move( p.m_tokens ) )
         {
         }

         explicit pointer( const std::string& v )
         {
            parse( v );
         }

         pointer( const std::initializer_list< token >& l )
            : m_tokens( l )
         {
         }

         ~pointer() = default;

         pointer& operator=( const pointer& ) = default;

         pointer& operator=( pointer&& p ) noexcept
         {
            m_tokens = std::move( p.m_tokens );
            return *this;
         }

         pointer& operator=( const std::string& v )
         {
            m_tokens.clear();
            parse( v );
            return *this;
         }

         pointer& operator=( const std::initializer_list< token >& l )
         {
            m_tokens = l;
            return *this;
         }

         explicit operator bool() const noexcept
         {
            return !m_tokens.empty();
         }

         bool empty() const noexcept
         {
            return m_tokens.empty();
         }

         std::size_t size() const noexcept
         {
            return m_tokens.size();
         }

         const token& at( const std::size_t i ) const noexcept
         {
            return m_tokens.at( i );
         }

         const token& operator[]( const std::size_t i ) const noexcept
         {
            return m_tokens[ i ];
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
            m_tokens.emplace_back( v );
         }

         void push_back( std::string&& v )
         {
            m_tokens.emplace_back( std::move( v ) );
         }

         void push_back( const std::size_t i )
         {
            m_tokens.emplace_back( i );
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

      inline bool operator!=( const pointer& lhs, const pointer& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      inline bool operator>( const pointer& lhs, const pointer& rhs ) noexcept
      {
         return rhs < lhs;
      }

      inline bool operator<=( const pointer& lhs, const pointer& rhs ) noexcept
      {
         return !( rhs < lhs );
      }

      inline bool operator>=( const pointer& lhs, const pointer& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      inline pointer& operator+=( pointer& lhs, const std::string& rhs )
      {
         lhs.push_back( rhs );
         return lhs;
      }

      inline pointer& operator+=( pointer& lhs, std::string&& rhs )
      {
         lhs.push_back( std::move( rhs ) );
         return lhs;
      }

      inline pointer& operator+=( pointer& lhs, const std::size_t rhs )
      {
         lhs.push_back( rhs );
         return lhs;
      }

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

      inline pointer operator+( const pointer& p, const std::size_t i )
      {
         pointer nrv( p );
         nrv += i;
         return nrv;
      }

      namespace internal
      {
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
            return std::runtime_error( internal::format( "unable to resolve JSON Pointer '", tokens_to_string( begin, end ), "' -- value type is neither 'object' nor 'array'" ) );
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
                     throw invalid_type( begin, std::next( it ) );  // NOLINT
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
