// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_HPP
#define TAO_JSON_PEGTL_HPP

#include "tao/pegtl/config.hpp"
#include "tao/pegtl/parse.hpp"
#include "tao/pegtl/version.hpp"

#include "tao/pegtl/ascii.hpp"
#include "tao/pegtl/rules.hpp"
#include "tao/pegtl/utf8.hpp"

#include "tao/pegtl/argv_input.hpp"
#include "tao/pegtl/buffer_input.hpp"
#include "tao/pegtl/cstream_input.hpp"
#include "tao/pegtl/istream_input.hpp"
#include "tao/pegtl/memory_input.hpp"
#include "tao/pegtl/read_input.hpp"
#include "tao/pegtl/string_input.hpp"

// This has to be included *after* the above inputs,
// otherwise the amalgamated header will not work!
#include "tao/pegtl/file_input.hpp"

#include "tao/pegtl/change_action.hpp"
#include "tao/pegtl/change_action_and_state.hpp"
#include "tao/pegtl/change_action_and_states.hpp"
#include "tao/pegtl/change_control.hpp"
#include "tao/pegtl/change_state.hpp"
#include "tao/pegtl/change_states.hpp"

#include "tao/pegtl/disable_action.hpp"
#include "tao/pegtl/enable_action.hpp"

#include "tao/pegtl/discard_input.hpp"
#include "tao/pegtl/discard_input_on_failure.hpp"
#include "tao/pegtl/discard_input_on_success.hpp"

#include "tao/pegtl/must_if.hpp"
#include "tao/pegtl/visit.hpp"

#endif
