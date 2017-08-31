// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/stream.hpp>
#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      struct user
      {
         bool is_human;
         std::string name;
         unsigned age;

         user( const bool h, const std::string& n, const unsigned a )
            : is_human( h ), name( n ), age( a )
         {
         }
      };

      template<>
      struct traits< user >
      {
         static const char* default_key;

         static void assign( custom_value& v, const user& u )
         {
            v = {
               { "is_human", u.is_human },
               { "name", u.name },
               { "age", u.age }
            };
         }

         template< typename T >
         static user as( const T& v )
         {
            return user( v.at( "is_human" ).get_boolean(),
                         v.at( "name" ).get_string(),
                         v.at( "age" ).template as< unsigned >() );
         }
      };

      const char* traits< user >::default_key = "user";

      void unit_test()
      {
         user u( true, "Daniel", 44 );

         user u2( false, "Rusty", 3 );
         user u3( false, "Amelie", 3 );
         user u4( false, "Milow", 2 );
         user u5( false, "Jessy", 1 );

         custom_value v = {
            u,
            { "cats", custom_value::array( { u2, u3, u4, u5 } ) }
         };

         std::cout << std::setw( 2 ) << v << std::endl;

         user r = v.at( "user" ).as< user >();
         std::cout << std::setw( 2 ) << custom_value( r ) << std::endl;
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
