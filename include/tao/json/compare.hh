// Copyright (c) 2015-2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARE_HH
#define TAOCPP_JSON_INCLUDE_COMPARE_HH

#include "value.hh"

namespace tao
{
   namespace json
   {
      inline bool operator< ( const value & lhs, const value & rhs )
      {
         if ( lhs.type() == type::POINTER ) {
           return *lhs.unsafe_pointer() < rhs;
         }
         if ( rhs.type() == type::POINTER ) {
           return lhs < *rhs.unsafe_pointer();
         }
         if ( lhs.type() != rhs.type() ) {
            return lhs.type() < rhs.type();
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return false;
            case type::BOOL_:
               return lhs.unsafe_bool() < rhs.unsafe_bool();
            case type::INT64:
               return lhs.unsafe_int64() < rhs.unsafe_int64();
            case type::DOUBLE:
               return lhs.unsafe_double() < rhs.unsafe_double();
            case type::STRING:
               return lhs.unsafe_string() < rhs.unsafe_string();
            case type::ARRAY:
               return lhs.unsafe_array() < rhs.unsafe_array();
            case type::OBJECT:
               return lhs.unsafe_object() < rhs.unsafe_object();
            case type::POINTER:
               break;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      inline bool operator== ( const value & lhs, const value & rhs )
      {
         if ( lhs.type() == type::POINTER ) {
           return *lhs.unsafe_pointer() == rhs;
         }
         if ( rhs.type() == type::POINTER ) {
           return lhs == *rhs.unsafe_pointer();
         }
         if ( lhs.type() != rhs.type() ) {
            return false;
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return true;
            case type::BOOL_:
               return lhs.unsafe_bool() == rhs.unsafe_bool();
            case type::INT64:
               return lhs.unsafe_int64() == rhs.unsafe_int64();
            case type::DOUBLE:
               return lhs.unsafe_double() == rhs.unsafe_double();
            case type::STRING:
               return lhs.unsafe_string() == rhs.unsafe_string();
            case type::ARRAY:
               return lhs.unsafe_array() == rhs.unsafe_array();
            case type::OBJECT:
               return lhs.unsafe_object() == rhs.unsafe_object();
            case type::POINTER:
               break;
         }
         assert( false );  // LCOV_EXCL_LINE
      }

   } // json

} // tao

#endif
