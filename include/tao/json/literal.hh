// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_LITERAL_HH
#define TAOCPP_JSON_INCLUDE_LITERAL_HH

#include "value.hh"
#include "parse.hh"

inline tao::json::value operator"" _json( const char * data, const std::size_t size )
{
   return tao::json::parse( data, size );
}

#endif
