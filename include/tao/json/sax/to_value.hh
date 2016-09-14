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
         //    void boolean( const bool v ) {}
         //    void number( const std::int64_t v ) {}
         //    void number( const std::uint64_t v ) {}
         //    void number( const double v ) {}
         //    void string( std::string v ) {}
         //    void begin_array() {}
         //    void element() {}
         //    void end_array() {}
         //    void begin_object() {}
         //    void key( std::string v ) {}
         //    void value() {}
         //    void end_object() {}
         // };

         template< template< typename ... > class Traits >
         class to_basic_value
         {
         private:
            basic_value< Traits > value_;

            std::vector< basic_value< Traits > > stack_;
            std::vector< std::string > keys_;

         public:
            basic_value< Traits > & result() noexcept
            {
               return value_;
            }

            const basic_value< Traits > & result() const noexcept
            {
               return value_;
            }

            void null()
            {
               value_.unsafe_assign_null();
            }

            void boolean( const bool v )
            {
               value_.unsafe_assign_bool( v );
            }

            void number( const std::int64_t v )
            {
               value_.unsafe_assign_signed( v );
            }

            void number( const std::uint64_t v )
            {
               value_.unsafe_assign_unsigned( v );
            }

            void number( const double v )
            {
               value_.unsafe_assign_double( v );
            }

            void string( std::string v )
            {
               value_.unsafe_emplace_string( std::move( v ) );
            }

            // array
            void begin_array()
            {
               stack_.push_back( empty_array );
            }

            void element()
            {
               stack_.back().unsafe_emplace_back( std::move( value_ ) );
            }

            void end_array()
            {
               value_ = std::move( stack_.back() );
               stack_.pop_back();
            }

            // object
            void begin_object()
            {
               stack_.push_back( empty_object );
            }

            void key( std::string v )
            {
               keys_.push_back( std::move( v ) );
            }

            void value()
            {
               stack_.back().unsafe_emplace( std::move( keys_.back() ), std::move( value_ ) );
               keys_.pop_back();
            }

            void end_object()
            {
               value_ = std::move( stack_.back() );
               stack_.pop_back();
            }
         };

         using to_value = to_basic_value< traits >;

      } // sax

   } // json

} // tao

#endif
