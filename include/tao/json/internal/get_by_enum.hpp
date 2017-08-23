// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_GET_BY_ENUM_HPP

#include <cstdint>

#include "../type.hpp"

#include "../external/byte.hpp"

#include "data_union.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< type >
         struct get_by_enum;

         template<>
         struct get_by_enum< type::NULL_ >
         {
            static null_t get( const data_union& ) noexcept
            {
               return null;
            }
         };

         template<>
         struct get_by_enum< type::BOOLEAN >
         {
            static bool get( const data_union& u ) noexcept
            {
               return u.b;
            }
         };

         template<>
         struct get_by_enum< type::SIGNED >
         {
            static std::int64_t get( const data_union& u ) noexcept
            {
               return u.i;
            }
         };

         template<>
         struct get_by_enum< type::UNSIGNED >
         {
            static std::uint64_t get( const data_union& u ) noexcept
            {
               return u.u;
            }
         };

         template<>
         struct get_by_enum< type::DOUBLE >
         {
            static double get( const data_union& u ) noexcept
            {
               return u.d;
            }
         };

         template<>
         struct get_by_enum< type::STRING >
         {
            static std::string& get( data_union& u ) noexcept
            {
               return u.s;
            }

            static const std::string& get( const data_union& u ) noexcept
            {
               return u.s;
            }
         };

         template<>
         struct get_by_enum< type::STRING_VIEW >
         {
            static tao::string_view get( const data_union& u ) noexcept
            {
               return u.sv;
            }
         };

         template<>
         struct get_by_enum< type::BINARY >
         {
            static std::vector< tao::byte >& get( data_union& u ) noexcept
            {
               return u.x;
            }

            static const std::vector< tao::byte >& get( const data_union& u ) noexcept
            {
               return u.x;
            }
         };

         template<>
         struct get_by_enum< type::BINARY_VIEW >
         {
            static tao::byte_view get( const data_union& u ) noexcept
            {
               return u.xv;
            }
         };

         template<>
         struct get_by_enum< type::ARRAY >
         {
            static std::vector< data >& get( data_union& u ) noexcept
            {
               return u.a;
            }

            static const std::vector< data >& get( const data_union& u ) noexcept
            {
               return u.a;
            }
         };

         template<>
         struct get_by_enum< type::OBJECT >
         {
            static std::map< std::string, data >& get( data_union& u ) noexcept
            {
               return u.o;
            }

            static const std::map< std::string, data >& get( const data_union& u ) noexcept
            {
               return u.o;
            }
         };

         template<>
         struct get_by_enum< type::RAW_PTR >
         {
            static const data* get( const data_union& u ) noexcept
            {
               return u.p;
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
