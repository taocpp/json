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
         // template< template< typename ... > class Traits >
         // class value_handler
         // {
         //    void null() {}
         //    void bool_( const bool v ) {}
         //    void number( const std::int64_t v ) {}
         //    void number( const std::uint64_t v ) {}
         //    void number( const double v ) {}
         //    void string( std::string && v ) {}
         //    void begin_array() {}
         //    void commit_element() {}
         //    void end_array() {}
         //    void begin_object() {}
         //    void commit_key( std::string && key ) {}
         //    void commit_member() {}
         //    void end_object() {}
         // };

         template< template< typename ... > class Traits >
         struct value_builder
         {
            basic_value< Traits > value;

            basic_value< Traits > * current;
            std::vector< basic_value< Traits > * > stack;

            value_builder() : current( & value ) {}

            void null() { current->unsafe_assign_null(); }
            void bool_( const bool v ) { current->unsafe_assign_bool( v ); }
            void number( const std::int64_t v ) { current->unsafe_assign_signed( v ); }
            void number( const std::uint64_t v ) { current->unsafe_assign_unsigned( v ); }
            void number( const double v ) { current->unsafe_assign_double( v ); }
            void string( std::string && v ) { current->unsafe_emplace_string( std::move( v ) ); }

            // array
            void begin_array()
            {
               current->unsafe_emplace_array();
               current->unsafe_emplace_back( json::null );
               stack.push_back( current );
               current = & current->unsafe_get_array().back();
            }

            void commit_element()
            {
               stack.back()->unsafe_emplace_back( json::null );
               current = & stack.back()->unsafe_get_array().back();
            }

            void end_array()
            {
               current = stack.back();
               current->unsafe_get_array().pop_back();
               stack.pop_back();
            }

            // object
            void begin_object()
            {
               current->unsafe_emplace_object();
            }

            void commit_key( std::string && key )
            {
               const auto r = current->unsafe_emplace( std::move( key ), json::null );
               if ( ! r.second ) {
                  // TODO: throw on duplicate key? offer a choice?
                  r.first->second = json::null;
               }
               stack.push_back( current );
               current = & r.first->second;
            }

            void commit_member()
            {
               current = stack.back();
               stack.pop_back();
            }

            void end_object() {}
         };

      } // internal

   } // json

} // tao

#endif
