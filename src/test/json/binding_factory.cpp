// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json.hpp>

#include <tao/json/contrib/traits.hpp>

namespace tao
{
   namespace json
   {
      class base
      {
      public:
         virtual ~base() = default;

         base( const base& ) = delete;
         base( base&& ) = delete;
         void operator=( const base& ) = delete;
         void operator=( base&& ) = delete;

      protected:
         base() = default;
      };

      class derived_1
         : public base
      {
      public:
         std::string s;
      };

      class derived_2
         : public base
      {
      public:
         int i = 105;
         int j = 106;
      };

      template<>
      struct traits< derived_1 >
         : binding::object< TAO_JSON_BIND_REQUIRED( "s", &derived_1::s ) >
      {
      };

      template<>
      struct traits< derived_2 >
         : binding::object< TAO_JSON_BIND_REQUIRED( "i", &derived_2::i ),
                            TAO_JSON_BIND_REQUIRED( "j", &derived_2::j ) >
      {
         TAO_JSON_DEFAULT_KEY( "two" );
      };

      template<>
      struct traits< std::shared_ptr< base > >
         : binding::factory< base,
                             TAO_JSON_FACTORY_BIND( "one", derived_1 ),
                             TAO_JSON_FACTORY_BIND1( derived_2 ) >
      {
      };

      template<>
      struct traits< std::unique_ptr< base > >
         : traits< std::shared_ptr< base > >::template with_pointer< std::unique_ptr >
      {
      };

      void unit_test_1()
      {
         const value v = {
            { "one", {
                  { "s", "foo" }
               }
            }
         };
         const auto a = v.as< std::shared_ptr< base > >();
         TEST_ASSERT( a );
         const auto b = std::dynamic_pointer_cast< derived_1 >( a );
         TEST_ASSERT( b );
         TEST_ASSERT( b->s == "foo" );
         const value w = a;
         TEST_ASSERT( w == v );
         const value x = produce::to_value( a );
         TEST_ASSERT( w == x );
         parts_parser p( to_string( v ), __FUNCTION__ );
         const auto c = consume< std::shared_ptr< base > >( p );
         TEST_ASSERT( c );
         const auto d = std::dynamic_pointer_cast< derived_1 >( c );
         TEST_ASSERT( d );
         TEST_ASSERT( d->s == "foo" );
      }

      void unit_test_2()
      {
         const value v = {
            { "one", {
                  { "s", "foo" }
               }
            }
         };
         const auto a = v.as< std::unique_ptr< base > >();
         TEST_ASSERT( a );
         const auto* b = dynamic_cast< derived_1* >( a.get() );
         TEST_ASSERT( b );
         TEST_ASSERT( b->s == "foo" );
         const value w = a;
         TEST_ASSERT( w == v );
         const value x = produce::to_value( a );
         TEST_ASSERT( w == x );
         parts_parser p( to_string( v ), __FUNCTION__ );
         const auto c = consume< std::unique_ptr< base > >( p );
         TEST_ASSERT( c );
         const auto d = dynamic_cast< derived_1* >( c.get() );
         TEST_ASSERT( d );
         TEST_ASSERT( d->s == "foo" );
      }

      void unit_test_3()
      {
         const value v = {
            { "two", {
                  { "i", 42 },
                  { "j", 23 }
               }
            }
         };
         const auto a = v.as< std::shared_ptr< base > >();
         TEST_ASSERT( a );
         const auto b = std::dynamic_pointer_cast< derived_2 >( a );
         TEST_ASSERT( b );
         TEST_ASSERT( b->i == 42 );
         TEST_ASSERT( b->j == 23 );
         const value w = a;
         TEST_ASSERT( w == v );
         const value x = produce::to_value( a );
         TEST_ASSERT( w == x );
         parts_parser p( to_string( v ), __FUNCTION__ );
         const auto c = consume< std::shared_ptr< base > >( p );
         TEST_ASSERT( c );
         const auto d = std::dynamic_pointer_cast< derived_2 >( c );
         TEST_ASSERT( d );
         TEST_ASSERT( d->i == 42 );
         TEST_ASSERT( d->j == 23 );
      }

      void unit_test_4()
      {
         const value v = {
            { "two", {
                  { "i", 42 },
                  { "j", 23 }
               }
            }
         };
         const auto a = v.as< std::unique_ptr< base > >();
         TEST_ASSERT( a );
         const auto* b = dynamic_cast< derived_2* >( a.get() );
         TEST_ASSERT( b );
         TEST_ASSERT( b->i == 42 );
         TEST_ASSERT( b->j == 23 );
         const value w = a;
         TEST_ASSERT( w == v );
         const value x = produce::to_value( a );
         TEST_ASSERT( w == x );
         parts_parser p( to_string( v ), __FUNCTION__ );
         const auto c = consume< std::unique_ptr< base > >( p );
         TEST_ASSERT( c );
         const auto* d = dynamic_cast< derived_2* >( c.get() );
         TEST_ASSERT( d );
         TEST_ASSERT( d->i == 42 );
         TEST_ASSERT( d->j == 23 );
      }

      void unit_test()
      {
         unit_test_1();
         unit_test_2();
         unit_test_3();
         unit_test_4();
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
