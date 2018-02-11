// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <limits>
#include <type_traits>

#include "../../test/json/test.hpp"
#include "../../test/json/test_unhex.hpp"

#include <tao/json.hpp>
#include <tao/json/binding.hpp>
#include <tao/json/consume.hpp>
#include <tao/json/external/pegtl/contrib/integer.hpp>

// EVERYTHING IN THIS FILE IS STILL HIGHLY EXPERIMENTAL!!!

namespace tao
{
   namespace json
   {
      namespace cbor
      {
         template< utf8_mode V = utf8_mode::CHECK >
         class parser
         {
         public:
            explicit parser( const std::string& data )
               : m_input( test_unhex( data ), __FUNCTION__ )
            {
            }

            bool null()
            {
               if( internal::peek_byte_safe( m_input ) == std::uint8_t( internal::major::OTHER ) + 22 ) {
                  m_input.bump_in_this_line( 1 );
                  return true;
               }
               return false;
            }

            bool boolean()
            {
               const auto b = internal::peek_byte_safe( m_input );
               switch( b ) {
                  case std::uint8_t( internal::major::OTHER ) + 20:
                  case std::uint8_t( internal::major::OTHER ) + 21:
                     m_input.bump_in_this_line( 1 );
                     return bool( b - std::uint8_t( internal::major::OTHER ) - 20 );
                  default:
                     throw json_pegtl::parse_error( "expected cbor boolean", m_input );  // NOLINT
               }
            }

            std::string string()
            {
               const auto b = internal::peek_major_safe( m_input );
               if( b != internal::major::STRING ) {
                  throw json_pegtl::parse_error( "expected cbor string", m_input );  // NOLINT
               }
               if( internal::peek_minor( m_input ) != internal::minor_mask ) {
                  return internal::data< V >::template read_string_1< V, std::string >( m_input );
               }
               return internal::data< V >::template read_string_n< V, std::string >( m_input, internal::major::STRING );
            }

            std::string key()
            {
               return string();
            }

            // This would not work with fragmented CBOR strings (no contiguous block) and regular JSON (escaping); keep anyway?
            tao::string_view string_view()
            {
               const auto b = internal::peek_byte_safe( m_input );
               if( b != std::uint8_t( internal::major::STRING ) + internal::minor_mask ) {
                  throw json_pegtl::parse_error( "expected cbor definite string", m_input );  // NOLINT
               }
               return internal::data< V >::template read_string_1< V, tao::string_view >( m_input );
            }

            std::int64_t number_int64_unsigned()
            {
               const auto u = internal::data< V >::read_unsigned( m_input );
               if( u > 9223372036854775807ull ) {
                  throw json_pegtl::parse_error( "negative integer overflow", m_input );
               }
               return std::int64_t( u );
            }

            std::int64_t number_int64_negative()
            {
               const auto u = internal::data< V >::read_unsigned( m_input );
               if( u > 9223372036854775808ull ) {
                  throw json_pegtl::parse_error( "negative integer overflow", m_input );
               }
               return std::int64_t( ~u );
            }

            std::int64_t number_int64()
            {
               const auto b = internal::peek_major_safe( m_input );
               switch( b ) {
                  case internal::major::UNSIGNED:
                     return number_int64_unsigned();
                  case internal::major::NEGATIVE:
                     return number_int64_negative();
                  default:
                     throw json_pegtl::parse_error( "expected cbor integer", m_input );  // NOLINT
               }
            }

            std::uint64_t number_uint64()
            {
               const auto b = internal::peek_major_safe( m_input );
               if( b != internal::major::UNSIGNED ) {
                  throw json_pegtl::parse_error( "expected cbor unsigned", m_input );  // NOLINT
               }
               return internal::data< V >::read_unsigned( m_input );
            }

            struct state_t
            {
               state_t() = default;

               explicit state_t( const std::size_t in_size )
                  : size( in_size )
               {
               }

               std::size_t i = 0;
               tao::optional< std::size_t > size;
            };

            state_t begin_container( const internal::major m, const char* e )
            {
               const auto b = internal::peek_major_safe( m_input );
               if( b != m ) {
                  throw json_pegtl::parse_error( e, m_input );  // NOLINT
               }
               if( internal::peek_minor( m_input ) == 31 ) {
                  m_input.bump_in_this_line( 1 );
                  return state_t();
               }
               return state_t( internal::data< V >::read_size( m_input ) );
            }

