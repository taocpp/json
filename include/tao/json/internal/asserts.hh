// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_ASSERTS_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_ASSERTS_HH

// In some places ILP64 is assumed.

static_assert( sizeof( char ) == 1, "internal consistency" );
static_assert( sizeof( signed char ) == 1, "internal consistency" );
static_assert( sizeof( unsigned char ) == 1, "internal consistency" );
static_assert( sizeof( signed short ) == 2, "internal consistency" );
static_assert( sizeof( unsigned short ) == 2, "internal consistency" );
static_assert( sizeof( signed int ) == 4, "internal consistency" );
static_assert( sizeof( unsigned int ) == 4, "internal consistency" );
static_assert( sizeof( signed long ) == 8, "internal consistency" );
static_assert( sizeof( unsigned long ) == 8, "internal consistency" );
static_assert( sizeof( signed long long ) == 8, "internal consistency" );
static_assert( sizeof( unsigned long long ) == 8, "internal consistency" );

#endif
