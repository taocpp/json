// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_TO_STREAM_HH
#define TAOCPP_JSON_INCLUDE_SAX_TO_STREAM_HH

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
         // SAX consumer to build a JSON string representation
         class to_stream
         {
         private:
            std::ostream & os;
            bool first;

         public:
            explicit to_stream( std::ostream & os ) noexcept
                 : os( os )
            { }

            void null()
            {
               os.write( "null", 4 );
            }

            void boolean( const bool v )
            {
               if ( v ) {
                  os.write( "true", 4 );
               }
               else {
                  os.write( "false", 5 );
               }
            }

            void number( const std::int64_t v )
            {
               os << v;
            }

            void number( const std::uint64_t v )
            {
               os << v;
            }

            void number( const double v )
            {
               json_double_conversion::Dtostr( os, v );
            }

            void string( const std::string & v )
            {
               os.put( '"' );
               internal::escape( os, v );
               os.put( '"' );
            }

            // array
            void begin_array()
            {
               os.put( '[' );
               first = true;
            }

            void element()
            {
               os.put( ',' );
               first = false;
            }

            void end_array()
            {
               if ( ! first ) {
                  os.seekp( -1, std::ios_base::cur );
               }
               os.put( ']' );
               first = false;
            }

            // object
            void begin_object()
            {
               os.put( '{' );
               first = true;
            }

            void key( const std::string & v )
            {
               if ( ! first ) {
                  os.put( ',' );
               }
               os.put( '"' );
               internal::escape( os, v );
               os.write( "\":", 2 );
            }

            void member()
            {
               first = false;
            }

            void end_object()
            {
               os.put( '}' );
               first = false;
            }
         };

      } // sax

   } // json

} // tao

#endif
