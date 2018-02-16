// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <bitset>
#include <limits>
#include <list>
#include <map>
#include <type_traits>
#include <vector>

#include "../../test/json/test.hpp"
#include "../../test/json/test_unhex.hpp"

#include <tao/json.hpp>
#include <tao/json/binding.hpp>
#include <tao/json/consume.hpp>

#include <tao/json/external/pegtl/contrib/integer.hpp>
#include <tao/json/external/pegtl/contrib/json.hpp>

// EVERYTHING IN THIS FILE IS STILL HIGHLY EXPERIMENTAL!!!

#ifdef _MSC_VER
#pragma warning( disable : 4127 )
#endif

namespace tao
{
   namespace json
   {
      // struct producer_t
      // {
      //    constexpr explicit producer_t( int /*unused*/ )
      //    {
      //    }
      // };

      // constexpr producer_t producer{ 0 };  // NOLINT

      namespace internal
      {
         template< typename Input, typename... Ts >
         void throw_parse_error( Input& in, const Ts&... ts )
         {
            std::ostringstream oss;
            (void)json::internal::swallow{ ( operator<<( oss, ts ), true )... };
            throw json_pegtl::parse_error( oss.str(), in );  // NOLINT
         }

      }  // namespace internal

      namespace cbor
      {
         template< utf8_mode V = utf8_mode::CHECK >
         class parser
         {
         public:
            using Input = json_pegtl::memory_input< json_pegtl::tracking_mode::LAZY >;

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

            void check_major( const internal::major m, const char* e )
            {
               const auto b = internal::peek_major_safe( m_input );
               if( b != m ) {
                  throw json_pegtl::parse_error( e, m_input );  // NOLINT
               }
            }

            template< utf8_mode U, typename T >
            T cbor_string( const internal::major m, const char* e )
            {
               check_major( m, e );
               if( internal::peek_minor( m_input ) != internal::minor_mask ) {
                  return internal::parse_string_1< U, T >( m_input );
               }
               return internal::parse_string_n< U, T >( m_input, m );
            }

            std::string string()
            {
               return cbor_string< V, std::string >( internal::major::STRING, "expected cbor string" );
            }

            std::string binary()
            {
               return cbor_string< utf8_mode::TRUST, std::vector< tao::byte > >( internal::major::BINARY, "expected cbor binary" );
            }

            std::string key()
            {
               return string();
            }

            // This would not work with fragmented CBOR strings (no contiguous block) and regular JSON (escaping); keep anyway, and to binary_view, too??
            tao::string_view string_view()
            {
               const auto b = internal::peek_byte_safe( m_input );
               if( b != std::uint8_t( internal::major::STRING ) + internal::minor_mask ) {
                  throw json_pegtl::parse_error( "expected cbor definitive string", m_input );  // NOLINT
               }
               return internal::parse_string_1< V, tao::string_view >( m_input );
            }

            std::int64_t number_int64_unsigned()
            {
               const auto u = internal::parse_unsigned( m_input );
               if( u > 9223372036854775807ull ) {
                  throw json_pegtl::parse_error( "negative integer overflow", m_input );  // NOLINT
               }
               return std::int64_t( u );
            }

            std::int64_t number_int64_negative()
            {
               const auto u = internal::parse_unsigned( m_input );
               if( u > 9223372036854775808ull ) {
                  throw json_pegtl::parse_error( "negative integer overflow", m_input );  // NOLINT
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
               check_major( internal::major::UNSIGNED, "expected cbor unsigned" );
               return internal::parse_unsigned( m_input );
            }

            double number_double()
            {
               const auto b = internal::peek_byte_safe( m_input );
               switch( b ) {
                  case std::uint8_t( internal::major::OTHER ) + 25:
                     return internal::parse_floating_half_impl( m_input );
                  case std::uint8_t( internal::major::OTHER ) + 26:
                     return internal::parse_floating_impl< float >( m_input );
                  case std::uint8_t( internal::major::OTHER ) + 27:
                     return internal::parse_floating_impl< double >( m_input );
                  default:
                     throw json_pegtl::parse_error( "expected cbor floating point", m_input );  // NOLINT
               }
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
               check_major( m, e );
               if( internal::peek_minor( m_input ) == 31 ) {
                  m_input.bump_in_this_line( 1 );
                  return state_t();
               }
               return state_t( internal::parse_size( m_input ) );
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

            void skip_value()
            {
               // TODO: Optimise?
               json::events::discard consumer;
               json_pegtl::parse< json_pegtl::must< internal::data< V > > >( m_input, consumer );
            }

            auto mark() -> decltype( std::declval< Input >().template mark< json_pegtl::rewind_mode::REQUIRED >() )
            {
               return m_input.template mark< json_pegtl::rewind_mode::REQUIRED >();
            }

            template< typename... Ts >
            void throw_parse_error( const Ts&... ts )
            {
               json::internal::throw_parse_error( m_input, ts... );
            }

         private:
            Input m_input;
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

         template< typename... Ts >
         void throw_parse_error( const Ts&... ts )
         {
            internal::throw_parse_error( m_input, ts... );
         }

      private:
         Input m_input;
      };

      template< typename T, typename = void >
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
            return static_cast< int >( producer.number_int64() );
         }
      };

