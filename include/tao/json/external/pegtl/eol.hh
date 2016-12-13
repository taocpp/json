// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_EOL_HH
#define TAO_CPP_PEGTL_EOL_HH

#include <utility>
#include <cstdlib>

namespace tao_json_pegtl
{
   using eol_pair = std::pair< bool, std::size_t >;

} // namespace tao_json_pegtl

#include "lf_eol.hh"
#include "cr_eol.hh"
#include "crlf_eol.hh"
#include "lf_crlf_eol.hh"
#include "cr_crlf_eol.hh"

#endif
