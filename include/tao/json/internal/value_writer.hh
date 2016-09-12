// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_WRITER_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_WRITER_HH

#include "escape.hh"

#include "../external/double.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         class value_writer
         {
         private:
            std::ostream & os;

         public:
            explicit value_writer( std::ostream & os ) noexcept : os( os ) {}

            void null() { os.write( "null", 4 ); }
            void true_() { os.write( "true", 4 ); }
            void false_() { os.write( "false", 5 ); }
            void number( const std::int64_t v ) { os << v; }
            void number( const std::uint64_t v ) { os << v; }
            void number( const double v ) { json_double_conversion::Dtostr( os, v ); }
            void string( const std::string & v ) { internal::escape( os, v ); }
            void begin_array() { os.put( '[' ); }
            void commit_element() noexcept {}
            void element_separator() { os.put( ',' ); }
            void end_array() { os.put( ']' ); }
            void begin_object() { os.put( '{' ); }
            void commit_key( const std::string & v ) { internal::escape( os, v ); }
            void name_separator() { os.put( ':' ); }
            void commit_member() noexcept {}
            void value_separator() { os.put( ',' ); }
            void end_object() { os.put( '}' ); }
         };

      } // internal

   } // json

} // tao

#endif
