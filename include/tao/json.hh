// Copyright (c) 2015-2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json

#ifndef TAOCPP_JSON_INCLUDE_JSON_HH
#define TAOCPP_JSON_INCLUDE_JSON_HH

#include "json/value.hh"
#include "json/compare.hh"
#include "json/traits.hh"

#include "json/stream.hh"
#include "json/to_string.hh"
#include "json/from_string.hh"
#include "json/parse_file.hh"

#include "json/compare_null.hh"
#include "json/compare_bool.hh"
#include "json/compare_int64.hh"
#include "json/compare_double.hh"
#include "json/compare_string.hh"
#include "json/compare_array.hh"
#include "json/compare_object.hh"

// Include this if required to enable
// C++11 user defined JSON literals that
// can be used like "[null]"_json .
//
// #include "json/literal.hh"

#endif