      template<>
      struct my_traits< unsigned >
         : traits< unsigned >
      {
         template< template< typename... > class Traits, typename Producer >
         static unsigned consume( Producer& producer )
         {
            return static_cast< unsigned >( producer.number_uint64() );
         }
      };

      template< typename T >
      struct my_traits< std::list< T > >
         : public traits< std::list< T > >
      {
         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& producer, std::list< T >& v )
         {
            auto p = producer.begin_array();
            while( producer.element_or_end_array( p ) ) {
               v.emplace_back( json::consume< T, Traits >( producer ) );
            }
         }
      };

      template< typename T >
      struct my_traits< std::vector< T > >
         : public traits< std::vector< T > >
      {
         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& producer, std::vector< T >& v )
         {
            auto p = producer.begin_array();
            if( p.size ) {
               v.reserve( *p.size );
            }
            while( producer.element_or_end_array( p ) ) {
               v.emplace_back( json::consume< T, Traits >( producer ) );
            }
         }
      };

      template< typename T >
      struct my_traits< std::map< std::string, T > >
         : public traits< std::map< std::string, T > >
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

      template< typename T, typename U = T >
      struct my_shared_traits
         : public shared_traits< T, U >
      {
         template< template< typename... > class Traits, typename Producer >
         static std::shared_ptr< U > consume( Producer& producer )
         {
            if( producer.null() ) {
               return std::shared_ptr< U >();
            }
            auto t = std::make_shared< T >();  // TODO: More control?
            json::consume< Traits >( producer, *t );
            return t;
         }
      };

      template< typename T >
      struct my_traits< std::shared_ptr< T > >
         : public my_shared_traits< T >
      {
         template< typename U >
         using with_base = my_shared_traits< T, U >;
      };

      template< typename... As >
      struct my_array
         : public binding::array< As... >
      {
         template< typename A, template< typename... > class Traits = traits, typename Producer, typename C, typename P >
         static bool consume_element( Producer& producer, C& x, P& p )
         {
            producer.element( p );
            A::template consume< Traits >( producer, x );
            return true;
         }

         template< template< typename... > class Traits = traits, typename Producer, typename C >
         static void consume( Producer& producer, C& x )
         {
            auto p = producer.begin_array();
            (void)internal::swallow{ consume_element< As, Traits >( producer, x, p )... };
            producer.end_array( p );
         }
      };

      // TODO: Control how to create the instances?

      template< binding::for_unknown_key E, typename... As >
      struct my_object
         : public binding::basic_object< E, As... >
      {
         template< typename F >
         struct entry
         {
            entry( F c, std::size_t i )
               : function( c ),
                 index( i )
            {
            }

            F function;
            std::size_t index;
         };

         template< typename A, template< typename... > class Traits, typename Producer, typename F >
         static bool emplace_consume( std::map< std::string, entry< F > >& m, std::size_t& i )
         {
            m.emplace( A::key(), entry< F >( &A::template consume< Traits, Producer >, i++ ) );
            return true;
         }

         template< template< typename... > class Traits = traits, typename Producer, typename C >
         static void consume( Producer& producer, C& x )
         {
            using F = void ( * )( Producer&, C& );
            static const std::map< std::string, entry< F > > m = []( std::size_t i ) {
               std::map< std::string, entry< F > > t;
               (void)json::internal::swallow{ emplace_consume< As, Traits, Producer >( t, i )... };
               assert( t.size() == sizeof...( As ) );
               return t;
            }( 0 );
            static const std::bitset< sizeof...( As ) > o = []( std::size_t i ) {
               std::bitset< sizeof...( As ) > t;
               (void)internal::swallow{ binding::basic_object< E, As... >::template set_optional_bit< As >( t, i )... };
               return t;
            }( 0 );

            auto p = producer.begin_object();
            std::bitset< sizeof...( As ) > b;
            while( producer.member_or_end_object( p ) ) {
               const auto k = producer.key();
               const auto i = m.find( k );
               if( i == m.end() ) {
                  binding::internal::throw_o< E >::r_continue( k );
                  producer.skip_value();
                  continue;
               }
               if( b.test( i->second.index ) ) {
                  producer.throw_parse_error( "duplicate object key ", internal::escape( k ) );
               }
               i->second.function( producer, x );
               b.set( i->second.index );
            }
            b |= o;
            if( !b.all() ) {
               // TODO: List the missing required key(s) in the exception?
               throw std::runtime_error( "missing required key(s)" );  // NOLINT
            }
         }
      };

