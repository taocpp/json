// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_KEYS_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_KEYS_HPP

#include <stdexcept>
#include <string>
#include <utility>

#include "../external/pegtl.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer, typename Rule >
         struct validate_keys
            : public Consumer
         {
            using Consumer::Consumer;

            void validate_key( const std::string& v )
            {
               json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY > in( v, __PRETTY_FUNCTION__ );
               if( !json_pegtl::parse< Rule >( in ) ) {
                  throw std::runtime_error( "invalid key: " + v );
               }
            }

            void key( const std::string& v )
            {
               validate_key( v );
               Consumer::key( v );
            }

            void key( std::string&& v )
            {
               validate_key( v );
               Consumer::key( std::move( v ) );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
