// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_VALUE_UNION_HPP
#define TAO_JSON_INTERNAL_VALUE_UNION_HPP

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "../binary_view.hpp"
#include "../forward.hpp"
#include "../type.hpp"

namespace tao::json::internal
{
   struct opaque_ptr_t
   {
      const void* data;
      producer_t producer;
   };

   template< typename T >
   using value_union = std::variant< uninitialized_t,
                                     null_t,
                                     bool,
                                     std::int64_t,
                                     std::uint64_t,
                                     double,
                                     std::string,
                                     std::string_view,
                                     std::vector< std::byte >,
                                     tao::binary_view,
                                     std::vector< T >,
                                     std::map< std::string, T, std::less<> >,
                                     const T*,
                                     opaque_ptr_t >;

}  // namespace tao::json::internal

#endif
