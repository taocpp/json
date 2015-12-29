// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARE_INT64_HH
#define TAOCPP_JSON_INCLUDE_COMPARE_INT64_HH

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE signed char

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE unsigned char

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE signed short

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE unsigned short

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE signed int

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE unsigned int

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE signed long

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#define TAOCPP_JSON_COMPARE_ENUM_TYPE type::INT64
#define TAOCPP_JSON_COMPARE_DATA_TYPE signed long long

#include "internal/compare_impl.hh"

#undef TAOCPP_JSON_COMPARE_ENUM_TYPE
#undef TAOCPP_JSON_COMPARE_DATA_TYPE

#endif
