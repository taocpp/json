// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARE_HH
#define TAOCPP_JSON_INCLUDE_COMPARE_HH

#include "internal/compare.hh"

namespace tao
{
   namespace json
   {
      inline bool operator< ( const value & l, const value & r )
      {
         return ( l.type() < r.type() ) || ( ( l.type() == r.type() ) && internal::less_value( l, r ) );
      }

      inline bool operator> ( const value & l, const value & r )
      {
         return r < l;
      }

      inline bool operator<= ( const value & l, const value & r )
      {
         return ! ( l > r );
      }

      inline bool operator>= ( const value & l, const value & r )
      {
         return ! ( l < r );
      }

      inline bool operator== ( const value & l, const value & r )
      {
         return ( l.type() == r.type() ) && internal::equal_value( l, r );
      }

      inline bool operator!= ( const value & l, const value & r )
      {
         return ! ( l == r );
      }

   } // json

} // tao

#endif
