// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARE_NULL_HH
#define TAOCPP_JSON_INCLUDE_COMPARE_NULL_HH

namespace tao
{
   namespace json
   {
      inline bool operator== ( const value & l, const std::nullptr_t )
      {
         return l.type() == type::NULL_;
      }

      inline bool operator< ( const value & l, const std::nullptr_t )
      {
         return l.type() < type::NULL_;
      }

      inline bool operator> ( const value & l, const std::nullptr_t )
      {
         return l.type() > type::NULL_;
      }

   } // json

} // tao

#endif
