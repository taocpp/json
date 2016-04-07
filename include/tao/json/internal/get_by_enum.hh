// Copyright (c) 2015-2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HH

#include "../type.hh"

#include "value_union.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< type > struct get_by_enum {};

         template<> struct get_by_enum< type::NULL_ >
         {
            template< typename V >
            static std::nullptr_t get( const value_union< V > & )
            {
               return nullptr;
            }
         };

         template<> struct get_by_enum< type::BOOL_ >
         {
            template< typename V >
            static bool get( const value_union< V > & u )
            {
               return u.b;
            }
         };

         template<> struct get_by_enum< type::INT64 >
         {
            template< typename V >
            static int64_t get( const value_union< V > & u )
            {
               return u.i;
            }
         };

         template<> struct get_by_enum< type::DOUBLE >
         {
            template< typename V >
            static double get( const value_union< V > & u )
            {
               return u.d;
            }
         };

         template<> struct get_by_enum< type::STRING >
         {
            template< typename V >
            static const std::string & get( const value_union< V > & u )
            {
               return u.s;
            }
         };

         template<> struct get_by_enum< type::ARRAY >
         {
            template< typename V >
            static const std::vector< V > & get( const value_union< V > & u )
            {
               return u.a;
            }
         };

         template<> struct get_by_enum< type::OBJECT >
         {
            template< typename V >
            static const std::map< std::string, V > & get( const value_union< V > & u )
            {
               return u.o;
            }
         };

         template<> struct get_by_enum< type::POINTER >
         {
            template< typename V >
            static const V * get( const value_union< V > & u )
            {
               return u.p;
            }
         };

      } // internal

   } // json

} // tao

#endif
