// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include "nlohmann/json.hpp"

#include <tao/json/sax/from_string.hh>
#include <tao/json/sax/to_stream.hh>

namespace tao
{
   namespace json
   {
      namespace
      {
         // SAX consumer to build an nlohmann/json value
         class to_nlohmann
         {
         private:
            std::vector< nlohmann::json > stack_;
            std::vector< std::string > keys_;

         public:
            nlohmann::json value;

            void null()
            {
               value = nullptr;
            }

            void boolean( const bool v )
            {
               value = v;
            }

            void number( const std::int64_t v )
            {
               value = v;
            }

            void number( const std::uint64_t v )
            {
               value = v;
            }

            void number( const double v )
            {
               value = v;
            }

            void string( const std::string & v )
            {
               value = v;
            }

            void string( std::string && v )
            {
               value = std::move( v );
            }

            // array
            void begin_array()
            {
               stack_.push_back( nlohmann::json::array() );
            }

            void element()
            {
               stack_.back().push_back( std::move( value ) );
            }

            void end_array()
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }

            // object
            void begin_object()
            {
               stack_.push_back( nlohmann::json::object() );
            }

            void key( const std::string & v )
            {
               keys_.push_back( v );
            }

            void key( std::string && v )
            {
               keys_.push_back( std::move( v ) );
            }

            void member()
            {
               stack_.back().push_back( nlohmann::json::object_t::value_type( std::move( keys_.back() ), std::move( value ) ) );
               keys_.pop_back();
            }

            void end_object()
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }
         };

         // SAX producer for an nlohmann/json value
         template< typename Handler >
         void traverse_nlohmann( const nlohmann::json & v, Handler & handler )
         {
            switch( v.type() ) {
               case nlohmann::json::value_t::null:
                  handler.null();
                  break;
               case nlohmann::json::value_t::boolean:
                  handler.boolean( v.get< bool >() );
                  break;
               case nlohmann::json::value_t::number_integer:
                  handler.number( v.get< std::int64_t >() );
                  break;
               case nlohmann::json::value_t::number_unsigned:
                  handler.number( v.get< std::uint64_t >() );
                  break;
               case nlohmann::json::value_t::number_float:
                  handler.number( v.get< double >() );
                  break;
               case nlohmann::json::value_t::string:
                  handler.string( v.get_ref< const std::string & >() );
                  break;
               case nlohmann::json::value_t::array:
                  handler.begin_array();
                  for( const auto & e : v ) {
                     traverse_nlohmann( e, handler );
                     handler.element();
                  }
                  handler.end_array();
                  break;
               case nlohmann::json::value_t::object:
                  handler.begin_object();
                  for( nlohmann::json::const_iterator it = v.begin(); it != v.end(); ++it ) {
                     handler.key( it.key() );
                     traverse_nlohmann( it.value(), handler );
                     handler.member();
                  }
                  handler.end_object();
                  break;
               default:
                  throw std::logic_error( "invalid value for nohmann::json::type()" );  // LCOV_EXCL_LINE
            }
         }

      } //

      void unit_test()
      {
         // combine a taocpp/json SAX parser (using the PEGTL) with the above
         to_nlohmann handler;
         sax::from_string( "[ null, true, false, 42, 43.0, \"foo\", [ 1, 2, 3 ], { \"a\" : \"b\", \"c\" : \"d\" } ]", handler );

         const auto & v = handler.value;

         TEST_ASSERT( v.type() == nlohmann::json::value_t::array );
         TEST_ASSERT( v.size() == 8 );
         TEST_ASSERT( v[ 0 ] == nullptr );
         TEST_ASSERT( v[ 1 ].get< bool >() == true );
         TEST_ASSERT( v[ 2 ].get< bool >() == false );
         TEST_ASSERT( v[ 3 ] == 42 );
         TEST_ASSERT( v[ 4 ] == 43.0 );
         TEST_ASSERT( v[ 5 ] == "foo" );
         TEST_ASSERT( v[ 6 ].type() == nlohmann::json::value_t::array );
         TEST_ASSERT( v[ 6 ].size() == 3 );
         TEST_ASSERT( v[ 6 ][ 0 ] == 1 );
         TEST_ASSERT( v[ 6 ][ 1 ] == 2 );
         TEST_ASSERT( v[ 6 ][ 2 ] == 3 );
         TEST_ASSERT( v[ 7 ].type() == nlohmann::json::value_t::object );
         TEST_ASSERT( v[ 7 ].size() == 2 );
         TEST_ASSERT( v[ 7 ].at( "a" ) == "b" );
         TEST_ASSERT( v[ 7 ].at( "c" ) == "d" );

         std::ostringstream oss;
         sax::to_stream oss_handler( oss );
         traverse_nlohmann( v, oss_handler );

         TEST_ASSERT( oss.str() == "[null,true,false,42,43.0,\"foo\",[1,2,3],{\"a\":\"b\",\"c\":\"d\"}]" );
      }

   } // json

} // tao

#include "main.hh"
