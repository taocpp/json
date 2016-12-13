// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_ISTREAM_READER_HH
#define TAO_CPP_PEGTL_INTERNAL_ISTREAM_READER_HH

#include <istream>

#include "../input_error.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      struct istream_reader
      {
         explicit
         istream_reader( std::istream & s )
               : m_istream( s )
         { }

         std::size_t operator() ( char * buffer, const std::size_t length )
         {
            m_istream.read( buffer, length );

            if ( const auto r = m_istream.gcount() ) {
               return r;
            }
            if ( m_istream.eof() ) {
               return 0;
            }
            TAO_CPP_PEGTL_THROW_INPUT_ERROR( "error in istream.read()" );
         }

         std::istream & m_istream;
      };

   } // namespace internal

} // namespace tao_json_pegtl

#endif
