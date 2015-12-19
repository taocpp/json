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

      inline bool operator== ( const value & l, const std::nullptr_t )
      {
         return l.is_null();
      }

      inline bool operator== ( const std::nullptr_t, const value & r )
      {
         return r.is_null();
      }

      inline bool operator!= ( const value & l, const std::nullptr_t )
      {
         return ! l.is_null();
      }

      inline bool operator!= ( const std::nullptr_t, const value & r )
      {
         return ! r.is_null();
      }

      inline bool operator== ( const value & l, const bool b )
      {
         return l.is_bool() && ( l.unsafe_bool() == b );
      }

      inline bool operator== ( const bool b, const value & r )
      {
         return r == b;
      }

      inline bool operator== ( const value & l, const signed char i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const signed char i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const unsigned char i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const unsigned char i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const signed short i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const signed short i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const unsigned short i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const unsigned short i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const signed int i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const signed int i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const unsigned int i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const unsigned int i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const signed long i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const signed long i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const signed long long i )
      {
         return l.is_int64() && ( l.unsafe_int64() == i );
      }

      inline bool operator== ( const signed long long i, const value & r )
      {
         return r == i;
      }

      inline bool operator== ( const value & l, const double d )
      {
         return l.is_double() && ( l.unsafe_double() == d );
      }

      inline bool operator== ( const double d, const value & r )
      {
         return r == d;
      }

      inline bool operator== ( const value & l, const char * s )
      {
         return l.is_string() && ( l.unsafe_string() == s );
      }

      inline bool operator== ( const char * s, const value & r )
      {
         return r == s;
      }

      inline bool operator== ( const value & l, const std::string & s )
      {
         return l.is_string() && ( l.unsafe_string() == s );
      }

      inline bool operator== ( const std::string & s, const value & r )
      {
         return r == s;
      }

      inline bool operator== ( const value & l, const std::vector< value > & v )
      {
         return l.is_array() && ( l.unsafe_array() == v );
      }

      inline bool operator== ( const std::vector< value > & v, const value & r )
      {
         return r == v;
      }

      inline bool operator== ( const value & l, const std::map< std::string, value > & v )
      {
         return l.is_object() && ( l.unsafe_object() == v );
      }

      inline bool operator== ( const std::map< std::string, value > & v, const value & r )
      {
         return r == v;
      }

   } // json

} // tao

#endif
