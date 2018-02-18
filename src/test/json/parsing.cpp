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

#include <tao/json/cbor/parts_parser.hpp>
#include <tao/json/parts_parser.hpp>

namespace tao
{
   namespace json
   {
      template< typename T, typename = void >
      struct my_traits
         : public traits< T >
      {
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
         : binding::object< TAO_JSON_BIND_REQUIRED( "s", &derived_one::s ) >
      {
      };

      // TODO: Find way to handle inheritance.

      template<>
      struct my_traits< derived_two >
         : binding::object< TAO_JSON_BIND_REQUIRED( "i", &derived_two::i ) >
      {
      };

      template<>
      struct my_traits< std::shared_ptr< base_class > >
         : binding::factory< base_class,
                             TAO_JSON_FACTORY_BIND( "one", derived_one ),
                             TAO_JSON_FACTORY_BIND( "two", derived_two ) >
      {
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

            template< template< typename... > class Traits = traits, typename Parts >
            static void consume( Parts& parser, C& /*unused*/ )
            {
               if( parser.boolean() != V ) {
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

            template< template< typename... > class Traits = traits, typename Parts >
            static void consume( Parts& parser, C& /*unused*/ )
            {
               if( parser.number_signed() != V ) {
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

            template< template< typename... > class Traits = traits, typename Parts >
            static void consume( Parts& parser, C& /*unused*/ )
            {
               if( parser.number_unsigned() != V ) {
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

            template< template< typename... > class Traits = traits, typename Parts >
            static void consume( Parts& parser, T& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               if( parser.string() != s ) {
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
         : binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                           TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >
      {
      };

      namespace internal
      {
         template< typename Tuple, typename Indices >
         struct tuple_array_binding;

         template< typename... Ts, std::size_t... Is >
         struct tuple_array_binding< std::tuple< Ts... >, TAO_JSON_PEGTL_NAMESPACE::internal::index_sequence< Is... > >
         {
            using tuple_t = std::tuple< Ts... >;
            using type = binding::array< binding::element< const typename std::tuple_element< Is, tuple_t >::type& ( * )( const tuple_t& ), &std::get< Is, Ts... > >... >;
         };

      }  // namespace internal

      template< typename... Ts >
      struct my_traits< std::tuple< Ts... > >
         : public internal::tuple_array_binding< std::tuple< Ts... >, TAO_JSON_PEGTL_NAMESPACE::internal::make_index_sequence< sizeof...( Ts ) > >::type
      {
      };

      struct foo
      {
         std::string a = "hello";
         std::string b = "world";
      };

      struct foo_version_two
         : binding::array< TAO_JSON_BIND_ELEMENT( &foo::a ),
                           TAO_JSON_BIND_ELEMENT( &foo::b ),
                           binding::element_u< foo, 2 > >
      {
      };

      struct foo_version_one
         : binding::array< TAO_JSON_BIND_ELEMENT( &foo::a ),
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
         : binding::object< TAO_JSON_BIND_REQUIRED( "i", &bar::i ),
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
         : binding::array< TAO_JSON_BIND_ELEMENT( &baz::get_i ),
                           TAO_JSON_BIND_ELEMENT( &get_j ) >
      {
      };

      using my_value = basic_value< my_traits >;

      void unit_test()
      {
         {
            const std::tuple< int, std::string, double > b{ 42, "hallo", 3.0 };
            basic_value< my_traits > v = b;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 3 );
            TEST_ASSERT( v[ 0 ] == 42 );
            TEST_ASSERT( v[ 1 ] == std::string( "hallo" ) );
            TEST_ASSERT( v[ 2 ] == 3.0 );
         }
         {
            baz b;
            basic_value< my_traits > v = b;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 2 );
            TEST_ASSERT( v[ 0 ] == -1 );
            TEST_ASSERT( v[ 1 ] == -2 );
         }
         {
            parts_parser pp( "  [  \"kasimir\"  ,  \"fridolin\", 2  ]  " );
            const auto f = consume< foo, my_traits >( pp );
            TEST_ASSERT( f.a == "kasimir" );
            TEST_ASSERT( f.b == "fridolin" );
         }
         {
            parts_parser pp( "  [  ]  " );
            const auto v = consume< std::shared_ptr< std::vector< int > >, my_traits >( pp );
            TEST_ASSERT( v->empty() );
         }
         {
            parts_parser pp( "  [  1  , 2, 3  ]  " );
            const auto v = consume< std::vector< int >, my_traits >( pp );
            TEST_ASSERT( v.size() == 3 );
            TEST_ASSERT( v[ 0 ] == 1 );
            TEST_ASSERT( v[ 1 ] == 2 );
            TEST_ASSERT( v[ 2 ] == 3 );
         }
         {
            cbor::parts_parser pp( "80" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.empty() );
         }
         {
            cbor::parts_parser pp( "8a00010203040506070809" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.size() == 10 );
            for( std::size_t i = 0; i < 10; ++i ) {
               TEST_ASSERT( v[ i ] == i );
            }
         }
         {
            cbor::parts_parser pp( "9f00010203040506070809ff" );
            const auto v = consume< std::vector< unsigned >, my_traits >( pp );
            TEST_ASSERT( v.size() == 10 );
            for( std::size_t i = 0; i < 10; ++i ) {
               TEST_ASSERT( v[ i ] == i );
            }
         }
         {
            cbor::parts_parser pp( "8261616162" );
            const auto v = consume< foo, my_traits >( pp );
            TEST_ASSERT( v.a == "a" );
            TEST_ASSERT( v.b == "b" );
         }
         {
            parts_parser pp( " { \"a\" : 4, \"b\" : 5 } " );
            const auto v = consume< std::map< std::string, int >, my_traits >( pp );
            TEST_ASSERT( v.size() == 2 );
            TEST_ASSERT( v.at( "a" ) == 4 );
            TEST_ASSERT( v.at( "b" ) == 5 );
         }
         {
            parts_parser pp( " { \"c\" : \"yeah\" , \"i\" : 42 } " );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "yeah" );
            TEST_ASSERT( v.i == 42 );
         }
         {
            parts_parser pp( " { \"i\" : 42 } " );
            const auto v = consume< bar, my_traits >( pp );
            TEST_ASSERT( v.c == "c" );
            TEST_ASSERT( v.i == 42 );
         }
         {
            cbor::parts_parser pp( "bf616364796561686169182aff" );
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
            parts_parser pp( "{ \"two\":{ \"i\" : 17 }}" );
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
