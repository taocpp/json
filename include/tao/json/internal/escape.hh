// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ESCAPE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ESCAPE_HH

#include <ostream>
#include <string>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline void escape( std::ostream & o, const std::string & s )
         {
            static const char * h = "0123456789abcdef";

            o << '"';
            const char * p = s.data();
            const char * l = p;
            const char * const e = s.data() + s.size();
            while ( p != e ) {
               const unsigned char c = *p;
               if ( c == '\\' ) {
                  o.write( l, p - l );
                  l = ++p;
                  o << "\\\\";
               }
               else if ( c == '"' ) {
                  o.write( l, p - l );
                  l = ++p;
                  o << "\\\"";
               }
               else if ( c < 32 ) {
                  o.write( l, p - l );
                  l = ++p;
                  switch ( c ) {
                     case '\b':
                        o << "\\b";
                        break;
                     case '\f':
                        o << "\\f";
                        break;
                     case '\n':
                        o << "\\n";
                        break;
                     case '\r':
                        o << "\\r";
                        break;
                     case '\t':
                        o << "\\t";
                        break;
                     default:
                        o << "\\u00" << h[ ( c & 0xf0 ) >> 4 ] << h[ c & 0x0f ];
                  }
               }
               else if ( c == 127 ) {
                  o.write( l, p - l );
                  l = ++p;
                  o << "\\u007f";
               }
               else {
                  ++p;
               }
            }
            o.write( l, p - l );
            o << '"';
         }

      } // internal

   } // json

} // tao

#endif
