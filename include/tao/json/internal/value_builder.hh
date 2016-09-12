// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_BUILDER_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_VALUE_BUILDER_HH

#include "../value.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // General interface for a SAX handler:
         //
         // class value_handler
         // {
         //    void null() {}
         //    void true_() {}
         //    void false_() {}
         //    void number( const std::int64_t v ) {}
         //    void number( const std::uint64_t v ) {}
         //    void number( const double v ) {}
         //    void string( std::string v ) {}
         //    void begin_array() {}
         //    void commit_element() {}
         //    void element_separator() {}
         //    void end_array() {}
         //    void begin_object() {}
         //    void commit_key( std::string v ) {}
         //    void name_separator() {}
         //    void commit_member() {}
         //    void value_separator() {}
         //    void end_object() {}
         // };

         template< template< typename ... > class Traits >
         struct value_builder
         {
            basic_value< Traits > value;
            std::vector< basic_value< Traits > > stack;
            std::vector< std::string > keys;

            void null() { value.unsafe_assign_null(); }
            void true_() { value.unsafe_assign_bool( true ); }
            void false_() { value.unsafe_assign_bool( false ); }
            void number( const std::int64_t v ) { value.unsafe_assign_signed( v ); }
            void number( const std::uint64_t v ) { value.unsafe_assign_unsigned( v ); }
            void number( const double v ) { value.unsafe_assign_double( v ); }
            void string( std::string v ) { value.unsafe_emplace_string( std::move( v ) ); }

            // array
            void begin_array()
            {
               stack.push_back( empty_array );
            }

            void commit_element()
            {
               stack.back().unsafe_emplace_back( std::move( value ) );
            }

            void element_separator() {}

            void end_array()
            {
               value = std::move( stack.back() );
               stack.pop_back();
            }

            // object
            void begin_object()
            {
               stack.push_back( empty_object );
            }

            void commit_key( std::string v )
            {
               keys.push_back( std::move( v ) );
            }

            void name_separator() {}

            void commit_member()
            {
               stack.back().unsafe_emplace( std::move( keys.back() ), std::move( value ) );
               keys.pop_back();
            }

            void value_separator() {}

            void end_object()
            {
               value = std::move( stack.back() );
               stack.pop_back();
            }
         };

      } // internal

   } // json

} // tao

#endif
