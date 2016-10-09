// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json

#ifndef TAOCPP_JSON_INCLUDE_JSON_HH
#define TAOCPP_JSON_INCLUDE_JSON_HH

// DOM value
#include "json/value.hh"

// SAX producers
//#include "json/sax/from_stream.hh" // includes PEGTL header/grammar
#include "json/sax/from_string.hh" // includes PEGTL header/grammar
#include "json/sax/parse_file.hh" // includes PEGTL header/grammar
#include "json/sax/traverse_value.hh" // DOM to SAX

// SAX consumers
#include "json/sax/to_stream.hh"
#include "json/sax/to_pretty_stream.hh"
#include "json/sax/to_string.hh"
#include "json/sax/to_value.hh" // SAX to DOM

// SAX other
#include "json/sax/hash.hh"
#include "json/sax/discard.hh"
#include "json/sax/tee.hh"
#include "json/sax/validate_event_order.hh"
#include "json/sax/debug.hh"

// DOM producers
#include "json/from_string.hh" // includes PEGTL header/grammar
#include "json/parse_file.hh" // includes PEGTL header/grammar

// DOM writers
#include "json/to_stream.hh"
#include "json/to_string.hh"
#include "json/stream.hh" // operator<<

// DOM support
#include "json/self_contained.hh"
#include "json/patch.hh"
//#include "json/diff.hh"
#include "json/schema.hh"

#endif
