// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_HPP
#define TAO_JSON_HPP

// Value Class
#include "json/value.hpp"

// Value Reading
#include "json/from_stream.hpp"
#include "json/from_string.hpp"
#include "json/parse_file.hpp"

// Value Writing
#include "json/stream.hpp"  // operator<<
#include "json/to_stream.hpp"
#include "json/to_string.hpp"

// Value Support
#include "json/patch.hpp"
#include "json/schema.hpp"
#include "json/self_contained.hpp"

// Custom Types
#include "json/as.hpp"
#include "json/produce.hpp"

// JAXN Format
#include "json/jaxn.hpp"

// Binary Literals
#include "json/binary.hpp"

// Binary Formats
#include "json/cbor.hpp"
#include "json/msgpack.hpp"
#include "json/ubjson.hpp"

// Events Implementations
#include "json/events.hpp"

#endif
