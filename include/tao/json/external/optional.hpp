// The Art of C++
// Copyright (c) 2016-2018 Daniel Frey

#ifndef TAO_JSON_EXTERNAL_OPTIONAL_HPP
#define TAO_JSON_EXTERNAL_OPTIONAL_HPP

#ifndef TAO_JSON_USE_STD_OPTIONAL
#if __cplusplus >= 201703L
#define TAO_JSON_USE_STD_OPTIONAL
#endif
#endif

#ifndef TAO_JSON_USE_STD_OPTIONAL
#ifndef TAO_JSON_USE_TS_OPTIONAL
#if( __cplusplus >= 201402L ) && ( ( __GNUC__ > 4 ) || ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 9 ) )
#define TAO_JSON_USE_TS_OPTIONAL
#endif
#endif
#endif

#if defined( TAO_JSON_USE_STD_OPTIONAL )
#include <optional>
#elif defined( TAO_JSON_USE_TS_OPTIONAL )
#include <experimental/optional>
#else
#include "akrzemi1/optional.hpp"
#endif

namespace tao
{

#if defined( TAO_JSON_USE_STD_OPTIONAL )
   using std::in_place;
   using std::make_optional;
   using std::nullopt;
   using std::optional;
#else
   using std::experimental::in_place;
   using std::experimental::make_optional;
   using std::experimental::nullopt;
   using std::experimental::optional;
#endif

}  // namespace tao

#endif
