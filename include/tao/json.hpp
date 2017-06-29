// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_HPP
#define TAOCPP_JSON_INCLUDE_JSON_HPP

// JSON Value
#include "json/value.hpp"

// Events producers
#include "json/events/from_stream.hpp"
#include "json/events/from_string.hpp"
#include "json/events/from_value.hpp"
#include "json/events/parse_file.hpp"

// Events consumers
#include "json/events/to_pretty_stream.hpp"
#include "json/events/to_stream.hpp"
#include "json/events/to_string.hpp"
#include "json/events/to_value.hpp"

// Events transformer
#include "json/events/binary_to_base64.hpp"
#include "json/events/binary_to_base64url.hpp"
#include "json/events/binary_to_exception.hpp"
#include "json/events/binary_to_hex.hpp"
#include "json/events/non_finite_to_exception.hpp"
#include "json/events/non_finite_to_null.hpp"
#include "json/events/non_finite_to_string.hpp"
#include "json/events/prefer_signed.hpp"
#include "json/events/prefer_unsigned.hpp"

// Events other
#include "json/events/debug.hpp"
#include "json/events/discard.hpp"
#include "json/events/hash.hpp"
#include "json/events/tee.hpp"
#include "json/events/validate_event_order.hpp"
#include "json/events/validate_keys.hpp"

// Value producers
#include "json/from_stream.hpp"
#include "json/from_string.hpp"
#include "json/parse_file.hpp"

// Value writers
#include "json/stream.hpp"  // operator<<
#include "json/to_stream.hpp"
#include "json/to_string.hpp"

// Value support
#include "json/patch.hpp"
#include "json/self_contained.hpp"
//#include "json/diff.hpp"
#include "json/schema.hpp"

// Relaxed JSON
#include "json/jaxn.hpp"

// Binary literals
#include "json/binary.hpp"

// Binary formats
#include "json/cbor.hpp"
#include "json/msgpack.hpp"
#include "json/ubjson.hpp"

#endif