      namespace binding
      {
         template< typename... Vs >
         struct versions;

         template< typename V >
         struct versions< V >
            : public V
         {
            // TODO: Or static_assert more than one version?
         };

         template< typename V, typename... Vs >
         struct versions< V, Vs... >
            : public V
         {
            // NOTE: This is a bit like a high-level pegtl::sor<>.
            // TODO: Clear x between attempts?

            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool as_attempt( const basic_value< Traits, Base >& v, C& x )
            {
               try {
                  A::as( v, x );
                  return true;
               }
               catch( ... ) {
                  return false;
               }
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& x )
            {
               bool ok = false;
               (void)json::internal::swallow{ ok = as_attempt< V >( v, x ), ( ok = ok || as_attempt< Vs >( v, x ) )... };
               if( ! ok ) {
                  throw std::runtime_error( "all versions failed" );  // NOLINT
               }
            }

            template< typename A, template< typename... > class Traits, typename Producer, typename C >
            static bool consume_attempt( Producer& producer, C& x )
            {
               try {
                  auto m = producer.mark();
                  A::template consume< Traits >( producer, x );
                  return m( true );
               }
               catch( ... ) {
                  return false;
               }
            }

            template< template< typename... > class Traits, typename Producer, typename C >
            static void consume( Producer& producer, C& x )
            {
               bool ok = false;
               (void)json::internal::swallow{ ok = consume_attempt< V, Traits >( producer, x ), ( ok = ok || consume_attempt< Vs, Traits >( producer, x ) )... };
               if( ! ok ) {
                  throw std::runtime_error( "all versions failed" );  // NOLINT
               }
            }
         };

      }  // namespace binding

      template< typename U, typename... Ts >
      struct my_factory
         : binding::factory< U, Ts... >
      {
         template< typename F >
         struct entry
         {
            explicit entry( F c )
               : function( c )
            {
            }

            F function;
         };

         template< typename V, template< typename... > class Traits, typename Producer, typename F >
         static bool emplace_consume( std::map< std::string, entry< F > >& m )
         {
            using T = typename V::template type< U, Traits >;
            m.emplace( T::name(), entry< F >( &T::template consume< Traits, Producer > ) );
            return true;
         }

         template< template< typename... > class Traits, typename Producer >
         static std::shared_ptr< U > consume( Producer& producer )
         {
            using F = std::shared_ptr< U > ( * )( Producer & producer );
            static const std::map< std::string, entry< F > > m = []() {
               std::map< std::string, entry< F > > t;
               (void)json::internal::swallow{ emplace_consume< Ts, Traits, Producer >( t )... };
               return t;
            }();

            auto p = producer.begin_object();
            const auto k = producer.key();
            const auto i = m.find( k );
            if( i == m.end() ) {
               throw std::runtime_error( "unknown factory type " + json::internal::escape( k ) );  // NOLINT
            }
            auto r = i->second.function( producer );
            producer.end_object( p );
            return r;
         }
      };

      class base_class
      {
      public:
         virtual ~base_class() = default;

         base_class( const base_class& ) = delete;
         base_class( base_class&& ) = delete;
         void operator=( const base_class& ) = delete;
         void operator=( base_class&& ) = delete;

      protected:
         base_class() = default;
      };

      class derived_one
         : public base_class
      {
      public:
         std::string s;
      };

      class derived_two
         : public base_class
      {
      public:
         int i = 0;
      };

      template<>
      struct my_traits< derived_one >
         : my_object< binding::for_unknown_key::THROW,
                      TAO_JSON_BIND_REQUIRED( "s", &derived_one::s ) >
      {
      };

      // TODO: Find way to handle inheritance.

      template<>
      struct my_traits< derived_two >
         : my_object< binding::for_unknown_key::THROW,
                      TAO_JSON_BIND_REQUIRED( "i", &derived_two::i ) >
      {
      };

      template<>
      struct my_traits< std::shared_ptr< base_class > >
         : my_factory< base_class,
                       TAO_JSON_FACTORY_BIND( "one", derived_one ),
                       TAO_JSON_FACTORY_BIND( "two", derived_two ) >
      {
      };

      template<>
      struct my_traits< std::string >
         : public traits< std::string >
      {
         template< template< typename... > class Traits, typename Producer >
         static std::string consume( Producer& producer )
         {
            return producer.string();
         }
      };

      namespace binding
      {
         template< typename C, bool V >
         struct element_b
         {
            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.boolean( V );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& producer, C& /*unused*/ )
            {
               if( producer.boolean() != V ) {
                  throw std::runtime_error( "value mismatch" );  // NOLINT
               }
            }
         };

