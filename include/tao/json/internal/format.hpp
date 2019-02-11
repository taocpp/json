// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_FORMAT_HPP
#define TAO_JSON_INTERNAL_FORMAT_HPP

#include <ostream>
#include <sstream>
#include <typeinfo>

#include "escape.hpp"

#include "../forward.hpp"
#include "../message_extension.hpp"
#include "../type.hpp"

#include "../external/pegtl/internal/demangle.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline void to_stream( std::ostream& os, const bool v )
         {
            os << ( v ? "true" : "false" );
         }

         inline void to_stream( std::ostream& os, const type t )
         {
            os << to_string( t );
         }

         inline void to_stream( std::ostream& os, const std::type_info& t )
         {
            os << json_pegtl::internal::demangle( t.name() );
         }

         template< typename T >
         void to_stream( std::ostream& os, const T& t )
         {
            os << t;
         }

         template< std::size_t N >
         void to_stream( std::ostream& os, const char ( &t )[ N ] )
         {
            os.write( t, N - 1 );
         }

         template< typename... Ts >
         void format_to( std::ostream& oss, const Ts&... ts )
         {
            ( internal::to_stream( oss, ts ), ... );
         }

         template< typename... Ts >
         std::string format( const Ts&... ts )
         {
            std::ostringstream oss;
            format_to( oss, ts... );
            return oss.str();
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
