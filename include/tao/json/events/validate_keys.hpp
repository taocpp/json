// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_VALIDATE_KEYS_HPP
#define TAO_JSON_EVENTS_VALIDATE_KEYS_HPP

#include <stdexcept>
#include <string>
#include <utility>

#include "../external/pegtl.hpp"
#include "../external/string_view.hpp"

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

            void validate_key( const tao::string_view v )
            {
               json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY > in( v.data(), v.size(), "validate_key" );
               if( !json_pegtl::parse< Rule >( in ) ) {
                  throw std::runtime_error( "invalid key: " + std::string( v.data(), v.size() ) );  // NOLINT
               }
            }

            void key( const tao::string_view v )
            {
               validate_key( v );
               Consumer::key( v );
            }

            void key( const char* v )
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
