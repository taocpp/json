// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_GET_BY_ENUM_HPP
#define TAO_JSON_INTERNAL_GET_BY_ENUM_HPP

#include <cstdint>

#include "../type.hpp"

#include "value_union.hpp"

namespace tao::json::internal
{
   template< type >
   struct get_by_enum;

   template<>
   struct get_by_enum< type::NULL_ >
   {
      template< typename T >
      [[nodiscard]] static null_t get( const value_union< T >& /*unused*/ ) noexcept
      {
         return null;
      }
   };

   template<>
   struct get_by_enum< type::BOOLEAN >
   {
      template< typename T >
      [[nodiscard]] static bool get( const value_union< T >& u ) noexcept
      {
         return u.b;
      }
   };

   template<>
   struct get_by_enum< type::SIGNED >
   {
      template< typename T >
      [[nodiscard]] static std::int64_t get( const value_union< T >& u ) noexcept
      {
         return u.i;
      }
   };

   template<>
   struct get_by_enum< type::UNSIGNED >
   {
      template< typename T >
      [[nodiscard]] static std::uint64_t get( const value_union< T >& u ) noexcept
      {
         return u.u;
      }
   };

   template<>
   struct get_by_enum< type::DOUBLE >
   {
      template< typename T >
      [[nodiscard]] static double get( const value_union< T >& u ) noexcept
      {
         return u.d;
      }
   };

   template<>
   struct get_by_enum< type::STRING >
   {
      template< typename T >
      [[nodiscard]] static std::string& get( value_union< T >& u ) noexcept
      {
         return u.s;
      }

      template< typename T >
      [[nodiscard]] static const std::string& get( const value_union< T >& u ) noexcept
      {
         return u.s;
      }
   };

   template<>
   struct get_by_enum< type::STRING_VIEW >
   {
      template< typename T >
      [[nodiscard]] static std::string_view get( const value_union< T >& u ) noexcept
      {
         return u.sv;
      }
   };

   template<>
   struct get_by_enum< type::BINARY >
   {
      template< typename T >
      [[nodiscard]] static std::vector< std::byte >& get( value_union< T >& u ) noexcept
      {
         return u.x;
      }

      template< typename T >
      [[nodiscard]] static const std::vector< std::byte >& get( const value_union< T >& u ) noexcept
      {
         return u.x;
      }
   };

   template<>
   struct get_by_enum< type::BINARY_VIEW >
   {
      template< typename T >
      [[nodiscard]] static tao::binary_view get( const value_union< T >& u ) noexcept
      {
         return u.xv;
      }
   };

   template<>
   struct get_by_enum< type::ARRAY >
   {
      template< typename T >
      [[nodiscard]] static std::vector< T >& get( value_union< T >& u ) noexcept
      {
         return u.a;
      }

      template< typename T >
      [[nodiscard]] static const std::vector< T >& get( const value_union< T >& u ) noexcept
      {
         return u.a;
      }
   };

   template<>
   struct get_by_enum< type::OBJECT >
   {
      template< typename T >
      [[nodiscard]] static std::map< std::string, T >& get( value_union< T >& u ) noexcept
      {
         return u.o;
      }

      template< typename T >
      [[nodiscard]] static const std::map< std::string, T >& get( const value_union< T >& u ) noexcept
      {
         return u.o;
      }
   };

   template<>
   struct get_by_enum< type::VALUE_PTR >
   {
      template< typename T >
      [[nodiscard]] static const T* get( const value_union< T >& u ) noexcept
      {
         return u.p;
      }
   };

   template<>
   struct get_by_enum< type::OPAQUE_PTR >
   {
      template< typename T >
      [[nodiscard]] static internal::opaque_ptr_t get( const value_union< T >& u ) noexcept
      {
         return u.q;
      }
   };

}  // namespace tao::json::internal

#endif
