// Copyright (c) 2015-2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

namespace tao
{
   namespace json
   {
      inline bool operator== ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ( l.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( l.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() == r );
      }

      inline bool operator< ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ( l.type() < TAOCPP_JSON_COMPARE_ENUM_TYPE ) || ( ( l.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( l.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() < r ) );
      }

      inline bool operator> ( const value & l, const TAOCPP_JSON_COMPARE_DATA_TYPE & r )
      {
         return ( l.type() > TAOCPP_JSON_COMPARE_ENUM_TYPE ) || ( ( l.type() == TAOCPP_JSON_COMPARE_ENUM_TYPE ) && ( l.get_by_enum< TAOCPP_JSON_COMPARE_ENUM_TYPE >() > r ) );
      }

   } // json

} // tao
