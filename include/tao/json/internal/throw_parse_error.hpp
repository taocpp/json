// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_THROW_PARSE_ERROR_HPP
#define TAO_JSON_INTERNAL_THROW_PARSE_ERROR_HPP

#include <cstdint>
#include <ostream>
#include <typeinfo>

#include "../forward.hpp"

#include "../external/pegtl/internal/demangle.hpp"
#include "../external/pegtl/parse_error.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline bool to_stream( std::ostream& o, const std::type_info& t )
         {
            o << ' ' << json_pegtl::internal::demangle( t.name() );
            return true;
         }

         template< typename T >
         bool to_stream( std::ostream& o, const T& t )
         {
            o << ' ' << t;
            return true;
         }

         template< std::size_t N >
         bool to_stream( std::ostream& o, const char( &t )[ N ] )
         {
            o << ' ';
            o.write( t, N - 1 );
            return false;
         }

         template< typename Input, typename... Ts >
         [[noreturn]] void throw_parse_error( Input& in, const Ts&... ts )
         {
            std::ostringstream oss;
            oss << "json:";
            (void)swallow{ to_stream( oss, ts )... };
            throw json_pegtl::parse_error( oss.str(), in );  // NOLINT
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
