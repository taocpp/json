// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONFIG_HPP
#define TAO_JSON_PEGTL_CONFIG_HPP

// Compatibility, remove with 3.0
#ifdef TAOCPP_JSON_PEGTL_NAMESPACE
#define TAO_JSON_PEGTL_NAMESPACE TAOCPP_JSON_PEGTL_NAMESPACE
#endif

#ifndef TAO_JSON_PEGTL_NAMESPACE
#define TAO_JSON_PEGTL_NAMESPACE json_pegtl
#endif

// Enable some improvements to the readability of
// demangled type names under some circumstances.
// #define TAO_JSON_PEGTL_PRETTY_DEMANGLE

#endif