         template< typename C, std::int64_t V >
         struct element_i
         {
            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.number( V );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& producer, C& /*unused*/ )
            {
               if( producer.number_int64() != V ) {
                  throw std::runtime_error( "value mismatch" );  // NOLINT
               }
            }
         };

         template< typename C, std::uint64_t V >
         struct element_u
         {
            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.number( V );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& producer, C& /*unused*/ )
            {
               if( producer.number_uint64() != V ) {
                  throw std::runtime_error( "value mismatch" );  // NOLINT
               }
            }
         };

         template< char... Cs >
         using string = json_pegtl::string< Cs... >;

         template< typename T, typename S >
         struct element_s;

         template< typename T, char... Cs >
         struct element_s< T, json_pegtl::string< Cs... > >
         {
            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const T& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               consumer.string( tao::string_view( s, sizeof...( Cs ) ) );
            }

            template< template< typename... > class Traits = traits, typename Producer >
            static void consume( Producer& producer, T& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               if( producer.string() != s ) {
                  throw std::runtime_error( "value mismatch" );  // NOLINT
               }
            }
         };

         template< member_kind R, typename K, typename T, bool B >
         struct member_b
            : public element_b< T, B >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, typename T, std::int64_t V >
         struct member_i
            : public element_i< T, V >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, typename T, std::uint64_t V >
         struct member_u
            : public element_u< T, V >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, typename T, typename S >
         struct member_s
            : public element_s< T, S >,
              public member_key< R, K >
         {
         };

      }  // namespace binding

      template< typename U, typename V >
      struct my_traits< std::pair< U, V > >
         : my_array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                     TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >
      {
      };

      struct foo
      {
         std::string a = "hello";
         std::string b = "world";
      };

      struct foo_version_two
         : my_array< TAO_JSON_BIND_ELEMENT( &foo::a ),
                     TAO_JSON_BIND_ELEMENT( &foo::b ),
                     binding::element_u< foo, 2 > >
      {
      };

      struct foo_version_one
         : my_array< TAO_JSON_BIND_ELEMENT( &foo::a ),
                     TAO_JSON_BIND_ELEMENT( &foo::b ) >
      {
      };

      template<>
      struct my_traits< foo >
         : binding::versions< foo_version_two,
                              foo_version_one >
      {
      };

      struct bar
      {
         int i = -1;
         std::string c = "c";
      };

      template<>
      struct my_traits< bar >
         : my_object< binding::for_unknown_key::THROW,
                      TAO_JSON_BIND_REQUIRED( "i", &bar::i ),
                      TAO_JSON_BIND_OPTIONAL( "c", &bar::c ) >
      {
      };

      struct baz
      {
         int get_i() const
         {
            return i;
         }

         int i = -1;
         int j = -2;
      };

      int get_j( const baz& b )
      {
         return b.j;
      }

      template<>
      struct my_traits< baz >
         : my_array< TAO_JSON_BIND_ELEMENT( &baz::get_i ),
                     TAO_JSON_BIND_ELEMENT( &get_j ) >
      {
      };

      using my_value = basic_value< my_traits >;

      void unit_test()
      {
         {
            baz b;
            basic_value< my_traits > v = b;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 2 );
            TEST_ASSERT( v[ 0 ] == -1 );
            TEST_ASSERT( v[ 1 ] == -2 );
         }
         {
            parse_producer<> pp( "  [  \"kasimir\"  ,  \"fridolin\", 2  ]  " );
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
            parse_producer<> pp( " { \"i\" : 42 } " );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "c" );
            TEST_ASSERT( v.i == 42 );
         }
         {
            cbor::parser<> pp( "bf616364796561686169182aff" );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "yeah" );
            TEST_ASSERT( v.i == 42 );
         }
         {
            const basic_value< my_traits > v = {
               { "c", "x" },
               { "i", 2 }
            };
            const auto x = v.as< bar >();
            TEST_ASSERT( x.c == "x" );
            TEST_ASSERT( x.i == 2 );
         }
         {
            parse_producer<> pp( "{ \"two\":{ \"i\" : 17 }}" );
            const auto v = consume< std::shared_ptr< base_class >, my_traits >( pp );
            const auto c = std::dynamic_pointer_cast< derived_two >( v );
            TEST_ASSERT( c );
            TEST_ASSERT( c->i == 17 );
         }
         {
            basic_value< my_traits > v;
            v[ "two" ][ "i" ] = 17;
            const auto x = v.as< std::shared_ptr< base_class > >();
            const auto c = std::dynamic_pointer_cast< derived_two >( x );
            TEST_ASSERT( c );
            TEST_ASSERT( c->i == 17 );
         }
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
