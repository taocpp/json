// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_PARTS_PARSER_HPP
#define TAO_JSON_PARTS_PARSER_HPP

#include "external/pegtl.hpp"
#include "external/pegtl/contrib/integer.hpp"
#include "external/pegtl/contrib/json.hpp"

#include "internal/format.hpp"
#include "internal/grammar.hpp"
#include "internal/string_state.hpp"
#include "internal/unescape_action.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace rules
         {
            struct wss
               : json_pegtl::star< ws >
            {
            };

            struct boolean
            {
               using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::ANY >;

               template< json_pegtl::apply_mode,
                         json_pegtl::rewind_mode,
                         template< typename... > class,
                         template< typename... > class,
                         typename Input >
               static bool match( Input& in, bool& st ) noexcept( noexcept( in.size( 5 ) ) )
               {
                  const auto s = in.size( 5 );
                  if( s >= 5 ) {
                     if( std::memcmp( in.current(), "false", 5 ) == 0 ) {
                        in.bump_in_this_line( 5 );
                        st = false;
                        return true;
                     }
                  }
                  if( s >= 4 ) {
                     if( std::memcmp( in.current(), "true", 4 ) == 0 ) {
                        in.bump_in_this_line( 4 );
                        st = true;
                        return true;
                     }
                  }
                  return false;
               }
            };

            template< typename I >
            struct integer_state
            {
               I converted = 0;
            };

            template< typename Rule >
            struct integer_action
               : json_pegtl::nothing< Rule >
            {
            };

            template<>
            struct integer_action< json_pegtl::integer::signed_rule >
               : json_pegtl::integer::signed_action
            {
            };

            template<>
            struct integer_action< json_pegtl::integer::unsigned_rule >
               : json_pegtl::integer::unsigned_action
            {
            };

         }  // namespace rules

      }  // namespace internal

      // TODO: Optimise some of the simpler cases?

      template< typename Input = json_pegtl::string_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, std::string > >
      class basic_parts_parser
      {
      public:
         template< typename... Ts >
         explicit basic_parts_parser( Ts&&... ts )
            : m_input( std::forward< Ts >( ts )... )
         {
            json_pegtl::parse< internal::rules::wss >( m_input );
         }

         bool null()
         {
            return json_pegtl::parse< json_pegtl::seq< internal::rules::null, internal::rules::wss > >( m_input );
         }

         bool boolean()
         {
            bool r;
            json_pegtl::parse< json_pegtl::must< internal::rules::boolean, internal::rules::wss > >( m_input, r );
            return r;
         }

         double number_double()
         {
            // TODO
            assert( false );
            return 42.0;
         }

         std::int64_t number_signed()
         {
            internal::rules::integer_state< std::int64_t > st;
            json_pegtl::parse< json_pegtl::must< json_pegtl::sor< json_pegtl::one< '0' >, json_pegtl::integer::signed_rule >, internal::rules::wss >, internal::rules::integer_action >( m_input, st );
            return st.converted;
         }

         std::uint64_t number_unsigned()
         {
            internal::rules::integer_state< std::uint64_t > st;
            json_pegtl::parse< json_pegtl::must< json_pegtl::sor< json_pegtl::one< '0' >, json_pegtl::integer::unsigned_rule >, internal::rules::wss >, internal::rules::integer_action >( m_input, st );
            return st.converted;
         }

         std::string string()
         {
            internal::string_state s;
            json_pegtl::parse< json_pegtl::must< internal::rules::string, internal::rules::wss >, internal::unescape_action >( m_input, s );
            return std::move( s.unescaped );
         }

         std::vector< tao::byte > binary()
         {
            throw std::runtime_error( "binary not supported" );  // NOLINT
         }

         std::string key()
         {
            internal::string_state s;
            json_pegtl::parse< json_pegtl::must< internal::rules::string, internal::rules::wss, json_pegtl::one< ':' >, internal::rules::wss >, internal::unescape_action >( m_input, s );
            return std::move( s.unescaped );
         }

         template< char C >
         void parse_single_must()
         {
            json_pegtl::parse< json_pegtl::must< json_pegtl::one< C >, internal::rules::wss > >( m_input );
         }

         template< char C >
         bool parse_single_test()
         {
            return json_pegtl::parse< json_pegtl::seq< json_pegtl::one< C >, internal::rules::wss > >( m_input );
         }

         struct state_t
         {
            std::size_t i = 0;
            static constexpr std::size_t* size = nullptr;
         };

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
            json_pegtl::parse< json_pegtl::must< json_pegtl::json::value > >( m_input );  // Includes standard JSON right-padding.
         }

         auto mark() -> decltype( std::declval< Input >().template mark< json_pegtl::rewind_mode::REQUIRED >() )
         {
            return m_input.template mark< json_pegtl::rewind_mode::REQUIRED >();
         }

         template< typename T >
         void throw_parse_error( T&& t ) const
         {
            throw json_pegtl::parse_error( std::forward< T >( t ), m_input );
         }

      private:
         Input m_input;
      };

      using parts_parser = basic_parts_parser<>;

   }  // namespace json

}  // namespace tao

#endif
