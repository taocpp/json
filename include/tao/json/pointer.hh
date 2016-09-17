// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_POINTER_HH
#define TAOCPP_JSON_INCLUDE_POINTER_HH

#include <string>
#include <stdexcept>
#include <utility>

#include "type.hh"
#include "external/operators.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline void validate_pointer( const std::string & v )
         {
            if ( v.empty() ) {
               return;
            }
            if ( v[ 0 ] != '/' ) {
               throw std::invalid_argument( "invalid JSON Pointer value, must be empty or begin with '/'" );
            }
            // TODO: Should we also check UTF-8 encoding and code-point range?
            for ( auto it = v.begin(); it != v.end(); ++it ) {
               if ( *it == '~' ) {
                  if ( ++it == v.end() || !( *it == '0' || *it == '1' ) ) {
                     throw std::invalid_argument( "invalid JSON Pointer escape sequence, '~' must be followed by '0' or '1'" );
                  }
               }
            }
         }

         inline std::string pointer_next_token( const char * & p, const char * e )
         {
            std::string result;
            while ( p != e ) {
               switch ( *p ) {
                  case '/':
                     return result;
                  case '~':
                     switch ( *++p ) {
                        case '0':
                           result += '~';
                           break;
                        case '1':
                           result += '/';
                           break;
                        default:
                           throw std::logic_error( "code should be unreachable" );  // LCOV_EXCL_LINE
                     }
                     ++p;
                     break;
                  default:
                     result += *p++;
               }
            }
            return result;
         }

      } // internal

      // RFC 6901
      class pointer
           : operators::totally_ordered< pointer >
      {
      private:
         std::string m_value;

      public:
         pointer() = default;

         pointer( const pointer & ) = default;

         pointer( pointer && p ) noexcept
              : m_value( std::move( p.m_value ) )
         { }

         explicit pointer( std::string v )
              : m_value( std::move( v ) )
         {
            internal::validate_pointer( m_value );
         }

         pointer & operator= ( const pointer & ) = default;

         pointer & operator= ( pointer && p ) noexcept
         {
            m_value = std::move( p.m_value );
            return *this;
         }

         pointer & operator= ( std::string v )
         {
            internal::validate_pointer( v );
            m_value = std::move( v );
            return *this;
         }

         explicit operator bool() const noexcept
         {
            return ! m_value.empty();
         }

         const std::string& value() const noexcept
         {
            return m_value;
         }

         std::pair< pointer, std::string > split() const
         {
            const auto p = m_value.rfind( '/' );
            if( p == std::string::npos ) {
               throw std::logic_error( "empty JSON Pointer can not be split" );
            }
            const char * b = m_value.data() + p;
            const char * e = m_value.data() + m_value.size();
            return { pointer( m_value.substr( 0, p ) ), internal::pointer_next_token( ++b, e ) };
         }
      };

      inline bool operator== ( const pointer & lhs, const pointer & rhs ) noexcept
      {
         return lhs.value() == rhs.value();
      }

      inline bool operator< ( const pointer & lhs, const pointer & rhs ) noexcept
      {
         return lhs.value() < rhs.value();
      }

      namespace internal
      {
         inline unsigned long long pointer_token_to_index( const std::string & t )
         {
            if ( ( t.find_first_not_of( "0123456789" ) != std::string::npos ) || ( t.size() > 1 && t[ 0 ] == '0' ) ) {
               throw std::invalid_argument( "unable to resolve JSON Pointer, invalid token for array access '" + t + '\'' );
            }
            return std::stoull( t );
         }

         template< typename T >
         T & pointer_at( T * v, const pointer & k )
         {
            const char * p = k.value().data();
            const char * e = p + k.value().size();
            while ( p != e ) {
               switch ( v->type() ) {
                  case type::ARRAY:
                     v = &v->at( pointer_token_to_index( pointer_next_token( ++p, e ) ) );
                     break;
                  case type::OBJECT:
                     v = &v->at( pointer_next_token( ++p, e ) );
                     break;
                  default:
                     throw std::runtime_error( "unable to resolve JSON Pointer, "
                                               "value at '" + std::string( k.value().c_str(), p ) + "' "
                                               "is neither 'object' nor 'array', "
                                               "but '" + to_string( v->type() ) + "'" );
               }
            }
            return * v;
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
