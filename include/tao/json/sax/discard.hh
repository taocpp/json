// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_DISCARD_HH
#define TAOCPP_JSON_INCLUDE_SAX_DISCARD_HH

#include <cstdint>
#include <string>

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX consumer does nothing
         class discard
         {
            void null() {}

            void boolean( const bool ) {}

            void number( const std::int64_t ) {}
            void number( const std::uint64_t ) {}
            void number( const double ) {}

            // the producer may call either one of the next two for a string,
            // only implement the first one if you don't need the second.
            void string( const std::string & ) {}
            void string( std::string && ) {}

            // array
            void begin_array() {}
            void element() {}
            void end_array() {}

            // object
            void begin_object() {}
            // the producer may call either one of the next two for a key,
            // only implement the first one if you don't need the second.
            void key( const std::string & ) {}
            void key( std::string && ) {}
            void member() {}
            void end_object() {}
         };

      } // sax

   } // json

} // tao

#endif
