// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_TO_VALUE_HPP
#define TAO_JSON_EVENTS_TO_VALUE_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "../external/byte.hpp"
#include "../external/string_view.hpp"

#include "../byte_view.hpp"
#include "../value.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer to build a JSON Value.

         template< template< typename... > class Traits, typename Base = internal::empty_base >
         class to_basic_value
         {
         private:
            std::vector< basic_value< Traits, Base > > stack_;
            std::vector< std::string > keys_;

         public:
            basic_value< Traits, Base > value;

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

            void string( const tao::string_view v )
            {
               value.unsafe_emplace_string( v.data(), v.size() );
            }

            void string( const char* v )
            {
               value.unsafe_emplace_string( v );
            }

            void string( std::string&& v )
            {
               value.unsafe_assign_string( std::move( v ) );
            }

            void binary( const tao::byte_view v )
            {
               value.unsafe_emplace_binary( v.begin(), v.end() );
            }

            void binary( std::vector< tao::byte >&& v )
            {
               value.unsafe_assign_binary( std::move( v ) );
            }

            void begin_array()
            {
               stack_.push_back( empty_array );
            }

            void begin_array( const std::size_t size )
            {
               begin_array();
               stack_.back().unsafe_get_array().reserve( size );
            }

            void element()
            {
               stack_.back().unsafe_emplace_back( std::move( value ) );
               value.discard();
            }

            void end_array( const std::size_t /*unused*/ = 0 )
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               stack_.push_back( empty_object );
            }

            void key( const tao::string_view v )
            {
               keys_.emplace_back( v.data(), v.size() );
            }

            void key( const char* v )
            {
               keys_.emplace_back( v );
            }

            void key( std::string&& v )
            {
               keys_.push_back( std::move( v ) );
            }

            void member()
            {
               stack_.back().unsafe_emplace( std::move( keys_.back() ), std::move( value ) );
               value.discard();
               keys_.pop_back();
            }

            void end_object( const std::size_t /*unused*/ = 0 )
            {
               value = std::move( stack_.back() );
               stack_.pop_back();
            }
         };

         using to_value = to_basic_value< traits >;

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
