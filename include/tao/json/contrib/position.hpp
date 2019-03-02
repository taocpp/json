// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_POSITION_HPP
#define TAO_JSON_CONTRIB_POSITION_HPP

#include "../events/to_value.hpp"
#include "../events/transformer.hpp"
#include "../message_extension.hpp"
#include "../parse_file.hpp"
#include "../value.hpp"

namespace tao
{
   namespace json
   {
      struct position
      {
      private:
         std::size_t m_line = 0;
         std::size_t m_byte_in_line = 0;
         std::string m_source;

      public:
         position() noexcept  // NOLINT
         {
         }

         position( const position& ) = default;

         position( position&& p ) noexcept
         {
            m_line = p.m_line;
            m_byte_in_line = p.m_byte_in_line;
            m_source = std::move( p.m_source );
         }

         ~position() = default;

         position& operator=( const position& ) = default;

         position& operator=( position&& p ) noexcept
         {
            m_line = p.m_line;
            m_byte_in_line = p.m_byte_in_line;
            m_source = std::move( p.m_source );
            return *this;
         }

         const std::string& source() const noexcept
         {
            return m_source;
         }

         std::size_t line() const noexcept
         {
            return m_line;
         }

         std::size_t byte_in_line() const noexcept
         {
            return m_byte_in_line;
         }

         void set_source( const std::string& s )
         {
            m_source = s;
         }

         template< typename T >
         void set_position( const T& p )
         {
            m_line = p.line;
            m_byte_in_line = p.byte_in_line;
            m_source = p.source;
         }

         void append_message_extension( std::ostream& os ) const
         {
            os << '[' << m_source << ':' << m_line << ':' << m_byte_in_line << ']';
         }
      };

      namespace internal
      {
         template< typename Rule >
         struct position_action
            : public action< Rule >
         {
         };

         template<>
         struct position_action< rules::object::begin >
         {
            template< typename Input, typename Consumer >
            static void apply( const Input& in, Consumer& consumer )
            {
               action< rules::object::begin >::apply0( consumer );
               consumer.stack_.back().set_position( in.position() );
            }
         };

         template<>
         struct position_action< rules::array::begin >
         {
            template< typename Input, typename Consumer >
            static void apply( const Input& in, Consumer& consumer )
            {
               action< rules::array::begin >::apply0( consumer );
               consumer.stack_.back().set_position( in.position() );
            }
         };

         template<>
         struct position_action< rules::sor_value >
         {
            template< typename Input, typename Consumer >
            static void apply( const Input& in, Consumer& consumer )
            {
               consumer.value.set_position( in.position() );
            }
         };

         template< template< typename... > class Traits >
         struct position_traits
            : Traits< void >
         {
            template< typename >
            using public_base = position;
         };

      }  // namespace internal

      template< template< typename... > class Traits >
      struct make_position_traits
      {
         template< typename T >
         struct type
            : std::conditional_t< std::is_same_v< T, void >, internal::position_traits< Traits >, Traits< T > >
         {
         };
      };

      template< template< typename... > class Traits, template< typename... > class... Transformers >
      auto basic_parse_file_with_position( const std::string& filename )
      {
         events::transformer< events::to_basic_value< Traits >, Transformers... > consumer;
         json_pegtl::file_input< json_pegtl::tracking_mode::eager > in( filename );
         json_pegtl::parse< internal::grammar, internal::position_action, internal::errors >( in, consumer );
         return std::move( consumer.value );
      }

      template< template< typename... > class... Transformers >
      auto parse_file_with_position( const std::string& filename )
      {
         return basic_parse_file_with_position< make_position_traits< traits >::template type, Transformers... >( filename );
      }

   }  // namespace json

}  // namespace tao

#endif
