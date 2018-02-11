// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_TO_PRETTY_STREAM_HPP
#define TAO_JSON_EVENTS_TO_PRETTY_STREAM_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../byte_view.hpp"
#include "../external/byte.hpp"
#include "../external/double.hpp"
#include "../external/string_view.hpp"

#include "../internal/escape.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer to build a JSON pretty string representation.

         class to_pretty_stream
         {
         protected:
            std::ostream& os;
            const std::size_t indent;

            std::string current = "\n";
            bool first = true;
            bool after_key = true;

            void next()
            {
               if( !first ) {
                  os.put( ',' );
               }
               if( after_key ) {
                  after_key = false;
               }
               else {
                  os << current;
               }
            }

         public:
            to_pretty_stream( std::ostream& in_os, const std::size_t in_indent )
               : os( in_os ),
                 indent( in_indent )
            {
            }

            template< typename S >
            to_pretty_stream( std::ostream& in_os, const std::size_t in_indent, S&& in_eol )
               : os( in_os ),
                 indent( in_indent ),
                 current( std::forward< S >( in_eol ) )
            {
            }

            void null()
            {
               next();
               os.write( "null", 4 );
            }

            void boolean( const bool v )
            {
               next();
               if( v ) {
                  os.write( "true", 4 );
               }
               else {
                  os.write( "false", 5 );
               }
            }

            void number( const std::int64_t v )
            {
               next();
               os << v;
            }

            void number( const std::uint64_t v )
            {
               next();
               os << v;
            }

            void number( const double v )
            {
               next();
               if( !std::isfinite( v ) ) {
                  // if this throws, consider using non_finite_to_* transformers
                  throw std::runtime_error( "non-finite double value invalid for JSON string representation" );  // NOLINT
               }
               json_double_conversion::Dtostr( os, v );
            }

            void string( const tao::string_view v )
            {
               next();
               os.put( '"' );
               internal::escape( os, v );
               os.put( '"' );
            }

            void binary( const tao::byte_view /*unused*/ )
            {
               // if this throws, consider using binary_to_* transformers
               throw std::runtime_error( "binary data invalid for JSON string representation" );  // NOLINT
            }

            void begin_array( const std::size_t /*unused*/ = 0 )
            {
               next();
               os.put( '[' );
               current.resize( current.size() + indent, ' ' );
               first = true;
            }

            void element() noexcept
            {
               first = false;
            }

            void end_array( const std::size_t /*unused*/ = 0 )
            {
               current.resize( current.size() - indent );
               if( !first ) {
                  os << current;
               }
               os.put( ']' );
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               next();
               os.put( '{' );
               current.resize( current.size() + indent, ' ' );
               first = true;
            }

            void key( const tao::string_view v )
            {
               string( v );
               os.write( ": ", 2 );
               first = true;
               after_key = true;
            }

            void member() noexcept
            {
               first = false;
            }

            void end_object( const std::size_t /*unused*/ = 0 )
            {
               current.resize( current.size() - indent );
               if( !first ) {
                  os << current;
               }
               os.put( '}' );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
