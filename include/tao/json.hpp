// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_HPP
#define TAOCPP_JSON_INCLUDE_JSON_HPP

// DOM value
#include "json/value.hpp"

// SAX producers
//#include "json/sax/from_stream.hpp" // includes PEGTL header/grammar
#include "json/sax/from_string.hpp"  // includes PEGTL header/grammar
#include "json/sax/from_value.hpp"   // DOM to SAX
#include "json/sax/parse_file.hpp"   // includes PEGTL header/grammar

// SAX consumers
#include "json/sax/to_pretty_stream.hpp"
#include "json/sax/to_stream.hpp"
#include "json/sax/to_string.hpp"
#include "json/sax/to_value.hpp"  // SAX to DOM

// SAX other
#include "json/sax/debug.hpp"
#include "json/sax/discard.hpp"
#include "json/sax/hash.hpp"
#include "json/sax/tee.hpp"
#include "json/sax/validate_event_order.hpp"

// DOM producers
#include "json/from_string.hpp"  // includes PEGTL header/grammar
#include "json/parse_file.hpp"   // includes PEGTL header/grammar

// DOM writers
#include "json/stream.hpp"  // operator<<
#include "json/to_stream.hpp"
#include "json/to_string.hpp"

// DOM support
#include "json/patch.hpp"
#include "json/self_contained.hpp"
//#include "json/diff.hpp"
#include "json/schema.hpp"

#endif
