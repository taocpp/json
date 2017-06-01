// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_HPP
#define TAOCPP_JSON_INCLUDE_JSON_HPP

// JSON Value
#include "json/value.hpp"

// Events producers
//#include "json/events/from_stream.hpp" // includes PEGTL header/grammar
#include "json/events/from_string.hpp"  // includes PEGTL header/grammar
#include "json/events/from_value.hpp"   // Value to Events
#include "json/events/parse_file.hpp"   // includes PEGTL header/grammar

// Events consumers
#include "json/events/to_pretty_stream.hpp"
#include "json/events/to_stream.hpp"
#include "json/events/to_string.hpp"
#include "json/events/to_value.hpp"  // Events to Value

// Events other
#include "json/events/debug.hpp"
#include "json/events/discard.hpp"
#include "json/events/hash.hpp"
#include "json/events/tee.hpp"
#include "json/events/validate_event_order.hpp"

// Value producers
#include "json/from_string.hpp"  // includes PEGTL header/grammar
#include "json/parse_file.hpp"   // includes PEGTL header/grammar

// Value writers
#include "json/stream.hpp"  // operator<<
#include "json/to_stream.hpp"
#include "json/to_string.hpp"

// Value support
#include "json/patch.hpp"
#include "json/self_contained.hpp"
//#include "json/diff.hpp"
#include "json/schema.hpp"

// Binary formats
#include "json/cbor/from_string.hpp"
#include "json/cbor/to_stream.hpp"
#include "json/cbor/to_string.hpp"
#include "json/msgpack/to_stream.hpp"
#include "json/msgpack/to_string.hpp"
#include "json/ubjson/to_stream.hpp"
#include "json/ubjson/to_string.hpp"

#endif
