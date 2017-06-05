// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_DEBUG_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_DEBUG_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include "../external/double.hpp"

#include "../byte.hpp"
#include "../internal/escape.hpp"
#include "../internal/hexdump.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer that writes the events to a stream for debug purposes.

         class debug
         {
         private:
            std::ostream& os;

         public:
            explicit debug( std::ostream& os ) noexcept
               : os( os )
            {
            }

            void null()
            {
               os << "null\n";
            }

            void boolean( const bool v )
            {
               if( v ) {
                  os << "boolean: true\n";
               }
               else {
                  os << "boolean: false\n";
               }
            }

            void number( const std::int64_t v )
            {
               os << "std::int64_t: " << v << '\n';
            }

            void number( const std::uint64_t v )
            {
               os << "std::uint64_t: " << v << '\n';
            }

            void number( const double v )
            {
               os << "double: ";
               if( !std::isfinite( v ) ) {
                  os << v;
               }
               else {
                  json_double_conversion::Dtostr( os, v );
               }
               os << '\n';
            }

            void string( const std::string& v )
            {
               os << "string: \"";
               internal::escape( os, v );
               os << "\"\n";
            }

            void binary( const std::vector< byte >& v )
            {
               os << "binary: " << internal::hexdump( v ) << "\n";
            }

            void begin_array()
            {
               os << "begin array\n";
            }

            void begin_array( const std::size_t size )
            {
               os << "begin array " << size << '\n';
            }

            void element()
            {
               os << "element\n";
            }

            void end_array()
            {
               os << "end array\n";
            }

            void end_array( const std::size_t size )
            {
               os << "end array " << size << '\n';
            }

            void begin_object()
            {
               os << "begin object\n";
            }

            void begin_object( const std::size_t size )
            {
               os << "begin object " << size << '\n';
            }

            void key( const std::string& v )
            {
               os << "key: \"";
               internal::escape( os, v );
               os << "\"\n";
            }

            void member()
            {
               os << "member\n";
            }

            void end_object()
            {
               os << "end object\n";
            }

            void end_object( const std::size_t size )
            {
               os << "end object " << size << '\n';
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
