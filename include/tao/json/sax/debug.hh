// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_DEBUG_HH
#define TAOCPP_JSON_INCLUDE_SAX_DEBUG_HH

#include <ostream>
#include <cstdint>

#include "../external/double.hh"

#include "../internal/escape.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX consumer that writes the events to a stream for debug purposed
         class debug
         {
         private:
            std::ostream & os;

         public:
            explicit debug( std::ostream & os ) noexcept
                 : os( os )
            { }

            void null()
            {
               os << "null\n";
            }

            void boolean( const bool v )
            {
               if ( v ) {
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
               json_double_conversion::Dtostr( os, v );
               os << '\n';
            }

            void string( const std::string & v )
            {
               os << "string: ";
               internal::escape( os, v );
               os << '\n';
            }

            // array
            void begin_array()
            {
               os << "begin array\n";
            }

            void element()
            {
               os << "element\n";
            }

            void end_array()
            {
               os << "end array\n";
            }

            // object
            void begin_object()
            {
               os << "begin object\n";
            }

            void key( const std::string & v )
            {
               os << "key: ";
               internal::escape( os, v );
               os << '\n';
            }

            void member()
            {
               os << "member\n";
            }

            void end_object()
            {
               os << "end object\n";
            }
         };

      } // sax

   } // json

} // tao

#endif
