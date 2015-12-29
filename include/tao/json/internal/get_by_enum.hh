// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HH

#include "../type.hh"

#include "value_union.hh"

namespace tao
{
   namespace json
   {
      class value;

      namespace internal
      {
         template< type T > struct get_by_enum {};

         template<> struct get_by_enum< type::NULL_ >
         {
            using type = std::nullptr_t;

            static std::nullptr_t get( const value_union< value > & )
            {
               return nullptr;
            }
         };

         template<> struct get_by_enum< type::BOOL_ >
         {
            using type = bool;

            static bool get( const value_union< value > & u )
            {
               return u.b;
            }
         };

         template<> struct get_by_enum< type::INT64 >
         {
            using type = int64_t;

            static int64_t get( const value_union< value > & u )
            {
               return u.i;
            }
         };

         template<> struct get_by_enum< type::DOUBLE >
         {
            using type = double;

            static double get( const value_union< value > & u )
            {
               return u.d;
            }
         };

         template<> struct get_by_enum< type::STRING >
         {
            using type = const std::string &;

            static const std::string & get( const value_union< value > & u )
            {
               return u.s;
            }
         };

         template<> struct get_by_enum< type::ARRAY >
         {
            using type = const std::vector< value > &;

            static const std::vector< value > & get( const value_union< value > & u )
            {
               return u.a;
            }
         };

         template<> struct get_by_enum< type::OBJECT >
         {
            using type = const std::map< std::string, value >;

            static const std::map< std::string, value > & get( const value_union< value > & u )
            {
               return u.o;
            }
         };

      } // internal

   } // json

} // tao

#endif
