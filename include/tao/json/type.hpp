// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TYPE_HPP
#define TAOCPP_JSON_INCLUDE_TYPE_HPP

#include <cstdint>

#include "external/operators.hpp"

namespace tao
{
   namespace json
   {
      enum class type : std::uint8_t
      {
         UNINITIALIZED,
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

      inline bool needs_discard( const type t )
      {
         switch( t ) {
            case type::STRING:
            case type::ARRAY:
            case type::OBJECT:
               return true;
            default:
               return false;
         }
      }

      inline const char* to_string( const type t )
      {
         switch( t ) {
            case type::UNINITIALIZED:
               return "uninitialized";
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

      struct null_t
      {
         constexpr explicit null_t( int )
         {
         }
      };

      struct empty_array_t
      {
         constexpr explicit empty_array_t( int )
         {
         }
      };

      struct empty_object_t
      {
         constexpr explicit empty_object_t( int )
         {
         }
      };

      constexpr null_t null{ 0 };
      constexpr empty_array_t empty_array{ 0 };
      constexpr empty_object_t empty_object{ 0 };

   }  // json

}  // tao

#endif