            state_t begin_array()
            {
               return begin_container( internal::major::ARRAY, "expected cbor array" );
            }

            state_t begin_object()
            {
               return begin_container( internal::major::OBJECT, "expected cbor object" );
            }

            void end_array_sized( state_t& p )
            {
               if( *p.size != p.i ) {
                  throw std::runtime_error( "cbor array size mismatch" );  // NOLINT
               }
            }

            void end_object_sized( state_t& p )
            {
               if( *p.size != p.i ) {
                  throw std::runtime_error( "cbor object size mismatch" );  // NOLINT
               }
            }

            void end_array_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) != 0xff ) {
                  throw std::runtime_error( "cbor array not at end" );  // NOLINT
               }
               m_input.bump_in_this_line( 1 );
            }

            void end_object_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) != 0xff ) {
                  throw std::runtime_error( "cbor object not at end" );  // NOLINT
               }
               m_input.bump_in_this_line( 1 );
            }

            void end_array( state_t& p )
            {
               if( p.size ) {
                  end_array_sized( p );
               }
               else {
                  end_array_indefinitive( p );
               }
            }

            void end_object( state_t& p )
            {
               if( p.size ) {
                  end_object_sized( p );
               }
               else {
                  end_object_indefinitive( p );
               }
            }

            void element_sized( state_t& p )
            {
               if( p.i++ >= *p.size ) {
                  throw std::runtime_error( "cbor array at end" );  // NOLINT
               }
            }

            void member_sized( state_t& p )
            {
               if( p.i++ >= *p.size ) {
                  throw std::runtime_error( "cbor object at end" );  // NOLINT
               }
            }

            void element_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  throw std::runtime_error( "cbor array at end" );  // NOLINT
               }
            }

            void member_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  throw std::runtime_error( "cbor object at end" );  // NOLINT
               }
            }

            void element( state_t& p )
            {
               if( p.size ) {
                  element_sized( p );
               }
               else {
                  element_indefinitive( p );
               }
            }

            void member( state_t& p )
            {
               if( p.size ) {
                  member_sized( p );
               }
               else {
                  member_indefinitive( p );
               }
            }

            bool element_or_end_array_sized( state_t& p )
            {
               return p.i++ < *p.size;
            }

            bool member_or_end_object_sized( state_t& p )
            {
               return p.i++ < *p.size;
            }

            bool element_or_end_array_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  m_input.bump_in_this_line( 1 );
                  return false;
               }
               return true;
            }

            bool member_or_end_object_indefinitive( state_t& /*unused*/ )
            {
               if( internal::peek_byte_safe( m_input ) == 0xff ) {
                  m_input.bump_in_this_line( 1 );
                  return false;
               }
               return true;
            }

            bool element_or_end_array( state_t& p )
            {
               if( p.size ) {
                  return element_or_end_array_sized( p );
               }
               return element_or_end_array_indefinitive( p );
            }

            bool member_or_end_object( state_t& p )
            {
               if( p.size ) {
                  return member_or_end_object_sized( p );
               }
               return member_or_end_object_indefinitive( p );
            }

         private:
            json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY > m_input;
         };

      }  // namespace cbor

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
               static bool match( Input& in, bool& st )
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
      // TODO: Support all integer types int8, uint8, ... , uint64?

      template< typename Input = json_pegtl::string_input< json_pegtl::tracking_mode::LAZY, json_pegtl::eol::lf_crlf, std::string >, typename Paddington = internal::rules::wss >
      class parse_producer
      {
      public:
         explicit parse_producer( const std::string& data )
            : m_input( data, __FUNCTION__ )
         {
            json_pegtl::parse< json_pegtl::star< internal::rules::ws > >( m_input );
         }

         bool null()
         {
            return json_pegtl::parse< json_pegtl::seq< internal::rules::null, Paddington > >( m_input );
         }

         bool boolean()
         {
            bool r;
            json_pegtl::parse< json_pegtl::must< internal::rules::boolean, Paddington > >( m_input, r );
            return r;
         }

         double number_double()
         {
            // TODO
            assert( false );
            return 42.0;
         }

         std::int64_t number_int64()
         {
            internal::rules::integer_state< std::int64_t > st;
            json_pegtl::parse< json_pegtl::must< json_pegtl::sor< json_pegtl::one< '0' >, json_pegtl::integer::signed_rule >, Paddington >, internal::rules::integer_action >( m_input, st );
            return st.converted;
         }

         std::uint64_t number_uint64()
         {
            internal::rules::integer_state< std::uint64_t > st;
            json_pegtl::parse< json_pegtl::must< json_pegtl::sor< json_pegtl::one< '0' >, json_pegtl::integer::unsigned_rule >, Paddington >, internal::rules::integer_action >( m_input, st );
            return st.converted;
         }

         std::string string()
         {
            internal::string_state s;
            json_pegtl::parse< json_pegtl::must< internal::rules::string, Paddington >, internal::unescape_action >( m_input, s );
            return std::move( s.unescaped );
         }

         std::vector< tao::byte > binary()
         {
            throw std::runtime_error( "binary not supported" );  // NOLINT
         }

         std::string key()
         {
            internal::string_state s;
            json_pegtl::parse< json_pegtl::must< internal::rules::string, Paddington, json_pegtl::one< ':' >, Paddington >, internal::unescape_action >( m_input, s );
            return std::move( s.unescaped );
         }

         template< char C >
         void parse_single_must()
         {
            json_pegtl::parse< json_pegtl::must< json_pegtl::one< C >, Paddington > >( m_input );
         }

         template< char C >
         bool parse_single_test()
         {
            return json_pegtl::parse< json_pegtl::seq< json_pegtl::one< C >, Paddington > >( m_input );
         }

         struct state_t
         {
            std::size_t i = 0;
            static constexpr bool size = false;
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

         void end_object()
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

      private:
         Input m_input;
      };

      template< typename T >
      struct my_traits
         : public traits< T >
      {
      };

      template<>
      struct my_traits< int >
         : traits< int >
      {
         template< template< typename... > class Traits, typename Producer >
         static int consume( Producer& producer )
         {
            return producer.number_int64();
         }
      };

      template<>
      struct my_traits< unsigned >
         : traits< unsigned >
      {
         template< template< typename... > class Traits, typename Producer >
         static unsigned consume( Producer& producer )
         {
            return producer.number_uint64();
         }
      };

      template< typename T >
      struct is_basic_value
         : public std::false_type
      {
      };

      template< template< typename... > class Traits, typename Base >
      struct is_basic_value< basic_value< Traits, Base > >
         : public std::true_type
      {
      };

      std::size_t size_helper( const bool /*unused*/ )
      {
         assert( !"something went wrong" );
         return 0;
      }

      std::size_t size_helper( const tao::optional< std::size_t >& size )
      {
         return *size;
      }

      template< typename T >
      struct my_traits< std::vector< T > >
      {
         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& producer, std::vector< T >& v )
         {
            auto p = producer.begin_array();
            if( p.size ) {
               v.reserve( size_helper( p.size ) );
            }
            while( producer.element_or_end_array( p ) ) {
               v.emplace_back( json::consume< T, Traits >( producer ) );
            }
         }
      };

      template< typename T >
      struct my_traits< std::map< std::string, T > >
      {
         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& producer, std::map< std::string, T >& v )
         {
            auto p = producer.begin_object();
            while( producer.member_or_end_object( p ) ) {
               auto k = producer.key();
               v.emplace( std::move( k ), json::consume< T, Traits >( producer ) );
            }
         }
      };

      template< typename T >
      struct my_traits< std::shared_ptr< T > >
      {
         template< template< typename... > class Traits, typename Producer >
         static std::shared_ptr< T > consume( Producer& producer )
         {
            if( producer.null() ) {
               return std::shared_ptr< T >();
            }
            return std::make_shared< T >( json::consume< T, Traits >( producer ) );
         }
      };

      template<>
      struct my_traits< std::string >
         : traits< std::string >
      {
         template< template< typename... > class Traits, typename Producer >
         static std::string consume( Producer& producer )
         {
            return producer.string();
         }
      };

      template< typename... As >
      struct my_array
         : public binding::array< As... >
      {
         template< template< typename... > class Traits = traits, typename Producer, typename C >
         static void consume( Producer& producer, C& x )
         {
            auto p = producer.begin_array();
            using swallow = bool[];
            (void)swallow{ ( producer.element( p ), As::template consume< Traits >( producer, x ), true )... };
            producer.end_array( p );
         }
      };

      template< typename... As >
      struct my_object
         : public binding::object< As... >
      {
         template< template< typename... > class Traits = traits, typename Producer, typename C >
         static void consume( Producer& producer, C& x )
         {
            auto p = producer.begin_object();
            using f = void ( * )( Producer&, C& );
            // TODO: Or make the map static and check for missing members otherwise?
            std::map< std::string, f > m = {
               { As::key(), &As::template consume< Traits, Producer > }...
            };
            while( producer.member_or_end_object( p ) ) {
               const auto k = producer.key();
               const auto i = m.find( k );
               if( i == m.end() ) {
                  throw std::runtime_error( "unknown or duplicate object key " + k );  // NOLINT
               }
               i->second( producer, x );
               m.erase( i );
            }
            if( !m.empty() ) {
               throw std::runtime_error( "missing required key(s)" );  // NOLINT
            }
         }
      };

      struct foo
      {
         std::string a = "hello";
         std::string b = "world";
      };

      template<>
      struct my_traits< foo >
         : my_array< TAO_JSON_BIND_ELEMENT( &foo::a ),
                     TAO_JSON_BIND_ELEMENT( &foo::b ) >
      {
      };

      struct bar
      {
         int i = -1;
         std::string c;
      };

      template<>
      struct my_traits< bar >
         : my_object< TAO_JSON_BIND_MEMBER( "i", &bar::i ),
                      TAO_JSON_BIND_MEMBER( "c", &bar::c ) >
      {
      };

      using my_value = basic_value< my_traits >;

      void unit_test()
      {
         {
            parse_producer<> pp( "  [  \"kasimir\"  ,  \"fridolin\"  ]  " );
            const auto f = consume< foo, my_traits >( pp );
            TEST_ASSERT( f.a == "kasimir" );
            TEST_ASSERT( f.b == "fridolin" );
         }
         {
            parse_producer<> pp( "  [  ]  " );
            const auto v = consume< std::shared_ptr< std::vector< int > >, my_traits >( pp );
            TEST_ASSERT( v->empty() );
         }
         {
            parse_producer<> pp( "  [  1  , 2, 3  ]  " );
            const auto v = consume< std::vector< int >, my_traits >( pp );
            TEST_ASSERT( v.size() == 3 );
            TEST_ASSERT( v[ 0 ] == 1 );
            TEST_ASSERT( v[ 1 ] == 2 );
            TEST_ASSERT( v[ 2 ] == 3 );
         }
         {
            cbor::parser<> pp( "80" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.empty() );
         }
         {
            cbor::parser<> pp( "8a00010203040506070809" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.size() == 10 );
            for( std::size_t i = 0; i < 10; ++i ) {
               TEST_ASSERT( v[ i ] == i );
            }
         }
         {
            cbor::parser<> pp( "9f00010203040506070809ff" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.size() == 10 );
            for( std::size_t i = 0; i < 10; ++i ) {
               TEST_ASSERT( v[ i ] == i );
            }
         }
         {
            cbor::parser<> pp( "8261616162" );
            const auto v = consume< foo, my_traits >( pp );
            TEST_ASSERT( v.a == "a" );
            TEST_ASSERT( v.b == "b" );
         }
         {
            parse_producer<> pp( " { \"a\" : 4, \"b\" : 5 } " );
            const auto v = consume< std::map< std::string, int >, my_traits >( pp );
            TEST_ASSERT( v.size() == 2 );
            TEST_ASSERT( v.at( "a" ) == 4 );
            TEST_ASSERT( v.at( "b" ) == 5 );
         }
         {
            parse_producer<> pp( " { \"c\" : \"yeah\" , \"i\" : 42 } " );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "yeah" );
            TEST_ASSERT( v.i == 42 );
         }
         {
            cbor::parser<> pp( "bf616364796561686169182aff" );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "yeah" );
            TEST_ASSERT( v.i == 42 );
         }
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
