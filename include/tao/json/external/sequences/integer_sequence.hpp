// The Art of C++ / Sequences
// Copyright (c) 2015 Daniel Frey

#ifndef TAOCPP_JSON_EMBEDDED_SEQUENCES_INCLUDE_INTEGER_SEQUENCE_HPP
#define TAOCPP_JSON_EMBEDDED_SEQUENCES_INCLUDE_INTEGER_SEQUENCE_HPP

#include <cstddef>
#include <utility>

#include "config.hpp"

namespace tao
{
  namespace json_seq
  {

#ifdef TAOCPP_JSON_EMBEDDED_USE_STD_INTEGER_SEQUENCE

    using std::integer_sequence;
    using std::index_sequence;

#else

    template< typename T, T... Ns >
    struct integer_sequence
    {
      using value_type = T;

      static constexpr std::size_t size() noexcept
      {
        return sizeof...( Ns );
      }
    };

    template< std::size_t... Ns >
    using index_sequence = integer_sequence< std::size_t, Ns... >;

#endif

  }
}

#endif // TAOCPP_JSON_EMBEDDED_SEQUENCES_INCLUDE_INTEGER_SEQUENCE_HPP
