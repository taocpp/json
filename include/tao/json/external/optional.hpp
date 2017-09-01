// The Art of C++
// Copyright (c) 2016-2017 Daniel Frey

#ifndef TAOCPP_INCLUDE_OPTIONAL_HPP
#define TAOCPP_INCLUDE_OPTIONAL_HPP

#ifndef TAOCPP_USE_STD_OPTIONAL
#if __cplusplus >= 201703L
#define TAOCPP_USE_STD_OPTIONAL
#endif
#endif

#ifndef TAOCPP_USE_STD_OPTIONAL
#ifndef TAOCPP_USE_TS_OPTIONAL
#if( __cplusplus >= 201402L ) && ( ( __GNUC__ > 4 ) || ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 9 ) )
#define TAOCPP_USE_TS_OPTIONAL
#endif
#endif
#endif

#if defined( TAOCPP_USE_STD_OPTIONAL )
#include <optional>
#elif defined( TAOCPP_USE_TS_OPTIONAL )
#include <experimental/optional>
#else
#include "akrzemi1/optional.hpp"
#endif

namespace tao
{

#if defined( TAOCPP_USE_STD_OPTIONAL )
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

}  // namespace tao

#endif
