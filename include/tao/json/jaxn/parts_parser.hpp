// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_JAXN_PARTS_PARSER_HPP
#define TAO_JSON_JAXN_PARTS_PARSER_HPP

#include "../parts_parser.hpp"

#include "internal/bunescape_action.hpp"
#include "internal/grammar.hpp"
#include "internal/unescape_action.hpp"

namespace tao::json::jaxn
{
   namespace internal
   {
      namespace rules
      {
         struct wss
            : pegtl::star< ws >
         {
         };

         template< typename Rule >
         struct integer_action
            : pegtl::nothing< Rule >
         {
         };

         template<>
         struct integer_action< pegtl::integer::signed_rule >
            : pegtl::integer::signed_action
         {
         };

         template<>
         struct integer_action< pegtl::integer::unsigned_rule >
            : pegtl::integer::unsigned_action
         {
         };

      }  // namespace rules

      template< typename I >
      struct integer_state
      {
         I converted = 0;  // TODO: Remove superfluous initialisation when we manage to shup up the warnings on all compilers.
      };

   }  // namespace internal

   // TODO: Optimise some of the simpler cases?

   template< typename Input = pegtl::string_input< pegtl::tracking_mode::lazy, pegtl::eol::lf_crlf, std::string > >
   class basic_parts_parser
   {
   public:
      template< typename... Ts >
      explicit basic_parts_parser( Ts&&... ts )
         : m_input( std::forward< Ts >( ts )... )
      {
         pegtl::parse< internal::rules::wss >( m_input );
      }

      bool empty()  // noexcept( noexcept( m_input.empty() ) )
      {
         return m_input.empty();
      }

      bool null()
      {
         return pegtl::parse< pegtl::seq< json::internal::rules::null, jaxn::internal::rules::wss > >( m_input );
      }

      bool boolean()
      {
         bool r;
         pegtl::parse< pegtl::must< json::internal::rules::boolean, jaxn::internal::rules::wss > >( m_input, r );
         return r;
      }

      double number_double()
      {
         return 42.0;  // TODO
      }

      std::int64_t number_signed()
      {
         // TODO: Error on leading zero after sign.
         internal::integer_state< std::int64_t > st;
         pegtl::parse< pegtl::must< pegtl::sor< pegtl::one< '0' >, pegtl::integer::signed_rule >, internal::rules::wss >, internal::rules::integer_action >( m_input, st );
         return st.converted;
      }

      std::uint64_t number_unsigned()
      {
         internal::integer_state< std::uint64_t > st;
         pegtl::parse< pegtl::must< pegtl::sor< pegtl::one< '0' >, pegtl::integer::unsigned_rule >, internal::rules::wss >, internal::rules::integer_action >( m_input, st );
         return st.converted;
      }

      std::string string()
      {
         std::string unescaped;
         pegtl::parse< pegtl::must< internal::rules::string, jaxn::internal::rules::wss >, internal::unescape_action >( m_input, unescaped );
         return unescaped;
      }

      // TODO: std::string_view string_view() that only works for strings without escape sequences?

      std::vector< std::byte > binary()
      {
         std::vector< std::byte > data;
         pegtl::parse< pegtl::must< internal::rules::binary, jaxn::internal::rules::wss >, internal::bunescape_action >( m_input, data );
         return data;
      }

      std::string key()
      {
         std::string unescaped;
         pegtl::parse< pegtl::must< internal::rules::mkey, internal::rules::wss, pegtl::one< ':' >, internal::rules::wss >, internal::unescape_action >( m_input, unescaped );
         return unescaped;
      }

      // TODO: std::string_view key_view() that only works for identifiers (and possibly strings without escape sequences)?

   protected:
      template< char C >
      void parse_single_must()
      {
         pegtl::parse< pegtl::must< pegtl::one< C >, internal::rules::wss > >( m_input );
      }

      template< char C >
      bool parse_single_test()
      {
         return pegtl::parse< pegtl::seq< pegtl::one< C >, internal::rules::wss > >( m_input );
      }

      struct state_t
      {
         std::size_t i = 0;
         static constexpr std::size_t* size = nullptr;
      };

   public:
      state_t begin_array()
      {
         parse_single_must< '[' >();
         return state_t();
      }

      void end_array( state_t& /*unused*/ )
      {
         parse_single_must< ']' >();
      }

      void element( state_t& p )
      {
         if( p.i++ ) {
            parse_single_must< ',' >();
         }
      }

      bool element_or_end_array( state_t& p )
      {
         if( parse_single_test< ']' >() ) {
            return false;
         }
         element( p );
         return true;
      }

      state_t begin_object()
      {
         parse_single_must< '{' >();
         return state_t();
      }

      void end_object( state_t& /*unused*/ )
      {
         parse_single_must< '}' >();
      }

      void member( state_t& p )
      {
         if( p.i++ ) {
            parse_single_must< ',' >();
         }
      }

      bool member_or_end_object( state_t& p )
      {
         if( parse_single_test< '}' >() ) {
            return false;
         }
         member( p );
         return true;
      }

      void skip_value()
      {
         pegtl::parse< pegtl::must< pegtl::json::value > >( m_input );  // Includes right-padding.
      }

      auto mark()  // noexcept( noexcept( m_input.template mark< pegtl::rewind_mode::required >() ) )
      {
         return m_input.template mark< pegtl::rewind_mode::required >();
      }

      template< typename T >
      void throw_parse_error( T&& t ) const
      {
         throw pegtl::parse_error( std::forward< T >( t ), m_input );
      }

   protected:
      Input m_input;
   };

   using parts_parser = basic_parts_parser<>;

}  // namespace tao::json::jaxn

#endif
