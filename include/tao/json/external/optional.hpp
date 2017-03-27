// The Art of C++
// Copyright (c) 2016-2017 Daniel Frey

#ifndef TAOCPP_INCLUDE_OPTIONAL_HPP
#define TAOCPP_INCLUDE_OPTIONAL_HPP

#ifndef TAOCPP_USE_STD_OPTIONAL
#if __cplusplus >= 201799L // TODO: Fix when C++17 is published with support for std::optional
#include <optional>
#define TAOCPP_USE_STD_OPTIONAL
#elif ( __cplusplus >= 201402L ) && (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#include <experimental/optional>
#else
#include "akrzemi1/optional.hpp"
#endif
#endif

namespace tao
{
#if TAOCPP_USE_STD_OPTIONAL
  using std::optional;
  using std::nullopt;
  using std::in_place;
  using std::make_optional;
#else
  using std::experimental::optional;
  using std::experimental::nullopt;
  using std::experimental::in_place;
  using std::experimental::make_optional;
#endif
}

#endif
