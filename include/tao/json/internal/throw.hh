// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_THROW_HH

#include <sstream>
#include <stdexcept>

#include "../type.hh"

#define THROW_TYPE_ERROR( TyPe )                                        \
   do {                                                                 \
      std::ostringstream oss;                                           \
      oss << "invalid json type '" << json::to_string( TyPe ) << "' for function '" << __PRETTY_FUNCTION__ << "'"; \
      throw std::runtime_error( oss.str() );                            \
   } while ( false )

#define CHECK_TYPE_ERROR( HaVe, NeeD )          \
   do {                                         \
      if ( ( HaVe ) != ( NeeD ) ) {             \
         THROW_TYPE_ERROR( HaVe );              \
      }                                         \
   } while ( false )

#endif
