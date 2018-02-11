// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_FROM_STREAM_HPP
#define TAO_JSON_JAXN_FROM_STREAM_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "../value.hpp"

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"

#include "events/from_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace jaxn
      {
         template< template< typename... > class Traits, typename Base, template< typename... > class... Transformers >
         basic_value< Traits, Base > basic_from_stream( std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
         {
            json::events::transformer< json::events::to_basic_value< Traits, Base >, Transformers... > consumer;
            events::from_stream( consumer, stream, source, maximum_buffer_size );
            return std::move( consumer.value );
         }

         template< template< typename... > class Traits, template< typename... > class... Transformers >
         basic_value< Traits > basic_from_stream( std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
         {
            return basic_from_stream< Traits, json::internal::empty_base, Transformers... >( stream, source, maximum_buffer_size );
         }

         template< template< typename... > class Traits, typename Base, template< typename... > class... Transformers >
         basic_value< Traits, Base > basic_from_stream( std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
         {
            return basic_from_stream< Traits, Base, Transformers... >( stream, source.c_str(), maximum_buffer_size );
         }

         template< template< typename... > class Traits, template< typename... > class... Transformers >
         basic_value< Traits > basic_from_stream( std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
         {
            return basic_from_stream< Traits, Transformers... >( stream, source.c_str(), maximum_buffer_size );
         }

         template< template< typename... > class... Transformers >
         value from_stream( std::istream& stream, const char* source = nullptr, const std::size_t maximum_buffer_size = 4000 )
         {
            return basic_from_stream< traits, Transformers... >( stream, source, maximum_buffer_size );
         }

         template< template< typename... > class... Transformers >
         value from_stream( std::istream& stream, const std::string& source, const std::size_t maximum_buffer_size = 4000 )
         {
            return basic_from_stream< traits, Transformers... >( stream, source.c_str(), maximum_buffer_size );
         }

      }  // namespace jaxn

   }  // namespace json

}  // namespace tao

#endif
