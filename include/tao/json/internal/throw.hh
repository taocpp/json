// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HH

#include <stdexcept>
#include <string>

#include "../type.hh"

#define TAOCPP_JSON_THROW_TYPE_ERROR( TyPe )                            \
   do {                                                                 \
      throw std::runtime_error( std::string( "invalid json type '" ) + to_string( TyPe ) + "' for function '" + __PRETTY_FUNCTION__ + '\'' ); \
   } while ( false )

#define TAOCPP_JSON_CHECK_TYPE_ERROR( HaVe, NeeD )             \
   do {                                                        \
      if ( ( HaVe ) != ( NeeD ) ) {                            \
         TAOCPP_JSON_THROW_TYPE_ERROR( HaVe );                 \
      }                                                        \
   } while ( false )

#endif
