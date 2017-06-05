// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_TO_PRETTY_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_TO_PRETTY_STREAM_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../external/double.hpp"

#include "../byte.hpp"
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
            to_pretty_stream( std::ostream& os, const std::size_t indent )
               : os( os ),
                 indent( indent ),
                 first( true )
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
                  throw std::runtime_error( "non-finite double value invalid for JSON string representation" );
               }
               json_double_conversion::Dtostr( os, v );
            }

            void string( const std::string& v )
            {
               next();
               os.put( '"' );
               internal::escape( os, v );
               os.put( '"' );
            }

            void binary( const std::vector< byte >& )
            {
               // if this throws, consider using binary_to_* wrappers
               throw std::runtime_error( "binary data invalid for JSON string representation" );
            }

            void begin_array( const std::size_t = 0 )
            {
               next();
               os.put( '[' );
               current.resize( current.size() + indent, ' ' );
               first = true;
            }

            void element()
            {
               first = false;
            }

            void end_array( const std::size_t = 0 )
            {
               current.resize( current.size() - indent );
               if( !first ) {
                  os << current;
               }
               os.put( ']' );
            }

            void begin_object( const std::size_t = 0 )
            {
               next();
               os.put( '{' );
               current.resize( current.size() + indent, ' ' );
               first = true;
            }

            void key( const std::string& v )
            {
               string( v );
               os.write( ": ", 2 );
               first = true;
               after_key = true;
            }

            void member()
            {
               first = false;
            }

            void end_object( const std::size_t = 0 )
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
