// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TYPE_HH
#define TAOCPP_JSON_INCLUDE_TYPE_HH

#include <cstdint>

#include "external/operators.hpp"

namespace tao
{
   namespace json
   {
      enum class type : std::uint8_t
      {
         DISCARDED,
         NULL_,
         BOOLEAN,
         SIGNED,
         UNSIGNED,
         DOUBLE,
         STRING,
         ARRAY,
         OBJECT,
         RAW_PTR
      };

      inline bool needs_destroy( const type t )
      {
         switch ( t ) {
            case type::STRING:
            case type::ARRAY:
            case type::OBJECT:
               return true;
            default:
               return false;
         }
      }

      inline const char * to_string( const type t )
      {
         switch ( t ) {
            case type::DISCARDED:
               return "discarded";
            case type::NULL_:
               return "null";
            case type::BOOLEAN:
               return "boolean";
            case type::SIGNED:
               return "signed";
            case type::UNSIGNED:
               return "unsigned";
            case type::DOUBLE:
               return "double";
            case type::STRING:
               return "string";
            case type::ARRAY:
               return "array";
            case type::OBJECT:
               return "object";
            case type::RAW_PTR:
               return "raw_ptr";
         }
         return "unknown";
      }

      struct null_t {} constexpr null {};
      struct empty_array_t {} constexpr empty_array {};
      struct empty_object_t {} constexpr empty_object {};

   } // json

} // tao

#endif
