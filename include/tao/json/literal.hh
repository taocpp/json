// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_LITERAL_HH
#define TAOCPP_JSON_INCLUDE_LITERAL_HH

#include "value.hh"
#include "from_string.hh"

inline tao::json::value operator"" _json( const char * data, const std::size_t size )
{
   return tao::json::from_string( data, size, "literal" );
}

#endif
