// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/stream.hh>
#include <tao/json/to_string.hh>
#include <tao/json/value.hh>

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

         static void assign( value& v, const user& u )
         {
            v = {
               { "is_human", u.is_human },
               { "name", u.name },
               { "age", u.age }
            };
         }

         static user as( const value& v )
         {
            return user( v.at( "is_human" ).get_boolean(),
                         v.at( "name" ).get_string(),
                         v.at( "age" ).as< unsigned >() );
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

         value v = {
            u,
            { "cats", value::array( { u2, u3, u4, u5 } ) }
         };

         std::cout << std::setw( 2 ) << v << std::endl;

         user r = v.at( "user" ).as< user >();
         std::cout << std::setw( 2 ) << value( r ) << std::endl;
      }

   }  // json

}  // tao

#include "main.hh"
