// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INPUT_ERROR_HH
#define TAO_CPP_PEGTL_INPUT_ERROR_HH

#include <sstream>
#include <stdexcept>
#include <cerrno>

namespace tao_json_pegtl
{
   struct input_error
         : std::runtime_error
   {
      input_error( const std::string & message, const int in_errorno )
            : std::runtime_error( message ),
              errorno( in_errorno )
      { }

      int errorno;
   };

#define TAO_CPP_PEGTL_THROW_INPUT_ERROR( MESSAGE )                              \
   do {                                                                 \
      const int errorno = errno;                                        \
      std::ostringstream oss;                                           \
      oss << "pegtl: " << MESSAGE << " errno " << errorno;              \
      throw tao_json_pegtl::input_error( oss.str(), errorno );                   \
   } while ( false )

} // namespace tao_json_pegtl

#endif
