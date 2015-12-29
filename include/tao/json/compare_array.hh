// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARE_ARRAY_HH
#define TAOCPP_JSON_INCLUDE_COMPARE_ARRAY_HH

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::ARRAY
#define TAOCPP_JSON_COMPARE_DATA_TYPE std::vector< value >

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#endif
