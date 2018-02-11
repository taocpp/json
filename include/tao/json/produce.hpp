// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_PRODUCE_HPP
#define TAO_JSON_PRODUCE_HPP

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "forward.hpp"

#include "events/produce.hpp"
#include "events/to_pretty_stream.hpp"
#include "events/to_stream.hpp"
#include "events/to_string.hpp"
#include "events/to_value.hpp"

namespace tao
{
   namespace json
   {
      namespace produce
      {
         template< template< typename... > class Traits = traits, typename Base = internal::empty_base, typename T >
         basic_value< Traits, Base > to_value( T&& t )
         {
            events::to_basic_value< Traits, Base > consumer;
            events::produce< Traits >( consumer, std::forward< T >( t ) );
            return std::move( consumer.value );
         }

         template< template< typename... > class Traits = traits, typename T >
         void to_stream( std::ostream& os, const T& t )
         {
            events::to_stream consumer( os );
            events::produce< Traits >( consumer, t );
         }

         template< template< typename... > class Traits = traits, typename T >
         void to_stream( std::ostream& os, const T& t, const std::size_t indent )
         {
            events::to_pretty_stream consumer( os, indent );
            events::produce< Traits >( consumer, t );
         }

         template< template< typename... > class Traits = traits, typename T, typename S >
         void to_stream( std::ostream& os, const T& t, const std::size_t indent, S&& eol )
         {
            events::to_pretty_stream consumer( os, indent, std::forward< S >( eol ) );
            events::produce< Traits >( consumer, t );
         }

         template< template< typename... > class Traits = traits, typename... Ts >
         std::string to_string( Ts&&... ts )
         {
            std::ostringstream oss;
            to_stream< Traits >( oss, std::forward< Ts >( ts )... );
            return oss.str();
         }

      }  // namespace produce

   }  // namespace json

}  // namespace tao

#endif
