// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/value.hpp>
#include <tao/json/parse_file.hpp>
#include <tao/json/from_string.hpp>

namespace tao
{
   namespace json
   {
      namespace position
      {
         struct position_base
         {
            tao::optional< json_pegtl::position > position;
         };

         // TODO: We can still template over the traits.
         using value = basic_value< traits, position_base >;

         namespace internal
         {
            template< typename Rule >
            struct action
               : public json::internal::action< Rule >
            {
            };

            template<>
            struct action< json::internal::rules::value >
            {
               template< typename Input, typename Consumer >
               static void apply( const Input& in, Consumer& consumer )
               {
                  consumer.value.base().position = in.position();
               }
            };

         } // internal

         value parse_file( const std::string& filename )
         {
            events::to_basic_value< traits, position_base > consumer;
            json_pegtl::file_input< json_pegtl::tracking_mode::IMMEDIATE > in( filename );
            json_pegtl::parse< json::internal::grammar, internal::action, json::internal::control >( in, consumer );
            return std::move( consumer.value );
         }

      } // namespace position

      void unit_test()
      {
         auto v = position::parse_file( "tests/draft4/additionalItems.json" );
         //         std::cerr << *v[ 1 ][ "description" ].base().position << std::endl;
         TEST_ASSERT( v[ 1 ][ "description" ].base().position->line == 22 );
         TEST_ASSERT( v[ 1 ][ "description" ].base().position->byte_in_line == 23 );
      }

   } // namespace json

} // namespace tao

#include "main.hpp"
