// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_POSITION_HPP
#define TAO_JSON_CONTRIB_POSITION_HPP

#include <tao/json/events/to_value.hpp>
#include <tao/json/events/transformer.hpp>
#include <tao/json/parse_file.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      struct position
      {
         std::size_t line = 0;
         std::size_t byte_in_line = 0;
         std::string source;

         position() noexcept  // NOLINT
         {
         }

         position( const position& ) = default;

         position( position&& p ) noexcept
            : line( p.line ),
              byte_in_line( p.byte_in_line ),
              source( std::move( p.source ) )
         {
         }

         ~position() = default;

         position& operator=( const position& ) = default;

         position& operator=( position&& p ) noexcept
         {
            line = p.line;
            byte_in_line = p.byte_in_line;
            source = std::move( p.source );
            return *this;
         }
      };

      inline std::ostream& operator<<( std::ostream& o, const position& p )
      {
         o << p.source << ':' << p.line << ':' << p.byte_in_line;
         return o;
      }

      inline std::string to_string( const position& p )
      {
         std::ostringstream o;
         o << p;
         return o.str();
      }

      namespace internal
      {
         template< typename Rule >
         struct position_action
            : public action< Rule >
         {
         };

         template<>
         struct position_action< rules::sor_value >
         {
            template< typename Input, typename Consumer >
            static void apply( const Input& in, Consumer& consumer )
            {
               const auto p = in.position();
               consumer.value.base().line = p.line;
               consumer.value.base().byte_in_line = p.byte_in_line;
               consumer.value.base().source = p.source;
            }
         };

      }  // namespace internal

      template< template< typename... > class Traits, template< typename... > class... Transformers >
      basic_value< Traits, position > basic_parse_file_with_position( const std::string& filename )
      {
         events::transformer< events::to_basic_value< Traits, position >, Transformers... > consumer;
         json_pegtl::file_input< json_pegtl::tracking_mode::IMMEDIATE > in( filename );
         json_pegtl::parse< internal::grammar, internal::position_action, internal::control >( in, consumer );
         return std::move( consumer.value );
      }

      template< template< typename... > class... Transformers >
      basic_value< traits, position > parse_file_with_position( const std::string& filename )
      {
         return basic_parse_file_with_position< traits, Transformers... >( filename );
      }

   }  // namespace json

}  // namespace tao

#endif
