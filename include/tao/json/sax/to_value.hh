// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_TO_VALUE_HH
#define TAOCPP_JSON_INCLUDE_SAX_TO_VALUE_HH

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // General interface for a SAX handler:
         //
         // class value_handler
         // {
         //    void null() {}
         //
         //    void boolean( const bool v ) {}
         //
         //    void number( const std::int64_t v ) {}
         //    void number( const std::uint64_t v ) {}
         //    void number( const double v ) {}
         //
         //    // the producer may call either one of the next two for a string,
         //    // only implement the first one if you don't need the second.
         //    void string( const std::string & v ) {}
         //    void string( std::string && v ) {}
         //
         //    // array
         //    void begin_array() {}
         //    void element() {}
         //    void end_array() {}
         //
         //    // object
         //    void begin_object() {}
         //    // the producer may call either one of the next two for a key,
         //    // only implement the first one if you don't need the second.
         //    void key( const std::string & v ) {}
         //    void key( std::string && v ) {}
         //    void member() {}
         //    void end_object() {}
         // };

         // SAX consumer to build a JSON value
         template< template< typename ... > class Traits >
         class to_basic_value
         {
         private:
            std::vector< basic_value< Traits > > stack_;
            std::vector< std::string > keys_;

         public:
            basic_value< Traits > value;

            void null()
            {
               value.unsafe_assign_null();
            }

            void boolean( const bool v )
            {
               value.unsafe_assign_bool( v );
            }

            void number( const std::int64_t v )
            {
               value.unsafe_assign_signed( v );
            }

            void number( const std::uint64_t v )
            {
               value.unsafe_assign_unsigned( v );
            }

            void number( const double v )
            {
               value.unsafe_assign_double( v );
            }

            void string( const std::string & v )
            {
               value.unsafe_emplace_string( v );
            }

            void string( std::string && v )
            {
               value.unsafe_emplace_string( std::move( v ) );
            }

            // array
            void begin_array()
            {
               stack_.push_back( empty_array );
            }

            void element()
            {
               stack_.back().unsafe_emplace_back( std::move( value ) );
            }

            void end_array()
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }

            // object
            void begin_object()
            {
               stack_.push_back( empty_object );
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
               stack_.back().unsafe_emplace( std::move( keys_.back() ), std::move( value ) );
               keys_.pop_back();
            }

            void end_object()
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }
         };

         using to_value = to_basic_value< traits >;

      } // sax

   } // json

} // tao

#endif
