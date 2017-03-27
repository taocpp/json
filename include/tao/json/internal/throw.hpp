// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HPP

#include <stdexcept>
#include <string>

#include "../type.hpp"

#define TAOCPP_JSON_THROW_TYPE_ERROR( TyPe )                                                     \
   do {                                                                                          \
      throw std::logic_error( std::string( "invalid json type '" ) + to_string( TyPe ) + '\'' ); \
   } while( false )

#define TAOCPP_JSON_CHECK_TYPE_ERROR( HaVe, NeeD ) \
   do {                                            \
      if( ( HaVe ) != ( NeeD ) ) {                 \
         TAOCPP_JSON_THROW_TYPE_ERROR( HaVe );     \
      }                                            \
   } while( false )

#endif
