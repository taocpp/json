// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TYPE_HH
#define TAOCPP_JSON_INCLUDE_TYPE_HH

namespace tao
{
   namespace json
   {
      enum class type : uint8_t
      {
         NULL_,
         BOOL_,
         SIGNED,
         UNSIGNED,
         DOUBLE,
         STRING,
         ARRAY,
         OBJECT,
         POINTER
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
            case type::NULL_:
               return "null";
            case type::BOOL_:
               return "bool";
            case type::SIGNED:
               return "signed_integer";
            case type::UNSIGNED:
               return "unsigned_integer";
            case type::DOUBLE:
               return "double";
            case type::STRING:
               return "string";
            case type::ARRAY:
               return "array";
            case type::OBJECT:
               return "object";
            case type::POINTER:
               return "pointer";
         }
         return "unknown";
      }

      struct null_t {};
      struct empty_array_t {};
      struct empty_object_t {};

      static constexpr null_t null = null_t();
      static constexpr empty_array_t empty_array = empty_array_t();
      static constexpr empty_object_t empty_object = empty_object_t();

   } // json

} // tao

#endif
