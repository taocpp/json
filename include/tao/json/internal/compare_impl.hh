// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

namespace tao
{
   namespace json
   {
      inline bool operator< ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ( l.type() < TAOCPP_JSON_COMPARE_ENUM_TYPE ) || ( ( l.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( l.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() < r ) );
      }

      inline bool operator< ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return ( r.type() > TAOCPP_JSON_COMPARE_ENUM_TYPE ) || ( ( r.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( r.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() > l ) );
      }

      inline bool operator> ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return r < l;
      }

      inline bool operator> ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return r < l;
      }

      inline bool operator== ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ( l.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( l.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() == r );
      }

      inline bool operator== ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return r == l;
      }

      inline bool operator!= ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ! ( l == r );
      }

      inline bool operator!= ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return ! ( r == l );
      }

      inline bool operator<= ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ! ( r > l );
      }

      inline bool operator<= ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return ! ( r > l );
      }

      inline bool operator>= ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ! ( r < l );
      }

      inline bool operator>= ( const TAOCPP_JSON_COMPARE_DATA_TYPE & l, const value & r )
      {
         return ! ( r < l );
      }

   } // json

} // tao
