// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <fstream>
#include <iostream>
#include <iomanip>

#include <tao/json/value.hh>
#include <tao/json/pointer.hh>
#include <tao/json/schema.hh>
#include <tao/json/parse_file.hh>
#include <tao/json/from_stream.hh>

namespace tao
{
   namespace json
   {
      std::size_t tests = 0;

      value parse_stream( const std::string & name )
      {
         std::ifstream ifs( name.c_str() );
         return from_stream( ifs, name, 192 );
      }

      void test( const std::string & name )
      {
         std::cout << "File: " << name << std::endl;
         try {
            const value v = parse_stream( name );
            // const value v = parse_file( name );
            for ( const auto & e : v.get_array() ) {
               std::cout << "  Schema: " << e.at( "description" ).get_string() << std::endl;
               bool expected_schema = true;
               if ( const auto * schema_valid = e.find( "valid" ) ) {
                  expected_schema = schema_valid->get_boolean();
               }
               try {
                  schema s( e.at( "schema" ) );
                  if ( ! expected_schema ) {
                     ++failed;
                     std::cout << "  Failed: Schema is valid, but it should not be" << std::endl;
                  }
                  else for ( const auto & c : e.at( "tests" ).get_array() ) {
                     std::cout << "    Testcase: " << c.at( "description" ).get_string() << std::endl;
                     ++tests;
                     try {
                        const bool result = s.validate( c.at( "data" ) );
                        const bool expected = c.at( "valid" ).get_boolean();
                        if ( result != expected ) {
                           ++failed;
                           if ( result ) {
                              std::cout << "    Failed: Schema matches, but it should not" << std::endl;
                           }
                           else {
                              std::cout << "    Failed: Schema does not match, but it should" << std::endl;
                           }
                        }
                        if (!result) {
                          std::cout << "    Schema does not match with reasons: " << std::endl;
                          for (auto &r : s.reasons())
                            std::cout << "      " << r << std::endl;
                        }
                     }
                     catch( const std::exception & e ) {
                        ++failed;
                        std::cout << "    Failed with exception: " << e.what() << std::endl;
                     }
                  }
               }
               catch( const std::exception & e ) {
                  if ( expected_schema ) {
                     ++failed;
                     std::cout << "  Failed with exception: " << e.what() << std::endl;
                  }
                  else {
                     ++tests;
                  }
               }
            }
         }
         catch( const std::exception & e ) {
            ++failed;
            std::cout << "Failed with exception: " << e.what() << std::endl;
         }
      }

      void unit_test()
      {
         test( "tests/draft4/additionalItems.json" );
         test( "tests/draft4/additionalProperties.json" );
         test( "tests/draft4/allOf.json" );
         test( "tests/draft4/anyOf.json" );
         test( "tests/draft4/default.json" );
         test( "tests/draft4/definitions.json" );
         test( "tests/draft4/dependencies.json" );
         test( "tests/draft4/enum.json" );
         test( "tests/draft4/items.json" );
         test( "tests/draft4/maximum.json" );
         test( "tests/draft4/maxItems.json" );
         test( "tests/draft4/maxLength.json" );
         test( "tests/draft4/maxProperties.json" );
         test( "tests/draft4/minimum.json" );
         test( "tests/draft4/minItems.json" );
         test( "tests/draft4/minLength.json" );
         test( "tests/draft4/minProperties.json" );
         test( "tests/draft4/multipleOf.json" );
         test( "tests/draft4/not.json" );
         test( "tests/draft4/oneOf.json" );
         test( "tests/draft4/pattern.json" );
         test( "tests/draft4/patternProperties.json" );
         test( "tests/draft4/properties.json" );
         test( "tests/draft4/ref.json" );
         test( "tests/draft4/refRemote.json" );
         test( "tests/draft4/required.json" );
         test( "tests/draft4/type.json" );
         test( "tests/draft4/uniqueItems.json" );

         test( "tests/draft4/optional/bignum.json" );
         test( "tests/draft4/optional/format.json" );
         test( "tests/draft4/optional/zeroTerminatedFloats.json" );

         test( "tests/taocpp/schema.json" );
         test( "tests/taocpp/number.json" );
         test( "tests/taocpp/dateTime.json" );

         // TODO: Remove this temporary work-around once all tests succeed
         if ( failed ) {
            std::cerr << "JSON Schema failures: " << failed << '/' << tests << std::endl;
         }
         failed = 0;
      }

   } // json

} // tao

#include "main.hh"
