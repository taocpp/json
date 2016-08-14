// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_POINTER_HH
#define TAOCPP_JSON_INCLUDE_JSON_POINTER_HH

#include <string>
#include <stdexcept>
#include <utility>

namespace tao
{
   namespace json
   {
      // TODO: Move to a .cc file once taocpp/json drops 'header-only'.
      namespace internal
      {
         inline void validate_json_pointer( const std::string & v )
         {
            if ( v.empty() ) {
               return;
            }
            if ( v[ 0 ] != '/' ) {
               throw std::domain_error( "invalid json_pointer value, must be empty or begin with '/'" );
            }
            // TODO: Should we also check UTF-8 encoding and code-point range?
            for ( auto it = v.begin(); it != v.end(); ++it ) {
               if ( *it == '~' ) {
                  if ( ++it == v.end() || !( *it == '0' || *it == '1' ) ) {
                     throw std::domain_error( "invalid json_pointer escape sequence, '~' must be followed by '0' or '1'" );
                  }
               }
            }
         }

         inline std::string next_json_pointer_token( const char * & p, const char * e )
         {
            std::string result;
            // TODO: Find next '/' first, call result.reserve( x )?
            while( p != e ) {
               switch( *p ) {
                  case '/':
                     return result;
                  case '~':
                     switch( *++p ) {
                        case '0':
                           result += '~';
                           break;
                        case '1':
                           result += '/';
                           break;
                        default:
                           assert( !"code should be unreachable" );  // LCOV_EXCL_LINE
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
      class json_pointer
      {
      private:
         std::string m_value;

      public:
         json_pointer() noexcept
         { }

         json_pointer( const json_pointer& ) = default;

         json_pointer( json_pointer&& p ) noexcept
            : m_value( std::move( p.m_value ) )
         { }

         explicit json_pointer( std::string v )
            : m_value( std::move( v ) )
         {
            internal::validate_json_pointer( m_value );
         }

         json_pointer& operator=( const json_pointer& ) = default;

         json_pointer& operator=( json_pointer&& p ) noexcept
         {
            m_value = std::move( p.m_value );
            return *this;
         }

         json_pointer& operator=( std::string v )
         {
            internal::validate_json_pointer( v );
            m_value = std::move( v );
            return *this;
         }

         const std::string& value() const noexcept
         {
            return m_value;
         }
      };

      inline namespace literals
      {
         inline json_pointer operator "" _json_pointer( const char* p, const std::size_t s )
         {
            return json_pointer( { p, s } );
         }

      } // literals

   } // json

} // tao

#endif
